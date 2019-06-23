#include "yahtzee_game_map.h"   
#include "yahtzee_probability_matrix.h"
#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

int main(void)
{
    struct prob_mat* matrix = prob_mat_create(); 
    printf("%u \n", matrix->rows);
    prob_mat_destory(matrix);
}
