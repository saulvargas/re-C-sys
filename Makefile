CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g -std=c99
LDLIBS = -lm

EXE = test_recdata test_usersim test_itemsim test_ub test_ib test_pop
OBJ = recdata.o recdata_simple.o recdata_transposed.o similarity.o similarity_setcosine.o similarity_veccosine.o topn.o recommender.o recommender_ub.o recommender_ib.o idpairs.o aux.o recommender_pop.o

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE): % : %.o $(OBJ)
