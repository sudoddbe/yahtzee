from yahtzee_dtype import game_state_dtype, scorecard_dtype, NBR_ROLLS
from yahtzee_probability import dice_probability_dict
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

def set_map_entry(out_queue, turn, nbr_roll, reverse_probability_dict):
    keys = generate_keys_for_turn(scorecard_dtype.names[1:], turn)
    map_entry = np.zeros((len(keys), len(reverse_probability_dict.keys())), dtype=game_state_dtype)
    for i, key in enumerate(keys):
        for j, current_rolls in enumerate(reverse_probability_dict.keys()):
            map_entry[i,j]["nbr_roll"] = nbr_roll
            map_entry[i,j]["current_roll"] = np.array(current_rolls, dtype="B")
            binary = np.binary_repr(key)
            binary = map(int, list(binary))
            for c, v in zip(scorecard_dtype.names[1:], binary):
                map_entry[i,j]["scorecard"][c] = v
    out_queue.put((turn, nbr_roll, map_entry))
    print 3*turn + nbr_roll

#Game map has one entry for every round
#i.e three per category
def generate_game_map():
    game_map = [ None for turn in range(NBR_ROLLS*(len(scorecard_dtype.names[1:]) +1))]
    _, reverse_probability_dict = dice_probability_dict()
    p_list = []
    out_queue = multiprocessing.Queue()
    for turn in range(len(scorecard_dtype.names[1:]) +1):
        for nbr_roll in range(NBR_ROLLS):
            p_list.append(multiprocessing.Process(target = set_map_entry, args = (out_queue, turn, nbr_roll, reverse_probability_dict,)))
    for p in p_list:
        p.start()

    for p in p_list:
        (turn, nbr_roll, map_entry) = out_queue.get()
        game_map[3*turn + nbr_roll] = map_entry
    return game_map

if __name__ == "__main__":
    game_map = generate_game_map()
    print "first", game_map[0]
    print "second",game_map[1]
    print "second to last", game_map[-2]
    print "last",game_map[-1]
    _, reverse_probability_dict = dice_probability_dict()
    print reverse_probability_dict.keys() 
    
    print len(game_map)
