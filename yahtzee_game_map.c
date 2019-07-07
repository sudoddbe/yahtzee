#include "yahtzee_game_map.h"
#include "yahtzee_probability_matrix.h"
#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

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

#define NBR_ROLLS (252)
//Shameless temporary define
int get_index(int scorecard_index, int roll_index, int reroll_index, int upper_score_index)
{
    int index = 0;
    index +=  scorecard_index * (NBR_ROLLS*NBR_REROLLS*MAX_UPPER_SCORE);
    index +=  roll_index * (NBR_REROLLS*MAX_UPPER_SCORE);
    index +=  reroll_index * (MAX_UPPER_SCORE);
    index +=  upper_score_index;
    return index;
}


int find_first_bit_set(int a)
{
    int mask = 1;
    int counter = 0;
    while(a >0){
        if( (a & mask) == 1){
            return counter;
        }
        a = a >> 1;
        counter++;
    }
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

//Last subturn is when we have filled all but one category on the scorecard
//We have only one option to fill, and will get a score based on that.
void fill_end_turn(struct game_map* gm)
{
    int sc_index;
    int roll_index;
    int us_index;
    int reroll_index = NBR_REROLLS -1;

    int scorecards[NBR_SCORECARDS] = {0};
    int nbr_scorecards = 0;
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
                if( us_index + score >= MAX_UPPER_SCORE -1){
                    score += 50;
                }
                int index = get_index(scorecard, roll_index, reroll_index, us_index);
                gm->score_map[index] = score;
            }
        }
    }
}

int main(void)
{
    struct game_map* gm = yahtzee_game_map_create();
    printf("%u \n", gm->prob_mat->cols);
    printf("Size of map : %ld \n", NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * MAX_UPPER_SCORE);
    yahtzee_game_map_destroy(gm);
}
