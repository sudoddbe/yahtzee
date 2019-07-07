#ifndef YAHTZEE_GAME_MAP_H_
#define YAHTZEE_GAME_MAP_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "yahtzee_constants.h"

struct game_map {
    struct prob_mat* prob_mat;
    float* score_map;
};

#endif /* YAHTZEE_GAME_MAP_H_ */