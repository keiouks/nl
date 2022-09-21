TARGET = nl
CC=gcc
OBJS = \
	lex.yy.o\
	y.tab.o\
	create.o\
	eval.o\
	main.o
CFLAGS = -c -g -Wall -Wswitch-enum -ansi -pedantic -DDEBUG
INCLUDES = \

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ -lm
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
y.tab.h : nl.y
	yacc -dv nl.y
y.tab.c : nl.y
	yacc -dv nl.y
lex.yy.c : nl.l nl.y y.tab.h nl.h
	lex nl.l
y.tab.o : y.tab.c nl.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o : lex.yy.c nl.h
	$(CC) -c -g $*.c $(INCLUDES)

create.o: create.c nl.h
eval.o: eval.c nl.h
main.o: main.c 
