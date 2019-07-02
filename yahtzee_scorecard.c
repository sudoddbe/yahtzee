#include "yahtzee_scorecard.h"
#include "yahtzee_constants.h"

int ones_in_scorecard(int scorecard)
{
    assert(scorecard < NBR_SCORECARDS);
    int nbr_ones = 0;
    while(scorecard > 0){
        nbr_ones += scorecard & 1;
        scorecard = scorecard >> 1;

    }
    return nbr_ones;
}

void find_scorecards_for_turn(int turn, int* dest, unsigned int* nbr_scorecards){
    if (turn > NBR_CATEGORIES){
        printf("INVALID TURN \n");
        assert(false);
    }
    int index = 0;
    int sc;
    for( sc = 0; sc < NBR_SCORECARDS; sc++){
        if( ones_in_scorecard(sc) == turn){
            dest[index] = sc;
            index++;
            printf("Adding scorecard %i to turn %i \n", sc, turn);
        }
    }
    *nbr_scorecards = index;
}

void find_forward_scorecard(int current_scorecard, int* dest, unsigned int *nbr_forward_scorecards)
{
    //mask is a bitmask with all zeros except one bit
    int mask = (1 << (NBR_CATEGORIES - 1));
    int i = 0;
    while(mask > 0){
        //Use bitwise and to check if category is filled in scorecard
        int masked_bit = current_scorecard & mask;
        //If the masked bit is 0 the category is not filled, i.e it can be filled to form a forward scorecard
        if(masked_bit == 0){
            //add mask to fill the cateogry (the one bit is in the correct place already)
            int forward_scorecard = current_scorecard + mask;
            dest[i] = forward_scorecard;
            i++;
        }
        //Shift mask for next category
        mask = mask >> 1;
    }
    *nbr_forward_scorecards = i;
}

void bin_count(unsigned char* rolls, unsigned char* bins)
{
    /* memset maybe */
    int i;
    for(i = 0; i < NBR_FACES; i++){
        bins[i] = 0;
    }
    for(i = 0; i < NBR_DICES; i++){
        bins[rolls[i]-1]++;
    }
}

int score_face(unsigned char* rolls, unsigned char face)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    //Offset between index and dice value is 1 (i.e number of ones is stored in place 0)
    return face*bins[face - 1];
}

/* intended for easier use with function pointers with the same properties (i.e int (*fun)(unsigned char*) ) */
int score_ones(unsigned char* rolls)
{
    return score_face(rolls, 1);
}
int score_twos(unsigned char* rolls)
{
    return score_face(rolls, 2);
}

int score_threes(unsigned char* rolls)
{
    return score_face(rolls, 3);
}

int score_fours(unsigned char* rolls)
{
    return score_face(rolls, 4);
}

int score_fives(unsigned char* rolls)
{
    return score_face(rolls, 5);
}

int score_sixes(unsigned char* rolls)
{
    return score_face(rolls, 6);
}

int score_n_of_a_kind(unsigned char* rolls, unsigned char n){
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int i;
    for(i = NBR_FACES -1 ; i >= 0; i--){
        if (bins[i] >= n){
            //Offset between index and dice value is 1 (i.e number of ones is stored in place 0)
            return (i+1)*n;
        }
    }
    return 0;
}

/* intended for easier use with function pointers with the same properties (i.e int (*fun)(unsigned char*) ) */
int score_two_of_a_kind(unsigned char* rolls)
{
    return score_n_of_a_kind(rolls, 2);
}

int score_three_of_a_kind(unsigned char* rolls)
{
    return score_n_of_a_kind(rolls, 3);
}
int score_four_of_a_kind(unsigned char* rolls)
{
    return score_n_of_a_kind(rolls, 4);
}
int score_two_pair(unsigned char* rolls)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int score = 0;
    int done = 0;
    int i;
    for(i = NBR_FACES -1 ; i >= 0; i--){
        if (bins[i] == 2 || bins[i] == 3){
            score += 2*(i+1);
            done += 1;
            if (done == 2){
                return score;
            }
        }
    }
    return 0;
}

int score_full_house(unsigned char* rolls)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int score = 0;
    bool found_three = false;
    bool found_pair = false;
    int i;
    for(i = NBR_FACES -1 ; i >= 0; i--){
        if (bins[i] == 3 && !found_three){
            score += 3*(i+1);
            found_three = true;
        } else if (bins[i] == 2 && !found_pair){
            score += 2*(i+1);
            found_pair = true;
        }
        if (found_three && found_pair){
            return score;
        }
    }
    return 0;
}

int score_small_straight(unsigned char* rolls)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int i;
    for(i = 0; i < NBR_FACES -1; i++){
        if (bins[i] != 1){
            return 0;
        }
    }
    return 1 + 2+ 3+ 4+ 5;
}
int score_large_straight(unsigned char* rolls)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int i;
    for(i = 1; i < NBR_FACES ; i++){
        if (bins[i] != 1){
            return 0;
        }
    }
    return  2+ 3+ 4+ 5 +6;
}

int score_chance(unsigned char* rolls)
{
    int i;
    int sum = 0;
    for(i = 0; i < NBR_DICES; i++){
        sum += rolls[i];
    }
    return sum;
}
int score_yahtzee(unsigned char* rolls)
{
    unsigned char bins[NBR_FACES] =  {0};
    bin_count(rolls, bins);
    int i;
    for(i = 0; i < NBR_FACES; i++){
        if(bins[i] == NBR_DICES){
            return 50;
        }
    }
    return 0;
}

int (*fun_pointer[NBR_CATEGORIES]) (unsigned char*) = {
    score_ones,
    score_twos,
    score_threes,
    score_fours,
    score_fives,
    score_sixes,
    score_two_of_a_kind,
    score_three_of_a_kind,
    score_four_of_a_kind,
    score_two_pair,
    score_full_house,
    score_small_straight,
    score_large_straight,
    score_chance,
    score_yahtzee,
};

/* Have to handle top-half-score for bonus on receiving end */
int score_category(int category_int, unsigned char* rolls)
{
    return fun_pointer[category_int](rolls);
}

