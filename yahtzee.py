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
            score_ones(),
            score_twos(),
            score_threes(),
            score_fours(),
            score_fives(),
            score_sixes(),
            score_pair(),
            score_three_of_a_kind(),
            score_four_of_a_kind(),
            score_full_house(),
            score_small_straight(),
            score_large_straight(),
            score_yahtzee()
        ]
        self.categories = [Category(label, scoring_function) for label, scoring_function in zip(labels, scoring_functions)]


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

