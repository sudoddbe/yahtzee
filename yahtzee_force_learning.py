import numpy as np
import matplotlib.pyplot as plt
import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras import backend as K

from yahtzee_main import load_all_matrices, get_index, get_input_from_index
from output.yahtzee_constants import *

(input_rolls, output_rolls, forward_scorecards, probability_matrix, category_score, score_map, best_moves) = load_all_matrices()
output_rolls = np.array([np.array(roll) for roll in output_rolls])

#Every index below this is a reroll
def get_reroll_max_index():
    return NBR_REROLLS * (MAX_UPPER_SCORE*NBR_SCORECARDS*NBR_ROLLS)

def get_data(index):
    scorecard, roll_index, subturn, upper_score = get_input_from_index(index)
    roll = output_rolls[roll_index]
    features = np.array([scorecard, subturn, upper_score])
    features = np.vstack((features, roll.T)).T
    label = best_moves[index].T
    label = keras.utils.to_categorical(label, num_classes = len(input_rolls))

    return (features, label)


batch = 1024 
max_ind = get_reroll_max_index()
max_ind = 1000*batch 
model = Sequential()
model.add(Dense(128, activation='relu', input_shape = (8,)))
model.add(Dense(128, activation='relu'))
model.add(Dense(len(input_rolls), activation='softmax'))

model.compile(loss="MSE", optimizer="Adam", metrics=['accuracy'])
for e in range(10):
    shuffle = np.random.permutation(max_ind)
    for s in range(0, max_ind, batch):
        slice_step = slice(s, s+batch)
        tmp_index = shuffle[slice_step]
        x_train, y_train = get_data(tmp_index)

        model.fit(x_train, y_train, batch_size=128, epochs=40, verbose=1)
        score = model.evaluate(x_train, y_train, verbose=0)
        print 'Test loss:', score[0]
        print 'Test accuracy:', score[1]
