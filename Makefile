CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -g
LDLIBS = -lm

EXE = test_recdata test_similarity
OBJ = recdata.o recdata_simple.o recdata_transposed.o similarity.o similarity_cosine0.c topn.c

all : $(EXE)

clean :
	rm -f *.o core $(EXE)

$(EXE): % : %.o $(OBJ)
	$(CC) $(CFLAGS) -o $@ $@.o $(OBJ) $(LDLIBS)
