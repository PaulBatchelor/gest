OBJ=gest.o skgest.o

CFLAGS=-Wall -pedantic -O3 -g

default: lilgest

lilgest: $(OBJ) lilgest.o l_gest.o
	$(CC) $(CFLAGS) $(OBJ) l_gest.o lilgest.o -o $@ -lsndkit -lm

gest.c: gest.org
	worgle -g -Werror $<

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJ) lilgest.o l_gest.o
	$(RM) gest.c gest.h
	$(RM) lilgest
