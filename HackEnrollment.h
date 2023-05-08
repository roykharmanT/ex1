#ifndef EX1_HACKENROLLMENT_H
#define EX1_HACKENROLLMENT_H
#include "IsraeliQueue.h"
#include "Student.h"
#include <stdio.h>

typedef struct Course{
    IsraeliQueue course_queue;
    int course_number;
    int size;
}*Course;

//I thought to use indexes to put the object in their place in the array but then we need to allocate
//Every time or to go through the file twice to see how many students/hackers/courses there are.
//Or we can do linked list ("reshima mekusheret")
typedef struct EnrollmentSystem{
    Course* courses;
    int index_courses;
    Student* students;
    int index_students;
    Hacker* hackers;
    int index_hackers
}*EnrollmentSystem;

//creates an Enrollment System that will contain the data from the files
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);
//

//reading the file into enrollment system
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);
//

//putting the hackers from the file in the courses they asked
void hackEnrollment(EnrollmentSystem sys, FILE* out);

//parse line to course
Course parseLineToCourse(char* line);

//returns the differences between two names
int nameDifferences(Student first, Student second);

//returns the difference between two ID
int idDifferences(Student first, Student second);

void putStudentInEnrollment(FILE* students, EnrollmentSystem enrollmentSystem);

void putCoursesInEnrollment(FILE* courses, EnrollmentSystem enrollmentSystem);

void putHackersInEnrollment(FILE* hackers, EnrollmentSystem enrollmentSystem);
//get the length of a file
int getNumOfLines(FILE* file_to_read);
//allocate an enrollment system
<<<<<<< HEAD
EnrollmentSystem mallocEnrollmentSystem(File* students, File* courses, File* hackers);
//return how many strings in the line
int getNumOfStringsInTheLine(char* line);
=======
EnrollmentSystem mallocEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers);
//returns the max line length in the file
int getMaxLineLength(FILE* file_to_read);
//returns the max string length int the file
int getMaxStrLength(FILE* file_to_read);

//find student in enrollment by his id
Student findStudentById(EnrollmentSystem sys, char* id);
>>>>>>> main










#endif //EX1_HACKENROLLMENT_H
