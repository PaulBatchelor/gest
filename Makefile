OBJ=gest.o skgest.o

CFLAGS=-Wall -pedantic -O3 -g

WORGLE=skorgle

default: lilgest

lilgest: $(OBJ) lilgest.o l_gest.o
	$(CC) $(CFLAGS) $(OBJ) l_gest.o lilgest.o -o $@ -lsndkit -lm

gest.c: gest.org
	$(WORGLE) -g -Werror $<

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	mkdir -p /usr/local/bin
	cp lilgest /usr/local/bin

clean:
	$(RM) $(OBJ) lilgest.o l_gest.o
	$(RM) gest.c gest.h
	$(RM) lilgest
