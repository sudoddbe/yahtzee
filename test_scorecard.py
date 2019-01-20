from yahtzee_scorecard import Scorecard
import numpy as np

def test_create_scorecard():
    scorecard = Scorecard()
    assert(scorecard.categories)

def test_all_scoring():
    scorecard = Scorecard()
    names = [cat.name for cat in scorecard.categories]

    roll = (1,1,1,1,1)
    expected_scores = { name:0 for name in names}
    expected_scores['ones'] = 5
    expected_scores['pair'] = 2
    expected_scores['three_of_a_kind'] = 3
    expected_scores['four_of_a_kind'] = 4
    expected_scores['full_house'] = 5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (2,2,2,2,2)
    expected_scores = { name:0 for name in names}
    expected_scores['twos'] = 2*5
    expected_scores['pair'] = 2*2
    expected_scores['three_of_a_kind'] = 2*3
    expected_scores['four_of_a_kind'] = 2*4
    expected_scores['full_house'] = 2*5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (3,3,3,3,3)
    expected_scores = { name:0 for name in names}
    expected_scores['threes'] = 3*5
    expected_scores['pair'] = 3*2
    expected_scores['three_of_a_kind'] = 3*3
    expected_scores['four_of_a_kind'] = 3*4
    expected_scores['full_house'] = 3*5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (4,4,4,4,4)
    expected_scores = { name:0 for name in names}
    expected_scores['fours'] = 4*5
    expected_scores['pair'] = 4*2
    expected_scores['three_of_a_kind'] = 4*3
    expected_scores['four_of_a_kind'] = 4*4
    expected_scores['full_house'] = 4*5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (5,5,5,5,5)
    expected_scores = { name:0 for name in names}
    expected_scores['fives'] = 5*5
    expected_scores['pair'] = 5*2
    expected_scores['three_of_a_kind'] = 5*3
    expected_scores['four_of_a_kind'] = 5*4
    expected_scores['full_house'] = 5*5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (6,6,6,6,6)
    expected_scores = { name:0 for name in names}
    expected_scores['sixes'] = 6*5
    expected_scores['pair'] = 6*2
    expected_scores['three_of_a_kind'] = 6*3
    expected_scores['four_of_a_kind'] = 6*4
    expected_scores['full_house'] = 6*5
    expected_scores['yahtzee'] = 50
    test_scoring(roll, expected_scores)

    roll = (1,2,5,5,6)
    expected_scores = { name:0 for name in names}
    expected_scores['ones'] = 1
    expected_scores['twos'] = 2
    expected_scores['fives'] = 2*5
    expected_scores['sixes'] = 6
    expected_scores['pair'] = 2*5
    test_scoring(roll, expected_scores)

    roll = (1,5,5,5,6)
    expected_scores = { name:0 for name in names}
    expected_scores['ones'] = 1
    expected_scores['fives'] = 3*5
    expected_scores['sixes'] = 6
    expected_scores['pair'] = 2*5
    expected_scores['three_of_a_kind'] = 3*5
    test_scoring(roll, expected_scores)

    roll = (1,5,5,5,5)
    expected_scores = { name:0 for name in names}
    expected_scores['ones'] = 1
    expected_scores['fives'] = 4*5
    expected_scores['pair'] = 2*5
    expected_scores['three_of_a_kind'] = 3*5
    expected_scores['four_of_a_kind'] = 4*5
    test_scoring(roll, expected_scores)

    roll = (5,5,5,6,6)
    expected_scores = { name:0 for name in names}
    expected_scores['fives'] = 3*5
    expected_scores['sixes'] = 2*6
    expected_scores['pair'] = 2*6
    expected_scores['three_of_a_kind'] = 3*5
    expected_scores['full_house'] = np.sum(roll)
    test_scoring(roll, expected_scores)

    roll = (1,2,3,4,5)
    expected_scores = { name:0 for name in names}
    expected_scores['ones'] = 1
    expected_scores['twos'] = 2
    expected_scores['threes'] = 3
    expected_scores['fours'] = 4
    expected_scores['fives'] = 5
    expected_scores['small_straight'] = np.sum(roll)
    test_scoring(roll, expected_scores)

    roll = (2,3,4,5,6)
    expected_scores = { name:0 for name in names}
    expected_scores['twos'] = 2
    expected_scores['threes'] = 3
    expected_scores['fours'] = 4
    expected_scores['fives'] = 5
    expected_scores['sixes'] = 6
    expected_scores['large_straight'] = np.sum(roll)
    test_scoring(roll, expected_scores)


def test_scoring(roll, expected_scores):
    scorecard = Scorecard()
    print [ cat.name for cat in scorecard.categories]
    for cat in scorecard.categories:
        cat.fill(cat.score_roll(roll))
    for cat in scorecard.categories:
        print "Testing scoring of %s" % cat.name
        print "Roll : %s" % str(roll)
        if not cat.filled:
            print "Not filled"
            assert False
        if cat.score != expected_scores[cat.name]:
            print "Wrong score expected %i but got %i" % (expected_scores[cat.name], cat.score)
            assert False
    scorecard = Scorecard()
    score_dict = scorecard.score_all_categories(roll)
    for name,score in score_dict.iteritems():
        if score != expected_scores[name]:
            print "Wrong score expected %i but got %i" % (expected_scores[name], score)
            assert False
#Todo : Needs further testing
def test_key_generation():
    keys = Scorecard.generate_keys_for_turn(turn= 0)
    assert(keys[0] == 0)
    assert(len(keys) == 1)
    keys = Scorecard.generate_keys_for_turn(turn= 1)
    assert(len(keys) == 13)
    keys = Scorecard.generate_keys_for_turn(turn= 13)
    assert(len(keys) == 1)
    keys = Scorecard.generate_keys_for_turn(turn= 12)
    assert(len(keys) == 13)

    keys
if __name__ == "__main__":
    test_create_scorecard()
    test_all_scoring()
    test_key_generation()
