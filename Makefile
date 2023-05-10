CC = gcc
OBJS = main.o HackEnrollment.o IsraeliQueue.o Student.o
PROG = program
EXEC = HackEnrollment
DEBUG_FLAG = -g
COMP_FLAG = -std=c99 -lm -Itool -Wall -pedantic-errors -Werror -DNDEBUG
TARGET = ExampleTest/out.txt
HACK.C = ./tool/HackEnrollment.c
HACK.H = ./tool/HackEnrollment.h
STUDENT.C = ./tool/Student.c
STUDENT.H = ./tool/Student.h
MAIN = ./tool/main.c
DIRECTION = shohamkalman/home/mtm/public/2223b/ex1


$(PROG) : $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $(EXEC)

HackEnrollment.o: $(HACK.C) $(STUDENT.H) $(HACK.H) IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(HACK.C)

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) IsraeliQueue.c

main.o: $(MAIN) $(HACK.H) $(STUDENT.H)
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(MAIN)

Student.o: $(STUDENT.H) $(STUDENT.C) $(HACK.H) IsraeliQueue.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $(STUDENT.C)

clean:
	rm -f $(OBJS) $(EXEC)
