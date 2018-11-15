from yahtzee_probability import dice_probability_dict 
import itertools

def test_forward_and_backwards_probabilities(fpd, rpd):
    for input_set, result in fpd.iteritems():
        for output_set, probability in result.iteritems():
            assert fpd[input_set][output_set] == rpd[output_set][input_set]
            assert probability >= 0
            assert probability <= 1

#check results with pen and paper calculations...
#Might be redundant as this understanding is the input to the probability 
#calculations
def test_explicit_probability(fpd, input_sets, output_sets, expected_probabilities, verbose = False):
    for input_set, output_set, probability in zip(input_sets, output_sets, expected_probabilities):
            if verbose:
                print str(input_set) + " : " + str(output_set) + " - " + str(probability) + " : " + str(fpd[input_set][output_set])
            assert fpd[input_set][output_set] == probability

if __name__ == "__main__":
    dice_values = [1,2,3,4,5,6]
    input_sets = [p for n in range(6) for p in itertools.combinations_with_replacement(dice_values, n)]
    output_sets = [p for p in itertools.combinations_with_replacement(dice_values, 5)]
    fpd, rpd = dice_probability_dict(input_sets, output_sets)
    test_forward_and_backwards_probabilities(fpd,rpd)
    
    input_sets = [(), (1, 2, 3, 4), (1, 2, 3), (1, 2, 3), (1, 1, 1)]
    output_sets = [(6,6,6,6,6), (1, 2, 3, 4, 5), (1,2,3,4,5), (1,2,3,6,6), (1, 2, 3, 4, 5)]
    expected_probabilities = [(1/6.0)**5, (1/6.0), 2*(1/6.0)**2, (1/6.0)**2, 0.0]
    verbose = True
    test_explicit_probability(fpd, input_sets, output_sets, expected_probabilities, verbose = verbose)
