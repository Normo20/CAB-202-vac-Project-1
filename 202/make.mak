# Makefile for my vacuum program!
#
# $Revision:Sat apr 19 01:38:04 2019$

TARGETS=libvac.a 

FLAGS=-Wall -Werror -std=gnu99 -g
LIB_SRC= helpers.c draw_border.c timers.c vacuum.c charging.c trash.c misc.c
LIB_HDR= helpers.h draw_border.h timers.h vacuum.h charging.h trash.h misc.h
LIB_OBJ= helpers.o draw_border.o timers.c vacuum.o charging.o trash.o misc.o

all: $(TARGETS)

clean:
	for f in $(TARGETS); do \
		if [ -f $${f} ]; then rm $${f}; fi; \
		if [ -f $${f}.exe ]; then rm $${f}.exe; fi; \
	done

rebuild: clean all

libvac.a: $(LIB_SRC) $(LIB_HDR)
	gcc -c $(LIB_SRC) $(FLAGS)
	ar r $@ $(LIB_OBJ)
	rm $(LIB_OBJ)