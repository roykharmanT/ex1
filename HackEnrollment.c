#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 100


EnrollmentSystem createEnrollment(File* students, File* courses, File* hackers)
{
    EnrollmentSystem enrollmentSystem = mallocEnrollmentSystem(students, courses, hackers);
    if(enrollmentSystem == NULL)
        return NULL;
    putStudentInEnrollment(students, enrollmentSystem);
    putCoursesInEnrollment(courses, enrollmentSystem);
    putHackersInEnrollment(hackers, enrollmentSystem);

    return enrollmentSystem;

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
            if(student_into_enrollment == NULL)
                break;
            enrollmentSystem->students[enrollmentSystem->index_students] = student_into_enrollment;
            enrollmentSystem->index_students++;
        }
        free(student_line);
    }
}

void putCoursesInEnrollment(FILE* courses, EnrollmentSystem enrollmentSystem)
{
    if(courses != NULL) {
        char *result = NULL;
        char *course_line = (char *) malloc(MAX_LENGTH * sizeof(char));
        if(course_line == NULL)
            return;
        while (true) {
            result = fgets(course_line, MAX_LENGTH, courses);
            if (result == NULL)
                break;
            Course course_into_enrollment = parseLineToCourse(course_line);
            enrollmentSystem->courses[enrollmentSystem->index_courses] = course_into_enrollment;
            enrollmentSystem->index_courses++;
        }
        free(course_line);
    }

}


void putHackersInEnrollment(FILE* hackers, EnrollmentSystem enrollmentSystem)
{
    if(hackers == NULL)
        return;
    //each hacker has 4 lines that describes him
    enrollmentSystem->hackers = (Hacker*) malloc((getNumOfLines(hackers) / 4)* sizeof(Hacker));
    if(enrollmentSystem->hackers == NULL)
        return;
    char* line = (char *) malloc(MAX_LENGTH * sizeof(char));
    if(line == NULL) {
        free(enrollmentSystem->hackers);
        return;
    }

    int num_of_friends, num_of_courses, num_of_rivals, index = 0, current = 0;
    char* result = NULL;
    while(true){
        result = fgets(line, MAX_LENGTH, hackers);
        if(result == NULL)
            break;
        parseLineToHacker(enrollmentSystem->hackers[enrollmentSystem->index_hackers], result, (index % 4));
        index++;
        if(index % 4 == 0)
            enrollmentSystem->index_hackers++;
    }
    num_of_courses = getNumOfStringsInTheLine(line);
    result = fgets(line, MAX_LENGTH, hackers);
    if(result != NULL)
        num_of_friends = getNumOfStringsInTheLine(line);
    result = fgets(line, MAX_LENGTH, hackers);
    if(result != NULL)
        num_of_rivals = getNumOfStringsInTheLine(line);
/*
        Hacker hacker_into_enrollment = mallocHacker(num_of_courses,num_of_friends,num_of_rivals);
        if(new_hacker != NULL)
            enrollmentSystem->hackers[enrollmentSystem->index_hackers] = hacker_into_enrollment;
 */

}

int getNumOfStringsInTheLine(char* line)
{
    int index = 0, cnt = 0;
    while(line[index] != '\n'){
        if(line[index] == ' ')
            cnt++;
        index++;
    }
    return cnt;
}
Course parseLineToCourse(char* line)
{
    Course course = (Course)malloc(sizeof (*Course));
    if(course == NULL)
        return NULL;
    course->course_queue = IsraeliQueueCreate();// should put the friendship function array and comparison
    if(course->course_queue == NULL){
        free(course);
        return NULL;
    }
    char* space = " ";
    char* token = strtok_s(token, line, &space);
    course->course_number = stringToInt(token);
    token = strtok_s(token, NULL, &space);
    course->size = stringToInt(token);
    return course;
}


