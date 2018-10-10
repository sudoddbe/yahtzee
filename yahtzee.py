from yahtzee_probability import dice_probability_dict 
import itertools
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

