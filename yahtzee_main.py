import numpy as np
import math
from output.yahtzee_constants import *

def get_index(scorecard, roll_index, subturn, upper_score):
    index = 0
    index += subturn * (MAX_UPPER_SCORE*NBR_SCORECARDS*NBR_ROLLS)
    index += upper_score * (NBR_SCORECARDS*NBR_ROLLS)
    index +=  scorecard * (NBR_ROLLS)
    index +=  roll_index
    return index

def find_best_move_roll(subturn, upper_score, scorecard, roll_index, score_map, probability_matrix, input_rolls, output_rolls):
    best_move = None
    max_expected_value = 0
    for i in range(len(input_rolls)):
        if probability_matrix[i, roll_index] <= 0:
            continue

        expected_value = 0
        for j in range(len(output_rolls)):
            index = get_index(scorecard, j, subturn + 1,upper_score)
            expected_value += probability_matrix[i, j] * score_map[index]

        if(expected_value > max_expected_value):
            max_expected_value = expected_value
            best_move = input_rolls[i]

    return (best_move, max_expected_value)


def find_best_move_choose(subturn, upper_score, scorecard, roll_index, score_map, probability_matrix, forward_scorecards, category_score):
    best_move = None
    max_expected_value = 0
    forward_scorecards = forward_scorecards[forward_scorecards > 0]
    for fwd_scorecard in forward_scorecards:
        print fwd_scorecard
        category_index = int(round(math.log(fwd_scorecard - scorecard, 2)))
        score = category_score[category_index, roll_index]
        added_upper_score = (category_index < 6) * score
        tmp_upper_score = upper_score + added_upper_score
        if(tmp_upper_score > MAX_UPPER_SCORE -1):
            tmp_upper_score = MAX_UPPER_SCORE - 1

        expected_value = 0
        for j in range(probability_matrix.shape[1]):
            index = get_index(fwd_scorecard, j, 0, tmp_upper_score)
            expected_value += probability_matrix[-1, j] * score_map[index]

        expected_value += score
        if(expected_value > max_expected_value):
            max_expected_value = expected_value
            best_move = category_index
            best_added_upper_score = added_upper_score
    return (best_move, max_expected_value, best_added_upper_score)

def load_all_matrices():
    f = open("output/input_rolls")
    input_rolls = [tuple(map(int, line.split())) for line in f]
    f.close()

    f = open("output/output_rolls")
    output_rolls = [tuple(map(int, line.split())) for line in f]
    f.close()

    forward_scorecards = np.fromfile("output/yahtzee_forward_scorecards", dtype="int32")
    forward_scorecards = np.reshape(forward_scorecards, (NBR_SCORECARDS, NBR_CATEGORIES))

    probability_matrix = np.fromfile("output/yahtzee_probability_matrix", dtype="float32")
    probability_matrix = np.reshape(probability_matrix, (len(input_rolls), len(output_rolls)))

    category_scores = np.fromfile("output/yahtzee_category_scores", dtype="int32")
    category_scores = np.reshape(category_scores, (NBR_CATEGORIES, len(output_rolls)))
    score_map = np.memmap("output/yahtzee_table", dtype='float32', mode='r')
    return (input_rolls, output_rolls, forward_scorecards, probability_matrix, category_scores, score_map)

if __name__ == "__main__":
    category_names= ["ones", "twos", "threes", "fours", "fives", "sixes", "two_of_a_kind", "three_of_a_kind", "four_of_a_kind", "two_pair", "full_house", "small_straight", "large_straight", "chance", "yahtzee"]
    scorecard = 0
    upper_score = 0
    subturn = 0
    turn = 0
    score = 0

    (input_rolls, output_rolls, forward_scorecards, probability_matrix, category_score, score_map) = load_all_matrices()
    for turn in range(NBR_CATEGORIES):
        for subturn in range(NBR_REROLLS):
            while(True):
                roll = raw_input("Please write the current roll : ")
                roll = [int(c) for c in roll.split()]
                if(len(roll) == 5):
                    break

            roll = np.sort(roll)[::-1]
            roll = tuple(roll)
            roll_index = output_rolls.index(roll)
            print roll_index
            if(subturn < NBR_REROLLS -1):
                best_move, expected_value = find_best_move_roll(subturn, upper_score, scorecard, roll_index, score_map, probability_matrix, input_rolls, output_rolls)
                print " Best move is to save the these dices: "
                print best_move
                print "Expected value is ",(score + expected_value)
            else:
                best_move, expected_value, added_upper_score = find_best_move_choose(subturn, upper_score, scorecard, roll_index, score_map, probability_matrix, forward_scorecards[scorecard], category_score)
                print " Best move is to fill category: "
                print best_move
                print category_names[best_move]
                print "Expected value is ",(score+expected_value)
                print "old scorecard",np.binary_repr(scorecard, NBR_CATEGORIES)
                scorecard += 2**best_move
                print "new scorecard",np.binary_repr(scorecard, NBR_CATEGORIES)
                upper_score += added_upper_score
                upper_score = min(upper_score, MAX_UPPER_SCORE - 1)
                print "upper score is",upper_score


