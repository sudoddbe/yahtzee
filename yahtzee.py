from yahtzee_probability import dice_probability_dict 
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
            self.score_ones(face = 1),
            self.score_twos(face = 2),
            self.score_threes(face = 3),
            self.score_fours(face = 4),
            self.score_fives(face = 5),
            self.score_sixes(face = 6),
            self.score_pair(n = 2),
            self.score_three_of_a_kind(n = 3),
            self.score_four_of_a_kind((n = 4)),
            self.score_full_house(),
            self.score_small_straight(),
            self.score_large_straight(),
            self.score_yahtzee()
        ]
        self.categories = [Category(label, scoring_function) for label, scoring_function in zip(labels, scoring_functions)]
    def score_face(self, rolls, face):
        score = 0
        for roll in rolls:
            if roll == face:
                score +=face
        return score

    def score_n_of_a_kind(self, rolls, n):
        last_roll = 7
        rolls_in_a_row = 0
        for roll in rolls[::-1]:
            assert last_roll >= roll
            if roll == last_roll:
                #highest n-of-a-kind found (assumes sorted rolls)
                rolls_in_a_row += 1
                if rolls_in_a_row == n:
                    return n*roll
            else:
                last_roll = roll
                rolls_in_a_row = 0
        return 0

    #NOT FINISHED
    def score_full_house(self, rolls):
        last_roll = 7
        rolls_in_a_row = 0
        three_of_a_kind_score = 0
        pair_score = 0
        for roll in rolls[::-1]:
            assert last_roll >= roll
            if roll == last_roll:
                rolls_in_a_row += 1
                if rolls_in_a_row == 3:
                    three_of_a_kind_score 3*roll
            else:
                if rolls_in_a_row == 2:
                    pair_score = 2*rolls
                    if three_of_kind_score
                last_roll = roll
                rolls_in_a_row = 0
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
    input_sets = [p for n in range(7) for p in itertools.combinations_with_replacement(dice_values, n)]
    output_sets = [p for p in itertools.combinations_with_replacement(dice_values, 6)]
    print len(output_sets)
    print len(input_sets)
    fpd, rpd = dice_probability_dict(input_sets, output_sets)
    print len(fpd)
    print input_sets[11]
    print output_sets[69]
    print fpd[input_sets[11]][output_sets[69]]

