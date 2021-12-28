OBJ=gest.o skgest.o gestlang.o

CFLAGS=-Wall -pedantic -O3 -g

WORGLE=skorgle

default: lilgest

lilgest: $(OBJ) lilgest.o l_gest.o l_gestlang.o
	$(CC) $(CFLAGS) $(OBJ) l_gest.o l_gestlang.o lilgest.o -o $@ -lsndkit -lm

gest.c: gest.org
	$(WORGLE) -g -Werror $<

gestlang.c: gestlang.org
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
