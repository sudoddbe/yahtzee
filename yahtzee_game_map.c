#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>   
#include <stdbool.h>   
#include <assert.h>   
#include <math.h>   

#define NBR_DICE (5)
#if 1
#define DEBUG(x) (x)
#else
#define DEBUG(x)
#endif
struct roll {
    unsigned char input_set_size;
    unsigned char output_set_size;
    unsigned char input_set[NBR_DICE];
    unsigned char output_set[NBR_DICE];
    double probability; 
};

struct prob_mat {
    unsigned int rows;
    unsigned int cols;
    struct roll* rolls;
};

unsigned int n_choose_k(unsigned int n, unsigned int k)
{
    assert(n >= k);
    unsigned int result = 1.0;
    unsigned int denominator_1 = k;
    unsigned int denominator_2 = n - k;
    if (denominator_1 < denominator_2) {
        unsigned int tmp = denominator_1;
        denominator_1 = denominator_2;
        denominator_2 = tmp;
    }
    int i;
    for( i = n; i > denominator_1; i--){
        result *=i;
    }
    unsigned int denominator = 1;
    for( i = denominator_2; i >=1; i--){
        denominator *= i;
    }
    return result/denominator;
}

void fill_roll_probability(struct roll* roll){
    //Set diff set is defined by the rolls that is in the output set but not in the input set.
    //This is the roll we would need on the undecided dice to exactly get the output set
    unsigned char diff_set[NBR_DICE] = {0};
    memcpy(diff_set, roll->output_set, sizeof(roll->output_set));
    int diff_set_size = roll->output_set_size;
    int i;
    int j;
    for( i=0; i<roll->input_set_size; i++){
        bool is_subset = false;
        for( j=0; j<diff_set_size; j++){
            if(diff_set[j] == roll->input_set[i]){
                diff_set[j] = 0;
                is_subset = true;
                break;
            }
        }
        if (!is_subset){
            //As soon as we find a roll in the input set that is not in the output set we know the porbability is zero... 
            //Since we wont reroll any dice in the input set it is impossible to reach the output set if the input set is not a perfect subset of the output set.
            roll->probability = 0;
            return;
        }
    }

    //Count the number of repititions for each face of the dice.
    unsigned char repitions[6] = {0};
    for (i=0; i<6; i++){
        for (j=0; j<diff_set_size; j++){
            repitions[i] += (i+1) == diff_set[j];
        }
    }
    
    //Count the number of non-zero dice in the diff set (i.e the number of dice we need to roll correctly)
    int nbr_rolls = 0;
    for (j=0; j<diff_set_size; j++){
        nbr_rolls += diff_set[j] > 0;
    }
    //Calculate probability as (1.0/6.0)**nbr_rolls * nbr_rolls! / (prod(repitions!))
    double probability = pow(1.0/6.0, nbr_rolls);
    //Sloppy factorial
    for (i=1; i<=nbr_rolls; i++){
        probability *=i;
    }
    //Sloppy factorial for all repitions
    for (i=0; i<6; i++){
        for (j=1; j<=repitions[i]; j++){
            probability /=j;
        }
    }
    roll->probability = probability;
}

int fill_input_sets(struct roll* rolls, int row, int nbr_cols, int roll_pos, int current_max)
{
    if (roll_pos >= NBR_DICE){
        return row;
    }
    int i;
    for( i=1; i<=current_max;i++){
        rolls[row*nbr_cols].input_set[roll_pos] = i;
        rolls[row*nbr_cols].input_set_size = roll_pos+1;
        row++;
    }
    for( i=1; i<=current_max;i++){
        int new_row = fill_input_sets(rolls, row, nbr_cols, roll_pos+1, i);
        for (row; row<new_row; row++){
            rolls[row*nbr_cols].input_set[roll_pos] = i;
        }
    }
    return row;
}

int fill_output_sets(struct roll* rolls, int col, int roll_pos, int current_max)
{
    if (roll_pos >= NBR_DICE){
        rolls[col].output_set_size = NBR_DICE;
        return col+1;
    }
    int i;
    for( i=1; i<=current_max;i++){
        int new_col = fill_output_sets(rolls, col, roll_pos+1, i);
        for (col; col<new_col; col++){
            rolls[col].output_set[roll_pos] = i;
        }
    }
    return col;
}

struct prob_mat* prob_mat_create()
{
    struct prob_mat* matrix = malloc(sizeof(struct prob_mat));
    unsigned int nbr_cols = n_choose_k(5 + 6 -1, 5);
    unsigned int nbr_rows = 0;
    int i;
    int j;
    for( i = NBR_DICE; i >= 0; i--){
        nbr_rows += n_choose_k(i + 6 - 1, i);
    }
    matrix->rows = nbr_rows;
    matrix->cols = nbr_cols;
    matrix->rolls = malloc(nbr_rows*nbr_cols*sizeof(struct roll));
    int row = 0;
    int col = 0;
    int input_size = NBR_DICE;
    fill_input_sets(matrix->rolls, 0, nbr_cols, 0, 6);
    fill_output_sets(matrix->rolls, 0, 0, 6);
    for (i =0; i<nbr_rows;i++){
        for (j=0; j<nbr_cols;j++){
            struct roll *roll = matrix->rolls + i*nbr_cols + j;
            roll->input_set_size = matrix->rolls[i*nbr_cols].input_set_size;
            roll->output_set_size = matrix->rolls[j].output_set_size;
            memcpy(roll->input_set, matrix->rolls[i*nbr_cols].input_set, sizeof(roll->input_set));
            memcpy(roll->output_set, matrix->rolls[j].output_set, sizeof(roll->output_set));
        }
    }

    for (i =0; i<nbr_rows;i++){
        for(j=0; j<nbr_cols;j++){
            struct roll* r = matrix->rolls + i*nbr_cols + j;
            fill_roll_probability(r);
        }
    }
    for (i =0; i<nbr_rows;i++){
        int rolli;
        DEBUG(printf("%i :\t", i));
        for(rolli=0; rolli < matrix->rolls[i*nbr_cols + 10].input_set_size; rolli++){
            DEBUG(printf("%d \t", matrix->rolls[i*nbr_cols + 10].input_set[rolli]));
        }
        DEBUG(printf("\n"));
    }
    DEBUG(printf("\n"));
    DEBUG(printf("\n"));
    DEBUG(printf("\n"));
    for (i =0; i<nbr_cols;i++){
        int rolli;
        for(rolli=0; rolli < NBR_DICE; rolli++){
            DEBUG(printf("%d \t", matrix->rolls[i + 10*nbr_cols].output_set[rolli]));
        }
        DEBUG(printf("\n"));
    }
    return matrix;
}

void prob_mat_destory(struct prob_mat* matrix)
{
    free(matrix->rolls);
    free(matrix);
}


int main()
{    
    printf("%u \n", n_choose_k(3,1));
    printf("%u \n", n_choose_k(3,2));
    printf("%u \n", n_choose_k(3,3));
    printf("%u \n", n_choose_k(10,5));
    struct prob_mat* matrix = prob_mat_create();
    printf("%u \n", matrix->rows);
    printf("%u \n", matrix->cols);
    prob_mat_destory(matrix);
    //Create probability matrix
    //Create index to input set matching
    //Create index to output set matching
    //Create score matrix with output_sets*category elements.
    //Pre-calculate the score matrix
    //Fill probability matrix
    //Create game map
    //Fill last turn
    //backtrack till first turn
    //TODO : Write unit test
}
