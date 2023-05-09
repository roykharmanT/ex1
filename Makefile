HACK.C = ./tool/HackEnrollment.c
HACK.H = ./tool/HackEnrollment.h
STUDENT.C = ./tool/Student.c
STUDENT.H = ./tool/Student.h
MAIN = ./tool/main.c
EXEC = a #name of the execution file -- I saw there is already HackEnrollment.exe so didn't want to ruin it...


HackEnrollment: HackEnrollment.o IsraeliQueue.o main.o Student.o
	gcc main.o HackEnrollment.o IsraeliQueue.o Student.o -o EXEC

main.o: $(MAIN) $(HACK.H) $(STUDENT.H) IsraeliQueue.h
	gcc -c $(MAIN)

HackEnrollment.o: $(HACK.C) $(HACK.H) $(STUDENT.H) IsraeliQueue.h
	gcc -c $(HACK.C)

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
	gcc -c IsraeliQueue.c

Student.o: $(HACK.C) $(HACK.H) $(STUDENT.H) IsraeliQueue.h
	gcc -c $(STUDENT.C)

clean:
	rm -f HackEnrollment.o IsraeliQueue.o main.o Student.o HackEnrollment




