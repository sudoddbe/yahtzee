import numpy as np
from scipy.special import factorial
def tuple_is_subset(big_set, small_set):
    N_small = len(small_set) 
    if N_small == 0:
        return (True, big_set)
    N_big = len(big_set)
    tmp_set = [e for e in big_set]
    for es in small_set:
        if es in tmp_set:
            tmp_set.remove(es)
        else:
            #Not a subset
            return (False, None)
    return (True, tmp_set)

def roll_probability(wanted_rolls):
    rolls = len(wanted_rolls)
    dice_values = [1,2,3,4,5,6]
    repitions = {dv: 0 for dv in dice_values}
    for dv in dice_values:
        for wr in wanted_rolls:
            repitions[dv] += wr == dv
    probability = factorial(rolls) *(1.0/6.0)**rolls
    probability /= np.prod(factorial(np.array(repitions.values())))
    return probability 

def dice_probability_dict(input_sets, output_sets):
    forward_probability_dict = {input_set : {} for input_set in input_sets}
    reverse_probability_dict = {output_set : {} for output_set in output_sets}
    for input_set in input_sets:
        for output_set in output_sets:
            (is_subset,diff_set) = tuple_is_subset(output_set, input_set)
            if not is_subset:
                forward_probability_dict[input_set][output_set] = 0.0
                reverse_probability_dict[output_set][input_set] = 0.0
            else:
                #Currently no consideration of combinations...
                probability = roll_probability(diff_set)
                forward_probability_dict[input_set][output_set] = probability 
                reverse_probability_dict[output_set][input_set] = probability 
    return forward_probability_dict, reverse_probability_dict

