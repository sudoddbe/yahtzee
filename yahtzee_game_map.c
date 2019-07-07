#include "yahtzee_game_map.h"
#include "yahtzee_probability_matrix.h"
#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

#define NBR_ROLLS (252)
//Shameless temporary define
int get_index(int scorecard_index, int roll_index, int reroll_index, int upper_score_index)
{
#if 1
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

float scorecard_max_expected_value(struct game_map* gm, int scorecard, int us_index, int roll_index, int subturn)
{
    float max_value = 0;
    int forward_scorecards[NBR_CATEGORIES] = {0};
    unsigned int nbr_forward_scorecards = 0;
    find_forward_scorecard(scorecard, forward_scorecards, &nbr_forward_scorecards);
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
            expected_value += rolls[(gm->prob_mat->rows - 1)*gm->prob_mat->cols + prob_col].probability * gm->score_map[index];
        }
        expected_value += score;
        if (expected_value > max_value){
            max_value = expected_value;
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
    for( sc_index = 0; sc_index < nbr_scorecards; sc_index++){
        int scorecard = scorecards[sc_index];
        for( roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
            for( us_index = 0; us_index < MAX_UPPER_SCORE; us_index++){
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                gm->score_map[index] = scorecard_max_expected_value(gm, scorecard, us_index, roll_index, reroll_index);
            }
        }
    }
}

float roll_max_expected_value(struct game_map* gm, int sc_index, int us_index, int roll_index, int subturn)
{
    int prob_row;
    int prob_col;
    float max_value = 0;
    struct roll* rolls = gm->prob_mat->rolls;
    for(prob_row =0; prob_row<gm->prob_mat->rows; prob_row++){
        if(rolls[prob_row*gm->prob_mat->cols + roll_index].probability <= 0){
            continue;
        }
        float expected_value = 0;
        for(prob_col =0; prob_col<gm->prob_mat->cols; prob_col++){
            int index = get_index(sc_index, prob_col, subturn + 1, us_index);
            expected_value += rolls[prob_row*gm->prob_mat->cols + prob_col].probability * gm->score_map[index];
        }
        if(expected_value > max_value){
            max_value = expected_value;
        }
    }
    return max_value;
}

void fill_roll_subturn(struct game_map* gm, int turn, int reroll_index, int* scorecards, unsigned int nbr_scorecards)
{
    int sc_index;
    int roll_index;
    int us_index;
    for( sc_index = 0; sc_index < nbr_scorecards; sc_index++){
        int scorecard = scorecards[sc_index];
        for( roll_index = 0; roll_index < gm->prob_mat->cols; roll_index++){
            for( us_index = 0; us_index < MAX_UPPER_SCORE; us_index++){
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                gm->score_map[index] = roll_max_expected_value(gm, scorecard, us_index, roll_index, reroll_index);
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
    gm->category_score_map = calloc(NBR_CATEGORIES * gm->prob_mat->cols, sizeof(*(gm->category_score_map)));
    precalculate_scores(gm);
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
    yahtzee_game_map_destroy(gm);
}
