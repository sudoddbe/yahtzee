from yahtzee_dtype import yahtzee_dtype, scorecard_dtype, NBR_SUBTURNS, MAX_UPPER_SCORE, NBR_CATEGORIES, NBR_TURNS
from yahtzee_probability import dice_probability_dict
from yahtzee_scoring_functions import score_category
import numpy as np
import multiprocessing

def generate_keys_for_turn(categories, turn):
    nbr_categories = len(categories)
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
def fill_end_states(last_frame, reverse_probability_dicti, scorecard_dict):
    subturn = 3
    for  upper_score in range(UPPER_MAX_SCORE):
        for k, roll in reverse_probability_dict.keys():
            for l, scorecard in scorecard_dict.keys():
                score, added_upper_score = score_category(np.bitwise_not(scorecard), roll)                
                if upper_score + added_upper_score > MAX_SCORE_UPPER -1 :
                    score += 50
                last_frame[subturn, upper_score, k, l]["score"] = score

def fill_last_subturn(game_map, forward_game_frame):
    subturn = 3
    for  upper_score in range(UPPER_MAX_SCORE):
        for k, roll in reverse_probability_dict.keys():
            for l, scorecard in scorecard_dict.keys():
                score, added_upper_score = score_category(np.bitwise_not(scorecard), roll)                
                if upper_score + added_upper_score > MAX_SCORE_UPPER -1 :
                    score += 50
                last_frame[subturn, upper_score, k, l]["score"] = score
def fill_turn(game_map, turn):
    game_frame = game_map[turn]
    forward_game_frame = game_map[turn+1]
    fill_last_subturn(game_frame, forward_game_frame)
    for subturn in range(NBR_SUBTURNS-1)[::-1]:
        fill_roll_subturn(game_frame)
    
#Game map has one entry for every round
#i.e three per category
def generate_game_map():
    _, reverse_probability_dict = dice_probability_dict()
    category_index = [{ key, i for key, i in enumerate(generate_keys_for_turn(scorecard_dtype.names, turn))} for turn in range(NBR_TURNS)]
    nbr_unique_rolls = len(reverse_probability_dict.keys())
    game_map = [None for turn in range(NBR_TURNS)]]
    for turn in range(NBR_TURNS):
        nbr_scorecards = len(category_index[turn].keys())
        game_map[turn] = np.zeros((NBR_SUBTURNS, UPPER_MAX_SCORE, nbr_unique_rolls, nbr_scorebards) dtype = "yahtzee_dtype")
    fill_end_states(game_map[-1], reverse_probability_dict, category_index[-1])
    for turn in range(NBR_TURNS-1)[::-1]:
        fill_turn(game_map, turn)

if __name__ == "__main__":
    game_map = generate_game_map()
    print "first", game_map[0]
    print "second",game_map[1]
    print "second to last", game_map[-2]
    print "last",game_map[-1]
    _, reverse_probability_dict = dice_probability_dict()
    print reverse_probability_dict.keys() 
    
    print len(game_map)
    for t in range(len(game_map) -2):
        turn = t+1
        print turn
        i = 0
        global latest_index 
        latest_index = 0
        for map_entry_frames in game_map[turn]:
            for map_entry_rows in map_entry_frames:
                for map_entry in map_entry_rows:
                    print turn, i, latest_index
                    i += 1
                    get_forward_neighbours(map_entry, game_map, turn)
