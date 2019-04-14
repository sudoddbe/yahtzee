from yahtzee_dtype import yahtzee_dtype, scorecard_dtype, NBR_SUBTURNS, MAX_UPPER_SCORE, NBR_CATEGORIES, NBR_TURNS
from yahtzee_probability import dice_probability_dict
from yahtzee_scoring_functions import score_category, pre_calc_scores
import numpy as np
import multiprocessing
from functools import partial

def generate_keys_for_turn(turn):
    nbr_categories = NBR_CATEGORIES
    if turn > nbr_categories:
        print "invalid turn"
        assert(False)
    def ones_in_binary(num):
        a = np.binary_repr(num, width = nbr_categories)
        a = map(int, list(a))
        return np.sum(a)
    return [i for i in range(2**nbr_categories) if ones_in_binary(i) == turn]

def find_forward_categories(key):
    pow_2 = 2**np.arange(NBR_CATEGORIES)[::-1]
    a = np.binary_repr(key, width = NBR_CATEGORIES)
    a = map(int, list(a))
    keys = []
    for i,e in enumerate(a):
        if e == 0:
            copy = np.array([element for element in a])
            copy[i] = 1
            key = np.dot(pow_2, copy)
            keys += [int(key)] 
    return keys 

#End state is one unfilled category with a given roll and upper score. No choices to be made...
def fill_end_states(last_frame, reverse_probability_dict, forward_probability_dict, scorecard_dict):
    subturn = NBR_SUBTURNS-1
    for  upper_score in range(MAX_UPPER_SCORE):
        for k, roll in enumerate(reverse_probability_dict.keys()):
            for l, scorecard in enumerate(scorecard_dict.keys()):
                score, added_upper_score = score_category(2**NBR_CATEGORIES -1 - scorecard , roll)                
                if upper_score + added_upper_score >= MAX_UPPER_SCORE -1 :
                    score += 50
                last_frame[subturn, upper_score, k, scorecard_dict[scorecard]]["score"] = score

    for subturn in range(NBR_SUBTURNS-1)[::-1]:
        fill_roll_subturn(last_frame, subturn, reverse_probability_dict, forward_probability_dict, scorecard_dict)

def scorecard_max_expected_value(roll, upper_score, scorecards,forward_game_frame, forward_scorecard_dict, tmp_probability, pre_calc_score_dict):
    max_expected_values = [0 for s in scorecards]
    for l, scorecard in enumerate(scorecards):
        max_expected_value = 0
        fwd_cat = find_forward_categories(scorecard)
        for fc in fwd_cat:
            score, added_upper_score = score_category(fc - scorecard, roll, pre_calc_score_dict)
            tmp_upper_score = upper_score + added_upper_score
            tmp_upper_score = min(tmp_upper_score, MAX_UPPER_SCORE-1)
            tmp_fwd_scores = forward_game_frame[0, tmp_upper_score, :, forward_scorecard_dict[fc]]['score']
            expected_value = np.dot(tmp_probability, tmp_fwd_scores)
            expected_value += score
            if expected_value > max_expected_value:
                max_expected_value = expected_value
        max_expected_values[l] = max_expected_value
    return max_expected_values


def fill_last_subturn(game_frame, forward_game_frame, reverse_probability_dict, forward_probability_dict, scorecard_dict, forward_scorecard_dict, pre_calc_score_dict):
    MULTI = 1
    if MULTI:
        pool = multiprocessing.Pool()
    subturn = NBR_SUBTURNS -1
    tmp_probability = np.array(forward_probability_dict[tuple()].values())
    for  upper_score in range(MAX_UPPER_SCORE):
        if MULTI:
            max_expected_values = pool.map(partial(scorecard_max_expected_value, upper_score = upper_score, scorecards = scorecard_dict.keys(), forward_game_frame=forward_game_frame, forward_scorecard_dict=forward_scorecard_dict,tmp_probability=tmp_probability, pre_calc_score_dict = pre_calc_score_dict), reverse_probability_dict.keys())
        for k, roll in enumerate(reverse_probability_dict.keys()):
            for l, scorecard in enumerate(scorecard_dict.keys()):
                if MULTI:
                    game_frame[subturn, upper_score, k, scorecard_dict[scorecard]]["score"] = max_expected_values[k][l]
                else:
                    game_frame[subturn, upper_score, k, scorecard_dict[scorecard]]["score"] = scorecard_max_expected_value(roll, upper_score = upper_score, scorecards = [scorecard], forward_game_frame=forward_game_frame, forward_scorecard_dict=forward_scorecard_dict,tmp_probability=tmp_probability, pre_calc_score_dict=pre_calc_score_dict)[0]

    if MULTI:
        pool.close()
        
