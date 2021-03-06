#include <omp.h>
#include "yahtzee_game_map.h"
#include "yahtzee_probability_matrix.h"
#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

#define NBR_ROLLS (252)
//Shameless temporary define
int get_index(int scorecard_index, int roll_index, int reroll_index, int upper_score_index)
{
#if 0
    assert(scorecard_index < NBR_SCORECARDS);
    assert(roll_index < NBR_ROLLS);
    assert(reroll_index < NBR_REROLLS);
    assert(upper_score_index < MAX_UPPER_SCORE);
    assert(scorecard_index > -1);
    assert(roll_index > -1);
    assert(reroll_index > -1);
    assert(upper_score_index > -1);
#endif
    int index = 0;
    index += reroll_index * (MAX_UPPER_SCORE*NBR_SCORECARDS*NBR_ROLLS);
    index += upper_score_index * (NBR_SCORECARDS*NBR_ROLLS);
    index +=  scorecard_index * (NBR_ROLLS);
    index +=  roll_index;
    return index;
}


int find_first_bit_set(int a)
{
    int mask = 1;
    int tmp = a;
    int counter = 0;
    while(a >0){
        if( (a & mask) == 1){
            return counter;
        }
        a = a >> 1;
        counter++;
    }
    assert(false);
}

int get_precalculated_score(struct game_map* gm, int category_index, int roll_index){
    return gm->category_score_map[category_index*gm->prob_mat->cols + roll_index];
}

int precalculate_scores(struct game_map* gm)
{
    int category_index;
    int roll_index;
    for(category_index = 0; category_index < NBR_CATEGORIES; category_index++){
        for(roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
            gm->category_score_map[category_index*gm->prob_mat->cols + roll_index] = score_category(category_index, gm->prob_mat->rolls[roll_index].output_set);
        }
    }
}

void precalculate_forward_scorecards(struct game_map* gm)
{
    int scorecard;
    for(scorecard = 0; scorecard <NBR_SCORECARDS; scorecard++){
        int forward_scorecards[NBR_CATEGORIES];
        unsigned int nbr_forward_scorecards = 0;
        find_forward_scorecard(scorecard, forward_scorecards, &nbr_forward_scorecards);
        int i;
        for(i = 0; i < nbr_forward_scorecards; i++){
            gm->forward_scorecards[scorecard*NBR_CATEGORIES + i] = forward_scorecards[i];
        }
        gm->nbr_forward_scorecards[scorecard] = nbr_forward_scorecards;
    }
}

void get_forward_scorecards(struct game_map* gm, int scorecard, int** forward_scorecards, unsigned int* nbr_forward_scorecards )
{
   *forward_scorecards = &(gm->forward_scorecards[scorecard*NBR_CATEGORIES]);
   *nbr_forward_scorecards = gm->nbr_forward_scorecards[scorecard];
}

void strip_probability_map(struct game_map* gm)
{
    int prob_row;
    int prob_col;
    for(prob_row = 0; prob_row < gm->prob_mat->rows; prob_row++){
        for(prob_col = 0; prob_col < gm->prob_mat->cols; prob_col++){
            gm->stripped_prob_mat[prob_row*gm->prob_mat->cols + prob_col] = gm->prob_mat->rolls[prob_row*gm->prob_mat->cols + prob_col].probability;
        }
    }
}

float scorecard_max_expected_value(struct game_map* gm, int scorecard, int us_index, int roll_index, int subturn, int* best_move)
{
    float max_value = 0;
    int* forward_scorecards;
    unsigned int nbr_forward_scorecards = 0;
    get_forward_scorecards(gm, scorecard, &forward_scorecards, &nbr_forward_scorecards);
    int sc_index;
    int prob_col;
    struct roll* rolls = gm->prob_mat->rolls;
    for(sc_index = 0; sc_index < nbr_forward_scorecards; sc_index++){
        int fwd_scorecard = forward_scorecards[sc_index];
        int category_index = find_first_bit_set(fwd_scorecard - scorecard);
        int score = get_precalculated_score(gm, category_index, roll_index);
        int added_upper_score = (category_index < 6) * score;
        int tmp_upper_score = us_index + added_upper_score;
        if(tmp_upper_score > MAX_UPPER_SCORE -1){
            tmp_upper_score = MAX_UPPER_SCORE - 1;
        }
        float expected_value = 0;
        for(prob_col = 0; prob_col<gm->prob_mat->cols; prob_col++){
            int index = get_index(fwd_scorecard, prob_col, 0, tmp_upper_score);
            expected_value += gm->stripped_prob_mat[(gm->prob_mat->rows - 1)*gm->prob_mat->cols + prob_col] * gm->score_map[index];
        }
        expected_value += score;
        if (expected_value > max_value){
            max_value = expected_value;
            *best_move = category_index;
        }
    }
    return max_value;
}

