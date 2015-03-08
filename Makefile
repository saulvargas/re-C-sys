CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g
LDLIBS = -lm

EXE = test_recdata test_similarity test_recommender
OBJ = recdata.o recdata_simple.o recdata_transposed.o similarity.o similarity_setcosine.o similarity_veccosine.o topn.o recommender.o recommender_ub.o idpairs.o

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE): % : %.o $(OBJ)
