from yahtzee_probability import dice_probability_dict 
from yahtzee_scorecard import Scorecard 
import numpy as np

def test_create_scorecard():
    scorecard = Scorecard()
    assert(scorecard.categories)

def test_scoring():
    scorecard = Scorecard()
    print [ cat.name for cat in scorecard.categories]
    rolls = (1,2,3,4,5)
    for cat in scorecard.categories:
        cat.fill(cat.score_roll(rolls))
    for cat in scorecard.categories:
        assert cat.filled
        print cat.score
    
    
if __name__ == "__main__":
    test_create_scorecard()
    test_scoring()
