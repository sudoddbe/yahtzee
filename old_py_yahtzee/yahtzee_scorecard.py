from yahtzee_probability import dice_probability_dict
import numpy as np
import itertools

class Scorecard:


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
            self.score_ones,
            self.score_twos,
            self.score_threes,
            self.score_fours,
            self.score_fives,
            self.score_sixes,
            self.score_two_of_a_kind,
            self.score_three_of_a_kind,
            self.score_four_of_a_kind,
            self.score_full_house,
            self.score_small_straight,
            self.score_large_straight,
            self.score_yahtzee,
        ]
        self.categories = [Category(label, scoring_function) for label, scoring_function in zip(labels, scoring_functions)]

    def score_ones(self,rolls):
        return self.score_face(rolls, face=1)

    def score_twos(self,rolls):
        return self.score_face(rolls, face=2)

    def score_threes(self,rolls):
        return self.score_face(rolls, face=3)

    def score_fours(self,rolls):
        return self.score_face(rolls, face=4)

    def score_fives(self,rolls):
        return self.score_face(rolls, face=5)

    def score_sixes(self,rolls):
        return self.score_face(rolls, face=6)

    def score_face(self, rolls, face):
        bins = np.bincount(rolls)
        if len(bins) <= face:
            return 0
        else:
            return bins[face] * face

    def score_two_of_a_kind(self, rolls):
        return self.score_n_of_a_kind(rolls, n = 2)

    def score_three_of_a_kind(self, rolls):
        return self.score_n_of_a_kind(rolls, n = 3)

    def score_four_of_a_kind(self, rolls):
        return self.score_n_of_a_kind(rolls, n = 4)

    def score_n_of_a_kind(self, rolls, n):
        rolls_in_a_row = 0
        bins = np.bincount(rolls)
        last_roll = len(bins)-1
        for bin in bins[::-1]:
            if bin >= n:
                return last_roll*n
            last_roll -= 1
        return 0

    def score_full_house(self, rolls):
        bins = np.bincount(rolls)
        if np.sum(bins > 0) == 2:
            if (np.sum(bins == 2) == 1 and np.sum(bins ==3) == 1):
                return np.inner(bins, np.arange(len(bins)))
        elif np.sum(bins > 0) == 1:
            assert(np.sum(bins == 5) == 1)
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
        bins = np.bincount(rolls)
        if np.max(bins) == 5:
            return 50
        return 0

    def score_all_categories(self, rolls):
        return {cat.name:cat.score_roll_check(rolls) for cat in self.categories}

    def generate_keys_for_turn(self ,turn):
        nbr_categories = len(self.categories)
        if turn > nbr_categories:
            print "invalid turn"
            assert(False)
        def ones_in_binary(num):
            a = np.binary_repr(num, width = nbr_categories)
            a = map(int, list(a))
            return np.sum(a)
        return [i for i in range(2**nbr_categories) if ones_in_binary(i) == turn]

    @classmethod
    def get_scorecard_from_key(cls, key):
        nbr_categories = 13
        a = np.binary_repr(key, width = nbr_categories)
        a = map(int, list(a))
        scorecard = Scorecard()
        for i, category in zip(a, scorecard.categories):
            if i > 0:
                category.fill(0)
        return scorecard

class Category():
    def __init__(self, name, scoring_function):
        self.name = name
        self.scoring_function = scoring_function
        self.filled = False
        self.score = 0

    def score_roll_check(self, rolls):
        if self.filled:
            return -1
        return self.score_roll(rolls)

    def score_roll(self, rolls):
        return self.scoring_function(rolls)

    def fill(self, score):
        self.filled = True
        self.score = score

