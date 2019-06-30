IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib
LIBS=-lm

_DEPS = yahtzee_probability_matrix.h yahtzee_scorecard.h yahtzee_constants.h yahtzee_game_map.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = yahtzee_probability_matrix.o yahtzee_scorecard.o yahtzee_game_map.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

install: $(OBJ)
	$(CC) -o YahtzeeGame $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~ YahtzeeGame
check: $(OBJ) check/yahtzee_probability_check.c check/yahtzee_scorecard_check.c
	gcc yahtzee_probability_matrix.c check/yahtzee_probability_check.c -o $(ODIR)/test_probability_matrix.o -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit $(CFLAGS) 
	gcc yahtzee_scorecard.c check/yahtzee_scorecard_check.c -o $(ODIR)/test_scorecard.o -lcheck -pthread -lcheck_pic -pthread -lrt -lm -lsubunit $(CFLAGS) 
	./$(ODIR)/test_probability_matrix.o && ./$(ODIR)/test_scorecard.o
