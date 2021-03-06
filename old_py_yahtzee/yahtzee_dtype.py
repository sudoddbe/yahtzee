import numpy as np

NBR_SUBTURNS = 3
MAX_UPPER_SCORE = 64 
NBR_CATEGORIES = 15
NBR_TURNS = 15


#An attempt using dtype to save some memory in gamestate representation
#Currently the smallest basic type is bytes (i.e booleans are NOT bit-represented
#Using a bit representation on bools would save three quarters of the space on the scorecard
d_scorecard = { 
    'names' : (
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
            '?'),
    'itemsize' : 13
    }
scorecard_dtype = np.dtype(d_scorecard)
d = {'names' : (
            'nbr_roll',
            'roll_index', 
            'upper_score',
            'scorecard',
            'expected_value',
            ),
    'formats' : (
            'B',
            'B',
            'B',
            'u2', 
            'f2',
            ),
    'offsets' : (
            0,
            1,
            2,
            3,
            5),
    'itemsize' : 7 
    }
game_state_dtype = np.dtype(d)
d = {'names' : (
            'score',
            ),
    'formats' : (
            'f2',
            ),
    'offsets' : (
            0,
            ),
    'itemsize' : 2 
    }
yahtzee_dtype = np.dtype(d)
#Current size of game stat is 19 bytes. Might select 24 or 32 for performance reasons. 
#Estimation of size of all gamestates is:
#(combinations of 5 dices) * (nbr_rerolls) * (number of scorecards) * (size of single gamestate)
#Currently this is 252*3* (2**13) * 19 ~ 470 MB. Table linking gamestate with optimal selection would be smaller than twice this... 