void fill_select_subturn(struct game_map* gm, int turn, int* scorecards, unsigned int nbr_scorecards)
{
    int reroll_index = NBR_REROLLS - 1;
    int sc_index;
    int roll_index;
    int us_index;
    for( us_index = 0; us_index < MAX_UPPER_SCORE; us_index++){
        for( sc_index = 0; sc_index < nbr_scorecards; sc_index++){
            int scorecard = scorecards[sc_index];
#pragma omp parallel for
            for( roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                int best_move = 0;
                gm->score_map[index] = scorecard_max_expected_value(gm, scorecard, us_index, roll_index, reroll_index, &best_move);
                gm->best_moves[index] = best_move;
            }
        }
    }
}

float roll_max_expected_value(struct game_map* gm, int sc_index, int us_index, int roll_index, int subturn, int* best_move)
{
    int prob_row;
    int prob_col;
    float max_value = 0;
    for(prob_row =0; prob_row<gm->prob_mat->rows; prob_row++){
        if(gm->stripped_prob_mat[prob_row*gm->prob_mat->cols + roll_index] <= 0){
            continue;
        }
        float expected_value = 0;
        for(prob_col =0; prob_col<gm->prob_mat->cols; prob_col++){
            int index = get_index(sc_index, prob_col, subturn + 1, us_index);
            expected_value += gm->stripped_prob_mat[prob_row*gm->prob_mat->cols + prob_col] * gm->score_map[index];
        }
        if(expected_value > max_value){
            max_value = expected_value;
            *best_move = prob_row;
        }
    }
    return max_value;
}

void fill_roll_subturn(struct game_map* gm, int turn, int reroll_index, int* scorecards, unsigned int nbr_scorecards)
{
    int sc_index;
    int roll_index;
    int us_index;
    for( us_index = 0; us_index < MAX_UPPER_SCORE; us_index++){
        for( sc_index = 0; sc_index < nbr_scorecards; sc_index++){
            int scorecard = scorecards[sc_index];
#pragma omp parallel for
            for( roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                int best_move = 0;
                gm->score_map[index] = roll_max_expected_value(gm, scorecard, us_index, roll_index, reroll_index, &best_move);
                gm->best_moves[index] = best_move;
            }
        }
    }
}

void fill_turn(struct game_map* gm, int turn)
{
    int scorecards[NBR_SCORECARDS] = {0};
    unsigned int nbr_scorecards = 0;
    find_scorecards_for_turn(turn, scorecards, &nbr_scorecards);
    fill_select_subturn(gm, turn, scorecards, nbr_scorecards);
    int subturn = NBR_REROLLS - 2;
    while (subturn >= 0){
        printf(" Calculating : %i --- %i \n", turn, subturn);
        fill_roll_subturn(gm, turn, subturn, scorecards, nbr_scorecards);
        subturn--;
    }
}

