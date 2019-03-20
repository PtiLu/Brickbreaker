SHELL  = /bin/bash
CC     = gcc
RM     = rm -f
CFLAGS = `pkg-config gtk+-2.0 --cflags` -W -Wall -pedantic -ansi
LIBS   = `pkg-config gtk+-2.0 --libs` -lm
GENDEP = gcc -MM $(CFLAGS)
EXEC1  = pong 
EXEC2  = editeur
EXEC3  = my_game

OBJECTS= pong.o handler.o sui-gtk.o editeur.o my_game.o

.c.o : 
	$(CC) $(CFLAGS) -c *.c

pong : pong.o handler.o sui-gtk.o
editeur : editeur.o sui-gtk.o
my_game : my_game.o sui-gtk.o

all :: $(EXEC1)$(EXEC2) $(EXEC3)

$(EXEC1) :
	$(CC) -o $@ $@.o handler.o sui-gtk.o $(LIBS)
$(EXEC2) : 
	$(CC) -o $@ $@.o sui-gtk.o $(LIBS)
$(EXEC3) :
	$(CC) -o $@ $@.o sui-gtk.o $(LIBS)

clean ::
	$(RM) *.o $(EXEC1) $(EXEC2) $(EXEC3) *~
	true >| depend

