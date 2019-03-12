import numpy as np

#An attempt using dtype to save some memory in gamestate representation
#Currently the smallest basic type is bytes (i.e booleans are NOT bit-represented
#Using a bit representation on bools would save three quarters of the space on the scorecard
d_scorecard = { 
    'names' : (
            'extra_score_set'
            'ones',
            'twos',
            'threes',
            'fours',
            'fives',
            'sixes',
            'pair',
            'three_of_a_kind',
            'four_of_a_kind',
            'full_house',
            'small_straight',
            'large_straight',
            'yahtzee'),
    'formats' : (
            '?',
            '?', 
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?',
            '?'),
    'itemsize' : 13
    }
scorecard_dtype = np.dtype(d_scorecard)
d = {'names' : (
            'nbr_roll',
            'current_roll', 
            'scorecard',
            'scorecard_int'),
    'formats' : (
            'B',
            '5B',
            scorecard_dtype, 
            'u2'),
    'offsets' : (
            0,
            1,
            6,
            6),
    'itemsize' : 19 
    }
game_state_dtype = np.dtype(d)
#Current size of game stat is 19 bytes. Might select 24 or 32 for performance reasons. 
#Estimation of size of all gamestates is:
#(combinations of 5 dices) * (nbr_rerolls) * (number of scorecards) * (size of single gamestate)
#Currently this is 252*3* (2**15 -1) * 19 ~ 470 MB. Table linking gamestate with optimal selection would be smaller than twice this... 