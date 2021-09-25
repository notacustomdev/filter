IDIR =.
CC=g++
#CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm -lgsl -lgslcblas

_DEPS = filter.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o filter.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

filter: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 