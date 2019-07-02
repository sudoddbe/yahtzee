#ifndef YAHTZEE_PROBABILITY_MATRIX_H_
#define YAHTZEE_PROBABILITY_MATRIX_H_

#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>   
#include <stdbool.h>   
#include <assert.h>   
#include <math.h>   
#include "yahtzee_constants.h"

struct roll {
    unsigned char input_set_size;
    unsigned char output_set_size;
    unsigned char input_set[NBR_DICES];
    unsigned char output_set[NBR_DICES];
    double probability; 
};

struct prob_mat {
    unsigned int rows;
    unsigned int cols;
    struct roll* rolls;
};

unsigned int n_choose_k(unsigned int n, unsigned int k);
bool roll_compare_sets(struct roll* a, struct roll* b);
void fill_roll_probability(struct roll* roll);
int fill_input_sets(struct roll* rolls, int row, int nbr_cols, int roll_pos, int current_max);
int fill_output_sets(struct roll* rolls, int col, int roll_pos, int current_max);
struct prob_mat* prob_mat_create();
void prob_mat_destroy(struct prob_mat* matrix);

#endif /* YAHTZEE_PROBABILITY_MATRIX_H_ */