//Last subturn is when we have filled all but one category on the scorecard
//We have only one option to fill, and will get a score based on that.
void fill_end_turn(struct game_map* gm)
{
    int sc_index;
    int roll_index;
    int us_index;
    int reroll_index = NBR_REROLLS -1;

    int scorecards[NBR_SCORECARDS] = {0};
    unsigned int nbr_scorecards = 0;
    find_scorecards_for_turn(NBR_CATEGORIES-1, scorecards, &nbr_scorecards);
    assert(nbr_scorecards == NBR_CATEGORIES);

    for( sc_index = 0; sc_index < nbr_scorecards; sc_index++){
        int scorecard = scorecards[sc_index];
        for( roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
            for( us_index = 0; us_index < MAX_UPPER_SCORE; us_index++){
                //All ones in binary i.e all categories filled
                int category_int = (1 << NBR_CATEGORIES) -1;
                //Remove all bits already set (now we have a single bit set at the category we need to fill)
                category_int -= scorecard;
                int category_index = find_first_bit_set(category_int);
                int score = get_precalculated_score(gm, category_index, roll_index);
                int added_upper_score = (category_index < 6) * score;
                if( us_index + added_upper_score >= MAX_UPPER_SCORE -1){
                    score += 50;
                }
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                gm->score_map[index] = score;
                gm->best_moves[index] = category_index;
            }
        }
    }

    reroll_index = NBR_REROLLS - 2;
    while (reroll_index >= 0){
        fill_roll_subturn(gm, NBR_CATEGORIES -1, reroll_index, scorecards, nbr_scorecards);
        reroll_index--;
    }
}

struct game_map* yahtzee_game_map_create()
{
    struct game_map* gm= calloc(1, sizeof(*gm));
    gm->prob_mat = prob_mat_create();
    gm->score_map = calloc(NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * MAX_UPPER_SCORE, sizeof(*(gm->score_map)));
    gm->best_moves = calloc(NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * MAX_UPPER_SCORE, sizeof(*(gm->best_moves)));
    gm->category_score_map = calloc(NBR_CATEGORIES * gm->prob_mat->cols, sizeof(*(gm->category_score_map)));
    gm->forward_scorecards = calloc(NBR_SCORECARDS * NBR_CATEGORIES, sizeof(*(gm->forward_scorecards)));
    gm->nbr_forward_scorecards = calloc(NBR_SCORECARDS, sizeof(*(gm->nbr_forward_scorecards)));
    gm->stripped_prob_mat = calloc(gm->prob_mat->rows * gm->prob_mat->cols, sizeof(*(gm->stripped_prob_mat)));
    precalculate_scores(gm);
    precalculate_forward_scorecards(gm);
    strip_probability_map(gm);
    return gm;
}

void yahtzee_game_map_destroy(struct game_map* gm)
{
    prob_mat_destroy(gm->prob_mat);
    free(gm->score_map);
    free(gm->category_score_map);
}

