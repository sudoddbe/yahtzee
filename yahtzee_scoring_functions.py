from yahtzee_dtype import NBR_CATEGORIES
from yahtzee_probability import dice_probability_dict
import numpy as np
import itertools

def score_face(rolls, face):
    bins = np.bincount(rolls)
    if len(bins) <= face:
        return 0
    else:
        return bins[face] * face
def score_ones(rolls):
    return score_face(rolls, face=1)

def score_twos(rolls):
    return score_face(rolls, face=2)

def score_threes(rolls):
    return score_face(rolls, face=3)

def score_fours(rolls):
    return score_face(rolls, face=4)

def score_fives(rolls):
    return score_face(rolls, face=5)

def score_sixes(rolls):
    return score_face(rolls, face=6)

def score_n_of_a_kind(rolls, n):
    rolls_in_a_row = 0
    bins = np.bincount(rolls)
    last_roll = len(bins)-1
    for bin in bins[::-1]:
        if bin >= n:
            return last_roll*n
        last_roll -= 1
    return 0

def score_two_of_a_kind(rolls):
    return score_n_of_a_kind(rolls, n = 2)

def score_three_of_a_kind(rolls):
    return score_n_of_a_kind(rolls, n = 3)

def score_four_of_a_kind(rolls):
    return score_n_of_a_kind(rolls, n = 4)

def score_full_house(rolls):
    bins = np.bincount(rolls)
    if np.sum(bins > 0) == 2:
        if (np.sum(bins == 2) == 1 and np.sum(bins ==3) == 1):
            return np.inner(bins, np.arange(len(bins)))
    elif np.sum(bins > 0) == 1:
        assert(np.sum(bins == 5) == 1)
        return np.inner(bins, np.arange(len(bins)))
    return 0

def score_small_straight(rolls):
    if rolls == (1,2,3,4,5):
        return 1 + 2+ 3+ 4+ 5
    return 0

def score_large_straight(rolls):
    if rolls == (2,3,4,5,6):
        return  2+ 3+ 4+ 5 +6
    return 0

def score_yahtzee(rolls):
    bins = np.bincount(rolls)
    if np.max(bins) == 5:
        return 50
    return 0

keys = 2**np.arange(NBR_CATEGORIES)
scoring_functions = [
    score_ones,
    score_twos,
    score_threes,
    score_fours,
    score_fives,
    score_sixes,
    score_two_of_a_kind,
    score_three_of_a_kind,
    score_four_of_a_kind,
    score_full_house,
    score_small_straight,
    score_large_straight,
    score_yahtzee,
]
scoring_dict = dict(zip(keys, scoring_functions))
def score_category(category_int, rolls):
    if category_int < 2**5 +1:
        score = scoring_dict[category_int](rolls)
        return (score, score)
    else:
        score = scoring_dict[category_int](rolls)
        return (score, 0)
    
