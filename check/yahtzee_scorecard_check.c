#include <check.h>
#include "yahtzee_scorecard.h"

int test_scoring(unsigned char* roll, int* expected_scores)
{
    int failed = 0;
    int i = 0;
    for( i= 0; i<NBR_CATEGORIES; i++){
        int score = score_category(i,roll);
        if( score != expected_scores[i]){
            failed++;
        }
    }
    if(failed > 0){
        printf("Failure, roll = ");
        for( i= 0; i<NBR_DICES; i++){
            printf("\t %i", roll[i]);
        }
        printf("\n");
        for( i= 0; i<NBR_CATEGORIES; i++){
            int score = score_category(i,roll);
            printf("%i, %i, %i \n", i, score, expected_scores[i]);
        }
    }
    return failed;
}

START_TEST (test_all_scoring)
{
    int expected_scores[NBR_CATEGORIES] = {0};
    unsigned char roll_0[NBR_DICES] = {1,1,1,1,1};
    int failed = 0;
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 5;
    expected_scores[6] = 2;
    expected_scores[7] = 3;
    expected_scores[8] = 4;
    expected_scores[13] = 5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_0, expected_scores);
    fail_unless(failed == 0, "Test 1 : Wrong score in %i categories", failed);

    unsigned char roll_1[NBR_DICES] = {2,2,2,2,2};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[1] = 2*5;
    expected_scores[6] = 2*2;
    expected_scores[7] = 2*3;
    expected_scores[8] = 2*4;
    expected_scores[13] = 2*5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_1, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_2[NBR_DICES] = {3,3,3,3,3};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[2] = 3*5;
    expected_scores[6] = 3*2;
    expected_scores[7] = 3*3;
    expected_scores[8] = 3*4;
    expected_scores[13] = 3*5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_2, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_3[NBR_DICES] = {4,4,4,4,4};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[3] = 4*5;
    expected_scores[6] = 4*2;
    expected_scores[7] = 4*3;
    expected_scores[8] = 4*4;
    expected_scores[13] = 4*5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_3, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_4[NBR_DICES] = {5,5,5,5,5};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[4] = 5*5;
    expected_scores[6] = 5*2;
    expected_scores[7] = 5*3;
    expected_scores[8] = 5*4;
    expected_scores[13] = 5*5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_4, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_5[NBR_DICES] = {6,6,6,6,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[5] = 6*5;
    expected_scores[6] = 6*2;
    expected_scores[7] = 6*3;
    expected_scores[8] = 6*4;
    expected_scores[13] = 6*5;
    expected_scores[14] = 50;
    failed = test_scoring(roll_5, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_6[NBR_DICES] = {1,2,5,5,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 1;
    expected_scores[1] = 2;
    expected_scores[4] = 2*5;
    expected_scores[5] = 6;
    expected_scores[6] = 2*5;
    expected_scores[13] = 1 + 2 + 5 + 5 + 6;
    failed = test_scoring(roll_6, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_7[NBR_DICES] = {1,5,5,5,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 1;
    expected_scores[4] = 3*5;
    expected_scores[5] = 6;
    expected_scores[6] = 2*5;
    expected_scores[7] = 3*5;
    expected_scores[13] = 1 + 5 + 5 + 5 + 6;
    failed = test_scoring(roll_7, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_8[NBR_DICES] = {1,5,5,5,5};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 1;
    expected_scores[4] = 4*5;
    expected_scores[6] = 2*5;
    expected_scores[7] = 3*5;
    expected_scores[8] = 4*5;
    expected_scores[13] = 1 + 5 + 5 + 5 + 5;
    failed = test_scoring(roll_8, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_9[NBR_DICES] = {5,5,5,6,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[4] = 3*5;
    expected_scores[5] = 2*6;
    expected_scores[6] = 2*6;
    expected_scores[7] = 3*5;
    expected_scores[9] = 2*5 + 2*6;
    expected_scores[10] = 5 + 5 + 5 + 6 + 6;
    expected_scores[13] = 5 + 5 + 5 + 6 + 6;
    failed = test_scoring(roll_9, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_10[NBR_DICES] = {1,5,5,6,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 1 ;
    expected_scores[4] = 2*5;
    expected_scores[5] = 2*6;
    expected_scores[6] = 2*6;
    expected_scores[9] = 2*5 + 2*6;
    expected_scores[13] = 1 + 5 + 5 + 6 + 6;
    failed = test_scoring(roll_10, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_11[NBR_DICES] = {1,2,3,4,5};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[0] = 1;
    expected_scores[1] = 2;
    expected_scores[2] = 3;
    expected_scores[3] = 4;
    expected_scores[4] = 5;
    expected_scores[11] = 1 + 2 +3 +4 +5;
    expected_scores[13] = 1 + 2 +3 +4 +5;
    failed = test_scoring(roll_11, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);

    unsigned char roll_12[NBR_DICES] = {2,3,4,5,6};
    memset(expected_scores, 0, sizeof(expected_scores));
    expected_scores[1] = 2;
    expected_scores[2] = 3;
    expected_scores[3] = 4;
    expected_scores[4] = 5;
    expected_scores[5] = 6;
    expected_scores[12] = 2 +3 +4 +5 +6;
    expected_scores[13] = 2 +3 +4 +5 +6;
    failed = test_scoring(roll_12, expected_scores);
    fail_unless(failed == 0, "Wrong score in %i categories", failed);
}
END_TEST;


START_TEST (test_scorecards_for_turn)
{
    int scorecards[NBR_SCORECARDS] = {0};
    int nbr_scorecards = 0;
    find_scorecards_for_turn(0, scorecards, &nbr_scorecards);
    fail_unless(nbr_scorecards == 1);
    fail_unless(scorecards[0] == 0);

    nbr_scorecards = 0;
    find_scorecards_for_turn(1, scorecards, &nbr_scorecards);
    fail_unless(nbr_scorecards == NBR_CATEGORIES);
    int i;
    int j;
    for( i =0; i<NBR_CATEGORIES; i++){
        int tmp_category = 1 << i;
        bool found = false;
        for(j=0; j<nbr_scorecards; j++){
            if(scorecards[j] == tmp_category){
                found = true;
                break;
            }
        }
        fail_unless(found);
    }

    nbr_scorecards = 0;
    find_scorecards_for_turn(2, scorecards, &nbr_scorecards);
    fail_unless(nbr_scorecards == (NBR_CATEGORIES)*(NBR_CATEGORIES-1)/2);
    int k;
    for( i =0; i<NBR_CATEGORIES; i++){
        for( k =0; k<i; k++){
            int tmp_category = (1 << i) + (1<<k);
            bool found = false;
            for(j=0; j<nbr_scorecards; j++){
                if(scorecards[j] == tmp_category){
                    found = true;
                    break;
                }
            }
            fail_unless(found);
        }
    }


    nbr_scorecards = 0;
    find_scorecards_for_turn(NBR_CATEGORIES, scorecards, &nbr_scorecards);
    fail_unless(nbr_scorecards == 1);
    fail_unless(scorecards[0] == (1<<NBR_CATEGORIES) -1);

    nbr_scorecards = 0;
    find_scorecards_for_turn(NBR_CATEGORIES - 1, scorecards, &nbr_scorecards);
    fail_unless(nbr_scorecards == NBR_CATEGORIES);
    for( i =0; i<NBR_CATEGORIES; i++){
        int tmp_category = (1 << NBR_CATEGORIES) -1 - (1 << i);
        bool found = false;
        for(j=0; j<nbr_scorecards; j++){
            if(scorecards[j] == tmp_category){
                found = true;
                break;
            }
        }
        fail_unless(found);
    }

    //It should run for all turns without crashing...
    for( i =0; i<NBR_CATEGORIES; i++){
        find_scorecards_for_turn(i, scorecards, &nbr_scorecards);
        fail_unless(nbr_scorecards < NBR_SCORECARDS);
    }
}
END_TEST;

START_TEST (test_forward_scorecards)
{
    int scorecards[NBR_SCORECARDS] = {0};
    int nbr_scorecards = 0;
    find_forward_scorecard(0, scorecards, &nbr_scorecards);

    fail_unless(nbr_scorecards == NBR_CATEGORIES);
    int i;
    int j;
    for( i =0; i<NBR_CATEGORIES; i++){
        int tmp_scorecard = 1 << i;
        bool found = false;
        for(j=0; j<nbr_scorecards; j++){
            if(scorecards[j] == tmp_scorecard){
                found = true;
                break;
            }
        }
        fail_unless(found);
    }

    int k;
    int current_scorecard = 0;
    for( k = 0; k<NBR_CATEGORIES; k++){
        nbr_scorecards = 0;
        find_forward_scorecard(current_scorecard, scorecards, &nbr_scorecards);
        fail_unless(nbr_scorecards == NBR_CATEGORIES - k);
        for( i =k; i<NBR_CATEGORIES; i++){
            int tmp_scorecard = current_scorecard + (1 << i) ;
            bool found = false;
            for(j=0; j<nbr_scorecards; j++){
                if(scorecards[j] == tmp_scorecard){
                    found = true;
                    break;
                }
            }
            fail_unless(found);
        }
        current_scorecard += (1<<k);
    }
    for( i = 0; i < NBR_SCORECARDS; i++){
        find_forward_scorecard(i, scorecards, &nbr_scorecards);
        fail_unless(nbr_scorecards <= NBR_CATEGORIES);
    }

}
END_TEST;

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;
    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, test_all_scoring);
    tcase_add_test(tc1_1, test_scorecards_for_turn);
    tcase_add_test(tc1_1, test_forward_scorecards);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    return nf == 0 ? 0 : 1;
}
