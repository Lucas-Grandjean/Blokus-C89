CC = gcc

#---------------------------------------------------------------------------------
# options de compilation
#---------------------------------------------------------------------------------
CFLAGS = -Wall -g -ansi -pedantic

EXE = main.out
OFILES = board.o main.o mainscreen.o maincommands.o interface.o resultscreen.o

LIBS = -lgraph

but : ${EXE}

board.o : board.h
main.o : board.h
mainscreen.o : mainscreen.h
maincommands.o : maincommands.h
interface.o : interface.h
resultscreen.o : resultscreen.h


${EXE} : ${OFILES}
	${CC} ${CFLAGS} -o ${EXE} ${OFILES} ${LIBS}

clean :
	-rm -f ${OFILES}

run:
	make
	./main.out

.PHONY : but clean run