def fill_roll_subturn(game_frame, subturn, reverse_probability_dict, forward_probability_dict, scorecard_dict):
    prob_matrix = np.array([forward_probability_dict[input_set].values() for input_set in forward_probability_dict.keys()])
    for  upper_score in range(MAX_UPPER_SCORE):
        for k, roll in enumerate(reverse_probability_dict.keys()):
            index = np.array(reverse_probability_dict[roll].values()) > 0
            tmp_prob_matrix = prob_matrix[index,:]
            for l, scorecard in enumerate(scorecard_dict.keys()):
                max_expected_value = 0
                tmp_fwd_scores = game_frame[subturn+1, upper_score, :, l]['score']
                max_expected_value = np.max(np.dot(tmp_prob_matrix, tmp_fwd_scores))
                game_frame[subturn, upper_score, k, l]["score"] = max_expected_value

def fill_turn(game_map, turn, reverse_probability_dict, forward_probability_dict, scorecard_dict, forward_scorecard_dicti, pre_calc_score_dict):
    game_frame = game_map[turn]
    forward_game_frame = game_map[turn+1]
    fill_last_subturn(game_frame, forward_game_frame, reverse_probability_dict, forward_probability_dict, scorecard_dict,forward_scorecard_dicti, pre_calc_score_dict)
    for subturn in range(NBR_SUBTURNS-1)[::-1]:
        print turn, subturn
        fill_roll_subturn(game_frame, subturn, reverse_probability_dict, forward_probability_dict, scorecard_dict)
    
def generate_game_map():
    forward_probability_dict, reverse_probability_dict = dice_probability_dict()
    pre_calc_score_dict = pre_calc_scores(reverse_probability_dict.keys())
    category_index = [{ key: i for  i, key in enumerate(generate_keys_for_turn(turn))} for turn in range(NBR_TURNS)]
    nbr_unique_rolls = len(reverse_probability_dict.keys())
    game_map = [None for turn in range(NBR_TURNS)]
    for turn in range(NBR_TURNS):
        nbr_scorecards = len(category_index[turn].keys())
        game_map[turn] = np.zeros((NBR_SUBTURNS, MAX_UPPER_SCORE, nbr_unique_rolls, nbr_scorecards), dtype = yahtzee_dtype)

    fill_end_states(game_map[-1], reverse_probability_dict, forward_probability_dict, category_index[-1])
    for turn in range(NBR_TURNS-1)[::-1]:
        fill_turn(game_map, turn, reverse_probability_dict, forward_probability_dict, category_index[turn], category_index[turn+1], pre_calc_score_dict)
    return game_map

if __name__ == "__main__":
    import time
    start = time.time()
    game_map = generate_game_map()
    stop = time.time()
    diff = stop - start
    print "stop",stop
    print "start",start
    print "diff",diff
    import pickle
    forward_probability_dict, reverse_probability_dict = dice_probability_dict()
    tmp_probability = np.array(forward_probability_dict[tuple()].values())
    tmp_scores = game_map[0][0, 0 ]
    tmp_scores = game_map[0][0, 0, :, 0]['score']
    expected_value = np.dot(tmp_probability, tmp_scores)
    print expected_value
    for i,gm in enumerate(game_map):
        f = open("game_map_%i"%i, 'wb')
        pickle.dump(gm, f)
        f.close()
