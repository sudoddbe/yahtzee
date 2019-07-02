#ifndef YAHTZEE_SCORECARD_H_
#define YAHTZEE_SCORECARD_H_

#include <stdio.h>   
#include <string.h>   
#include <stdlib.h>   
#include <stdbool.h>   
#include <assert.h>   
#include <math.h>   
#include "yahtzee_constants.h"   

int score_category(int category_int, unsigned char* rolls);
void find_forward_scorecard(int current_scorecard, int* dest, unsigned int *nbr_forward_scorecards);
void find_scorecards_for_turn(int turn, int* dest, unsigned int* nbr_scorecards);
#endif /* YAHTZEE_SCORECARD_H_ */
