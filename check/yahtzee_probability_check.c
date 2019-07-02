#include <check.h>
#include "yahtzee_probability_matrix.h"

START_TEST (test_probability_sum)
{
    struct prob_mat* matrix = prob_mat_create();
    unsigned int nbr_rows = matrix->rows;
    unsigned int nbr_cols = matrix->cols;
    int i;
    int j;
    for (i =0; i<nbr_rows;i++){
        double p_sum = 0.0;
        for (j=0; j<nbr_cols;j++){
            double probability = matrix->rolls[i*nbr_cols + j].probability;
            fail_unless(probability >= 0.0, "Probability is negative");
            fail_unless(probability <= 1.0, "Probability is larger than 1");
            p_sum += probability;
        }
        fail_unless(p_sum > 0.9999 && p_sum < 1.0001, "Row sum is not 1, is %2.6f. row %u", p_sum, i);
    }
    struct roll test_rolls[5] = {0};
    unsigned char input_sets[5][6] = {{}, {4, 3, 2, 1}, {3, 2, 1}, {3, 2, 1}, {1, 1, 1}};
    unsigned char input_set_sizes[5] = {0, 4, 3, 3, 3};
    unsigned char output_sets[5][6] = {{6,6,6,6,6}, {5, 4, 3, 2, 1}, {5,4,3,2,1}, {6,6,3,2,1}, {5, 4, 3, 2, 1}};
    double probabilities[5] = {pow(1/6.0, 5), (1/6.0), 2*(1/6.0)*(1/6.0), (1/6.0)*(1/6.0), 0.0};
    struct roll* roll = test_rolls;
    unsigned char* input_set_size = input_set_sizes;
    double* p = probabilities;
    for(i = 0; i<5; i++, roll++, input_set_size++, p++){
        unsigned char* input_set = input_sets[i];
        unsigned char* output_set = output_sets[i];
        roll->input_set_size = *input_set_size;
        roll->output_set_size = 5;
        memcpy(roll->input_set, input_set, sizeof(roll->input_set));
        memcpy(roll->output_set, output_set, sizeof(roll->output_set));
        roll->probability = *p;
    }
    
    int found_matches = 0;
    for(i=0; i<nbr_rows*nbr_cols;i++){
        struct roll* matrix_element = matrix->rolls + i;
        for(j=0; j<5;j++){
            struct roll* test_roll = test_rolls + j;
            if(roll_compare_sets(matrix_element, test_roll)){
                found_matches++;
                fail_unless(abs(matrix_element->probability - test_roll->probability) <= pow(0.1,6));
            }
        }
    }
    fail_unless(found_matches == 5);
    prob_mat_destroy(matrix);
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_probability_sum);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
