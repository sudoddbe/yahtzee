from yahtzee_probability import dice_probability_dict 
import numpy as np
import itertools

Class Scorecard:
    def __init__(self):
        self.points = 0
        labels =[
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
            'yahtzee'
        ]
        scoring_functions = [
            self.score_face(face = 1),
            self.score_face(face = 2),
            self.score_face(face = 3),
            self.score_face(face = 4),
            self.score_face(face = 5),
            self.score_face(face = 6),
            self.score_n_of_a_kind(n = 2),
            self.score_n_of_a_kind(n = 3),
            self.score_n_of_a_kind((n = 4)),
            self.score_full_house(),
            self.score_small_straight(),
            self.score_large_straight(),
            self.score_yahtzee()
        ]
        self.categories = [Category(label, scoring_function) for label, scoring_function in zip(labels, scoring_functions)]
    def score_face(self, rolls, face):
        score = 0
        bins = np.binscount(rolls)
        if len(bins) < face:
            return 0
        else:
            return bins[face] * face

    def score_n_of_a_kind(self, rolls, n):
        last_roll = 7
        rolls_in_a_row = 0
        bins = np.binscount(rolls)
        last_roll = len(bins)
        for bin in bins[::-1]:
            if bin >= n:
                return last_roll*n
            last_roll -= 1
        return 0

    def score_full_house(self, rolls):
        bins = np.bincount(rolls)
        if len(bins > 0) == 2:
            assert(len(bins == 2) == 1 and len(bins ==3) == 1)
            return np.inner(bins, np.arange(len(bins)))
        return 0

    def score_small_straight(self, rolls):
        if rolls == (1,2,3,4,5):
            return 1 + 2+ 3+ 4+ 5
        return 0 

    def score_large_straight(self, rolls):
        if rolls == (2,3,4,5,6):
            return  2+ 3+ 4+ 5 +6
        return 0 

    def score_yahtzee(self, rolls):
        bins = np.binscount(rolls)
        if np.max(bins) == 5:
            return 50
        return 0

class Category():
    def __init__(self, name, scoring_function)
        self.name = name
        self.scoring_function = scoring_function
        self.filled = False
        self.score = 0

    def score(self, *args, **kwargs)
        return self.scoring_function(*args, **kwargs)

    def fill(self, score)
        self.filled = True
        self.score = score

if __name__ == "__main__":
    dice_values = [1,2,3,4,5,6]
    input_sets = [p for n in range(6) for p in itertools.combinations_with_replacement(dice_values, n)]
    output_sets = [p for p in itertools.combinations_with_replacement(dice_values, 5)]
    print len(output_sets)
    print len(input_sets)
    fpd, rpd = dice_probability_dict(input_sets, output_sets)
    print len(fpd)
    print input_sets[11]
    print output_sets[69]
    print fpd[input_sets[11]][output_sets[69]]

