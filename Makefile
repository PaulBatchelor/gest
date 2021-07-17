OBJ=gest.o skgest.o

CFLAGS=-Wall -Wno-format -O3 -g

default: example lilgest

example: $(OBJ) example.o
	$(CC) $(CFLAGS) $(OBJ) example.o -o $@ -lsndkit

lilgest: $(OBJ) lilgest.o l_gest.o
	$(CC) $(CFLAGS) $(OBJ) l_gest.o lilgest.o -o $@ -lsndkit

gest.c: gest.org
	worgle -g -Werror $<

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJ) example.o lilgest.o l_gest.o
	$(RM) example
	$(RM) gest.c gest.h
	$(RM) lilgest
