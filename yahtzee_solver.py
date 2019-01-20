from yahtzee_probability import dice_probability_dict
from yahtzee_scorecard import Scorecard
import numpy as np

class Solver:

    def __init__(self, scorecard = None, reverse_probability_dict = None):
        if scorecard is None:
            self.scorecard = Scorecard()
        else:
            self.scorecard = scorecard

        if reverse_probability_dict is None:
            _, self.reverse_probability_dict = dice_probability_dict()
        else:
            self.reverse_probability_dict = reverse_probability_dict

        #The game map should answer what to do in every situation
        #initially we will solve a game with no rerolls
        #The map should at every round say what to do with every
        #roll and every possible scorecard.
        self.game_map = self.create_game_map()

    #Always select the highest scoring category
    def select_category_greedy(self, rolls):
        score_dict = self.scorecard.score_all_categories(rolls)
        i = np.argmax(score_dict.values())
        return score_dict.keys()[i]

    def create_game_map(self):
        game_map = {}
        for turn in range(len(self.scorecard.categories)):
            keys = self.scorecard.generate_keys_for_turn(turn)
            key_dict = {}
            for key in keys:
                key_dict[key] = {output: None for output in self.reverse_probability_dict.keys()}
            game_map[turn] = key_dict
        return game_map


