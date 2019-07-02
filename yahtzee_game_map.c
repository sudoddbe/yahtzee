#include "yahtzee_game_map.h"
#include "yahtzee_probability_matrix.h"
#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

struct game_map* yahtzee_game_map_create()
{
    struct game_map* gm= calloc(1, sizeof(*gm));
    gm->prob_mat = prob_mat_create();
    gm->score_map = calloc(NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * EXTRA_SCORE_THRESH, sizeof(*(gm->score_map)));
    return gm;
}

void yahtzee_game_map_destroy(struct game_map* gm)
{
    prob_mat_destroy(gm->prob_mat);
    free(gm->score_map);
}

int main(void)
{
    struct game_map* gm = yahtzee_game_map_create();
    printf("%u \n", gm->prob_mat->cols);
    printf("Size of map : %ld \n", NBR_SCORECARDS * gm->prob_mat->cols * NBR_REROLLS * EXTRA_SCORE_THRESH);
    yahtzee_game_map_destroy(gm);
}
