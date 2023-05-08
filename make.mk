program: HackEnrollment.o IsraeliQueue.o main.o Student.o

HackEnrollment.o: HackEnrollment.c HackEnrollment.h IsraeliQueue.h Student.h
    gcc -c HackEnrollment.c

IsraeliQueue.o: IsraeliQueue.c IsraeliQueue.h
    gcc -c IsraeliQueue.c

main.o: main.c HackEnrollment.h IsraeliQueue.h Student.h
   gcc -c main.c

Student.o: Student.c HackEnrollment.h IsraeliQueue.h Student.h
    gcc -C Student.c