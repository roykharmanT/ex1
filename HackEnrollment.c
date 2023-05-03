#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define STUDENTS_FILE_NAME "students.txt"
#define COURSE_FILE_NAME "courses.txt"
#define MAX_LENGTH 100


EnrollmentSystem createEnrollment(File* students, File* courses, File* hackers)
{
    EnrollmentSystem enrollmentSystem = (EnrollmentSystem) malloc(sizeof (*EnrollmentSystem));
    if(enrollmentSystem == NULL)
        return NULL;
    putStudentInEnrollment(students, enrollmentSystem);
    putCoursesInEnrollment(courses, enrollmentSystem);
    putHackersInEnrollment(hackers, enrollmentSystem);

}

void putStudentInEnrollment(FILE* students, EnrollmentSystem enrollmentSystem)
{
    if (students != NULL) {
        char *result = NULL;
        char *student_line = (char *) malloc(MAX_LENGTH * sizeof(char));
        while (true) {
            result = fgets(student_line, MAX_LENGTH, students);
            if (result == NULL)
                break;
            Student student_into_enrollment = parseLineToStudent(result);


            //enrollmentSystem->students[enrollmentSystem->index_students] = student_into_enrollment;
            //enrollmentSystem->index_students++;
            //---need to think how to do it
        }
    }
}

void putCoursesInEnrollment(FILE* courses, EnrollmentSystem enrollmentSystem)
{
    if(courses != NULL) {
        char *course_line = (char *) malloc(MAX_LENGTH * sizeof(char));
        while (true) {
            result = fgets(course_line, MAX_LENGTH, courses);
            if (result == NULL)
                break;
            Course course_into_enrollment = parseLineToCourse(course_line);

            //---need to complete
        }
    }
}


void putHackersInEnrollment(FILE* hackers, EnrollmentSystem enrollmentSystem)
{

}


Course parseLineToCourse(char* line)
{
    Course course = (Course)malloc(sizeof (*Course));
    course->course_queue = IsraeliQueueCreate();// should put the friendship function array and comparison
    char* token = strtok_s(token, line, NULL);
    course->course_number = token;
    token = strtok_s(token, line, NULL);
    course->size = token;
    return course;
}


int nameDifferences(Student first, Student second)
{
    int index = 0, sum = 0;
    while(first->first_name[index] && second->first_name[index]){
        sum += abs(first->first_name[index] - 'a') - abs(second->first_name[index] - 'a') ;
        index++;
    }
    index = 0;
    while(first->surname[index] && second->surname[index]){
        sum += abs(first->surname[index] - 'a') - abs(second->surname[index] - 'a') ;
        index++;
    }
    return sum;
}


int idDifferences(Student first, Student second)
{
    int index = 0, first_id = 0, second_id = 0;
    for(int i = 0;i < ID_LENGTH - 1; i++){
        first_id = first_id * 10 + first->student_id[i] - '0';
        second_id = second_id * 10 + second->student_id[i] - '0';
    }
    return abs(first_id - second_id);
}