#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include "Student.h"
#include <string.h>
#define STUDENTS_FILE_NAME "students.txt"



Student mallocStudent()
{
    Student student = (Student)malloc(sizeof(*Student));
    if (student == NULL) {
        return NULL;
    }
    student->first_name = (char*)malloc(sizeof(char) * MAX_STR_LENGTH);
    if (student->first_name == NULL) {
        free(student);
        return NULL;
    }
    student->surname = (char*)malloc(sizeof(char) * MAX_STR_LENGTH);
    if (student->surname == NULL) {
        free(student->first_name);
        free(student);
        return NULL;
    }
    student->city = (char*)malloc(sizeof(char) * MAX_STR_LENGTH);
    if (student->city == NULL) {
        free(student->surname);
        free(student->first_name);
        free(student);
        return NULL;
    }
    student->department = (char*)malloc(sizeof(char) * MAX_STR_LENGTH);
    if (student->deparment == NULL) {
        free(student->surname);
        free(student->first_name);
        free(student->city);
        free(student);
        return NULL;
    }
    student->student_id = (char*)malloc(sizeof(char) * ID_LENGTH);
    if (student->student_id == NULL) {
        free(student->surname);
        free(student->first_name);
        free(student->city);
        free(student->department);
        free(student);
        return NULL;
    }
    return student;
}




Student parseLineToStudent(char* line)
{
    Student new_student = mallocStudent();
    if(new_student == NULL)
        return NULL;
    char* token = strtok_s(token, line, NULL);
    new_student->student_id = token;
    token = strtok_s(token, NULL, NULL);
    new_student->total_credits = token;
    token = strtok_s(token, NULL, NULL);
    new_student->gpa = token;
    token = strtok_s(token, NULL, NULL);
    new_student->first_name = token;
    token = strtok_s(token, NULL, NULL);
    new_student->surname = token;
    token = strtok_s(token, NULL, NULL);
    new_student->city = token;
    token = strtok_s(token, NULL, NULL);
    new_student->department = token;
    return new_student;
}



