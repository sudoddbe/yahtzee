from yahtzee_dtype import game_state_dtype, scorecard_dtype, NBR_ROLLS
from yahtzee_probability import dice_probability_dict
import numpy as np
import multiprocessing

latest_index = 0
def get_forward_neighbours_roll_to_roll(map_entry, game_map, turn):
    global latest_index
    forward = game_map[turn+1]
    #assert(map_entry["nbr_roll"] < 2)
    for i,state in enumerate(forward[latest_index:, 0]):
        if np.all(map_entry["scorecard"] == state["scorecard"]):
            latest_index = latest_index + i
            print latest_index
            print "i",i
            return forward[latest_index, :]

def get_forward_neighbours_roll_to_select(map_entry, game_map, turn):
    forward = game_map[turn+1]
    #assert(False)
    assert(map_entry["nbr_roll"] == 2)

    for j,state in enumerate(forward[0, :]):
        if np.all(map_entry["current_roll"] == state["current_roll"]):
            break

    scorecard_index = []
    for i,state in enumerate(forward[:, j]):
        scorecard_diff = state["scorecard_byte"] - map_entry["scorecard_byte"]
        if np.any(scorecard_diff < 0):
                continue;
        if np.sum(scorecard_diff) == 1:
            scorecard_index.append(i)
    return forward[scorecard_index, j]

def get_forward_neighbours_select_to_roll(map_entry, game_map, turn):
    global latest_index
    forward = game_map[turn+1]
    assert(map_entry["nbr_roll"] == 3)
    for i,state in enumerate(forward[latest_index:, 0]):
        if (map_entry["scorecard"] == state["scorecard"]):
            latest_index = latest_index + i
            print latest_index
            print i
            return forward[latest_index, :]

def get_forward_neighbours(map_entry, game_map, turn):
    if map_entry["nbr_roll"] < 2:
        return get_forward_neighbours_roll_to_roll(map_entry, game_map, turn)

    if map_entry["nbr_roll"] ==  2:
        return get_forward_neighbours_roll_to_select(map_entry, game_map, turn)

    if map_entry["nbr_roll"] ==  3:
        return get_forward_neighbours_select_to_roll(map_entry, game_map, turn)

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
    game_map = [ None for turn in range((NBR_ROLLS +1)*(len(scorecard_dtype.names[1:]) +1))]
    _, reverse_probability_dict = dice_probability_dict()
    p_list = []
    out_queue = multiprocessing.Queue()
    for turn in range(len(scorecard_dtype.names[1:]) +1):
        for nbr_roll in range(NBR_ROLLS + 1):
            p_list.append(multiprocessing.Process(target = set_map_entry, args = (out_queue, turn, nbr_roll, reverse_probability_dict,)))
    for p in p_list:
        p.start()

    for p in p_list:
        (turn, nbr_roll, map_entry) = out_queue.get()
        game_map[4*turn + nbr_roll] = map_entry
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
    for t in range(len(game_map) -2):
        turn = t+1
        print turn
        i = 0
        global latest_index 
        latest_index = 0
        for map_entry_rows in game_map[turn]:
            for map_entry in map_entry_rows:
                print turn, i, latest_index
                i += 1
                get_forward_neighbours(map_entry, game_map, turn)
