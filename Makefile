CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g
LDLIBS = -lm

EXE = test_recdata test_similarity test_recommender
OBJ = recdata.o recdata_simple.o recdata_transposed.o similarity.o similarity_cosine0.o topn.o recommender.o recommender_ubknn.o idpairs.o

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE): % : %.o $(OBJ)