int nameDifferences(Student first, Student second)
{
    int index = 0, sum = 0;
    while(first->first_name[index] && second->first_name[index]){
        sum += abs((first->first_name[index] - 'a') - (second->first_name[index] - 'a')) ;
        index++;
    }
    index = 0;
    while(first->surname[index] && second->surname[index]){
        sum += abs((first->surname[index] - 'a') - (second->surname[index] - 'a')) ;
        index++;
    }
    return sum;
}


int idDifferences(Student first, Student second)
{
    int index = 0, first_id = 0, second_id = 0;
    for(int i = 0;i < ID_LENGTH; i++){
        first_id = first_id * 10 + first->student_id[i] - '0';
        second_id = second_id * 10 + second->student_id[i] - '0';
    }
    return abs(first_id - second_id);
}

void getMaxStrAndLineLength(FILE* file_to_read, int* max_str, int* max_line)
{
    if(file_to_read != NULL) {
        int ch, cnt_str = 0, cnt_line = 0;
        while (true) {
            ch = fgetc(file_to_read);
            if (ch == EOF)
                break;
            if (ch == ' ') {
                if (cnt_str > *max_str) {
                    *max_str = cnt_str;
                }
                cnt_str = 0;
            }
            else
                cnt_str++;
            if (ch == '\n') {
                if (cnt_line > *max_line) {
                    *max_line = cnt_line;
                }
                cnt_line = 0;
            }
            else
                cnt_line++;
        }
    }
    rewind(file_to_read);
}
int getNumOfLines(FILE* file_to_read)
{
    int ch, lines_counter = 0;
    while(true){
        ch = fgetc(file_to_read);
        if(ch == EOF)
            break;
        if(ch == '\n')
            lines_counter++;
    }
    rewind(file_to_read);
    return lines_counter;
}

EnrollmentSystem mallocEnrollmentSystem(File* students, File* courses, File* hackers)
{
    int students_length, courses_length, hackers_length;
    students_length = getNumOfLines(students);
    courses_length = getNumOfLines(courses);
    hackers_length = getNumOfLines(hackers);
    EnrollmentSystem enrollmentSystem = (EnrollmentSystem)malloc(sizeof(*EnrollmentSystem));
    if(enrollmentSystem == NULL)
        return NULL;
     enrollmentSystem->students = (Student*)malloc(students_length*sizeof(*Student));
    if(enrollmentSystem->students == NULL) {
        free(enrollmentSystem);
        return NULL;
    }
    enrollmentSystem->courses = (Course*)malloc(courses_length*sizeof(*Course));
    if(enrollmentSystem->courses == NULL) {
        free(enrollmentSystem->students);
        free(enrollmentSystem);
        return NULL;
    }
    enrollmentSystem->hackers = (Hacker*)malloc(hackers_length*sizeof(*Hacker));
    if(enrollmentSystem->hackers == NULL) {
        free(enrollmentSystem->students);
        free(enrollmentSystem->courses);
        free(enrollmentSystem);
        return NULL;
    }
    enrollmentSystem->index_courses = 0;
    enrollmentSystem->index_students = 0;
    return enrollmentSystem;

}
//another option that prevent code duplication
void putCoursesOrStudentsInEnrollment(FILE* file_to_read, EnrollmentSystem enrollmentSystem, char type)
{
    if(file_to_read != NULL) {
        char *result = NULL;
        char *line = (char *) malloc(MAX_LENGTH * sizeof(char));
        if(line == NULL)
            return;
        while (true) {
            result = fgets(line, MAX_LENGTH, file_to_read);
            if (result == NULL)
                break;
            if(type == 'c'){
                Course course_into_enrollment = parseLineToCourse(line);
                if(course_into_enrollment == NULL)
                    break;
                enrollmentSystem->courses[enrollmentSystem->index_courses] = course_into_enrollment;
                enrollmentSystem->index_courses++;
                }
            else
            {
                Student student_into_enrollment = parseLineToStudent(result);
                if(student_into_enrollment == NULL)
                    break;
                enrollmentSystem->students[enrollmentSystem->index_students] = student_into_enrollment;
                enrollmentSystem->index_students++;
            }
        }
        free(line);
    }

}
