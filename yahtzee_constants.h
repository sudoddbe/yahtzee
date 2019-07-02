#ifndef YAHTZEE_CONSTANTS_H_
#define YAHTZEE_CONSTANTS_H_

#define NBR_DICES (5)
#define NBR_FACES (6)
#define NBR_CATEGORIES (15)
#define NBR_SCORECARDS ((1<<NBR_CATEGORIES))
#define NBR_REROLLS (3)
#define EXTRA_SCORE_THRESH (63)
#if 0
#define DEBUG(x) (x)
#else
#define DEBUG(x)
#endif
#endif /* YAHTZEE_CONSTANTS_H_ */
