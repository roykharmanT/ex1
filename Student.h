#ifndef EX1_STUDENT_H
#define EX1_STUDENT_H

#define ID_LENGTH 9
#define MAX_STR_LENGTH 30

typedef struct Student{
    char* student_id;
    int total_credits;
    int gpa;
    char* first_name;
    char* surname;
    char* city;
    char* department;
}*Student;

typedef struct Hacker{
    char* hacker_id;
    int* desired_courses;
    char** friends_id;
    char** rivals_id;
}*Hacker;

//parse line data into student
Student parseLineToStudent(char* line);

//allocates new student
Student mallocStudent();
//allocates new hacker
Hacker mallocHacker(int num_of_courses, int num_of_friends, int num_of_rivals);

int stringToInt(char* str);



















#endif //EX1_STUDENT_H
