#ifndef EX1_HACKENROLLMENT_H
#define EX1_HACKENROLLMENT_H
#include "IsraeliQueue.h"
#include "Student.h"

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
EnrollmentSystem createEnrollment(File* students, File* courses, File* hackers);
//

//reading the file into enrollment system
EnrollmentSystem readEnrollment(EnrollmentSystem sys, File* queues);
//

//putting the hackers from the file in the courses they asked
void hackEnrollment(EnrollmentSystem sys, File* out);

//parse line to course
Course parseLineToCourse(char* line);

//returns the differences between two names
int nameDifferences(char* first, char* second);

//returns the difference between two ID
int idDifferences(char* first, char* second);

void putStudentInEnrollment(FILE* students, EnrollmentSystem enrollmentSystem);

void putCoursesInEnrollment(FILE* courses, EnrollmentSystem enrollmentSystem);

void putHackersInEnrollment(FILE* hackers, EnrollmentSystem enrollmentSystem);
















#endif //EX1_HACKENROLLMENT_H