int main(void)
{
    struct game_map* gm = yahtzee_game_map_create();
    printf("%u \n", gm->prob_mat->cols);
    printf("Size of map : %ld \n", NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * MAX_UPPER_SCORE);
    fill_end_turn(gm);
    int turn = NBR_CATEGORIES -2;
    while(turn >= 0){
        fill_turn(gm, turn);
        turn--;
    }
    int prob_col;
    float expected_value = 0;
    for(prob_col = 0; prob_col<gm->prob_mat->cols; prob_col++){
        int index = get_index(0, prob_col, 0, 0);
        expected_value += gm->prob_mat->rolls[(gm->prob_mat->rows - 1)*gm->prob_mat->cols + prob_col].probability * gm->score_map[index];
        struct roll tmp = gm->prob_mat->rolls[(gm->prob_mat->rows - 1)*gm->prob_mat->cols + prob_col];
        printf(" Roll : %u, %u, %u, %u, %u, \t score : %4.8f \n", tmp.output_set[0], tmp.output_set[1], tmp.output_set[2], tmp.output_set[3], tmp.output_set[4], gm->score_map[index]);
    }
    printf("Expected value : %8.6f\n", expected_value);

    FILE *outfile;

    // open file for writing
    outfile = fopen ("output/yahtzee_score_table", "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }


    // write struct to file
    int upper_score;
    int reroll_index;
    for(reroll_index = 0; reroll_index < NBR_REROLLS; reroll_index++){
        for(upper_score = 0; upper_score < MAX_UPPER_SCORE; upper_score++){
            int index = 0;
            index += reroll_index * (MAX_UPPER_SCORE*NBR_SCORECARDS*NBR_ROLLS);
            index += upper_score * (NBR_SCORECARDS*NBR_ROLLS);
            fwrite(gm->score_map + index, sizeof(float), NBR_SCORECARDS*NBR_ROLLS, outfile);
        }
    }
    // close file
    fclose (outfile);

    // open file for writing
    outfile = fopen ("output/yahtzee_best_move_table", "w");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }

    // write struct to file
    for(reroll_index = 0; reroll_index < NBR_REROLLS; reroll_index++){
        for(upper_score = 0; upper_score < MAX_UPPER_SCORE; upper_score++){
            int index = 0;
            index += reroll_index * (MAX_UPPER_SCORE*NBR_SCORECARDS*NBR_ROLLS);
            index += upper_score * (NBR_SCORECARDS*NBR_ROLLS);
            fwrite(gm->best_moves + index, sizeof(short), NBR_SCORECARDS*NBR_ROLLS, outfile);
        }
    }
    // close file
    fclose (outfile);

    outfile = fopen ("output/yahtzee_forward_scorecards", "w");
    //Write all forward scorecards, including the zeros at the end, since no scorecard has 0 as a forward we can use this in python later
    //The zeros comes from allocating with calloc
    fwrite(gm->forward_scorecards, sizeof(*(gm->forward_scorecards)), NBR_SCORECARDS*NBR_CATEGORIES, outfile);
    fclose (outfile);

    int* category_score_map;

    outfile = fopen ("output/yahtzee_category_scores", "w");
    //Write all forward scorecards, including the zeros at the end, since no scorecard has 0 as a forward we can use this in python later
    //The zeros comes from allocating with calloc
    fwrite(gm->category_score_map, sizeof(*(gm->category_score_map)), NBR_CATEGORIES*gm->prob_mat->cols, outfile);
    fclose (outfile);

    outfile = fopen ("output/yahtzee_probability_matrix", "w");
    fwrite(gm->stripped_prob_mat, sizeof(*(gm->stripped_prob_mat)), gm->prob_mat->rows * gm->prob_mat->cols, outfile);
    fclose (outfile);

    outfile = fopen ("output/input_rolls", "w");
    int row;
    for(row = 0; row < gm->prob_mat->rows; row++){
        unsigned char* input_rolls = gm->prob_mat->rolls[row*gm->prob_mat->cols].input_set;
        unsigned char input_size = gm->prob_mat->rolls[row*gm->prob_mat->cols].input_set_size;
        int roll_index;
        for(roll_index = 0; roll_index < input_size; roll_index++){
            fprintf(outfile, "%u ", input_rolls[roll_index]);
        }
        fprintf(outfile, "\n");
    }
    fclose (outfile);

    outfile = fopen ("output/output_rolls", "w");
    int col;
    for(col = 0; col < gm->prob_mat->cols; col++){
        unsigned char* output_rolls = gm->prob_mat->rolls[col].output_set;
        unsigned char output_size = gm->prob_mat->rolls[col].output_set_size;
        int roll_index;
        for(roll_index = 0; roll_index < output_size; roll_index++){
            fprintf(outfile, "%u ", output_rolls[roll_index]);
        }
        fprintf(outfile, "\n");
    }
    fclose (outfile);

    outfile = fopen ("output/yahtzee_constants.py", "w");

    fprintf(outfile, "NBR_DICES = %i\n", NBR_DICES);
    fprintf(outfile, "NBR_FACES = %i\n", NBR_FACES);
    fprintf(outfile, "NBR_CATEGORIES = %i\n", NBR_CATEGORIES);
    fprintf(outfile, "NBR_SCORECARDS = %i\n", NBR_SCORECARDS);
    fprintf(outfile, "NBR_REROLLS = %i\n", NBR_REROLLS);
    fprintf(outfile, "MAX_UPPER_SCORE = %i\n", MAX_UPPER_SCORE);
    fprintf(outfile, "NBR_ROLLS = %i\n", NBR_ROLLS);
    fclose (outfile);
    yahtzee_game_map_destroy(gm);
    return 0;
}
