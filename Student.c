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
    student->student_id = (char*)malloc(sizeof(char) * (ID_LENGTH + 1));
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
    char* space = " ";
    char* token = strtok_s(token, line, &space);
    new_student->student_id = _strdup(token);
    token = strtok_s(token, NULL, &space);
    new_student->total_credits = stringToInt(token);
    token = strtok_s(token, NULL, &space);
    new_student->gpa = stringToInt(token);
    token = strtok_s(token, NULL, &space);
    new_student->first_name = _strdup(token);
    token = strtok_s(token, NULL, &space);
    new_student->surname = _strdup(token);
    token = strtok_s(token, NULL, &space);
    new_student->city = _strdup(token);
    token = strtok_s(token, NULL, &space);
    new_student->department = _strdup(token);
    return new_student;
}

int stringToInt(const char* str)
{
    int index = 0, number = 0;
    while(str[index]){
        number = (number*10) + (str[index] - '0');
        index++;
    }
    return number;
}


void destroyStudent(Student student)
{
    free(student->student_id);
    free(student->surname);
    free(student->first_name);
    free(student->city);
    free(student->department);
    free(student);
}

Hacker mallocHacker(int num_of_courses, int num_of_friends, int num_of_rivals)
{
    Hacker new_hacker = (Hacker)malloc(sizeof(*Hacker));
    if(new_hacker == NULL)
        return NULL;
    new_hacker->hacker_id = (char*)malloc((ID_LENGTH + 1)*sizeof(char));
    if(new_hacker->hacker_id == NULL){
        free(new_hacker);
        return NULL;
    }
    new_hacker->desired_courses = (int*)malloc(num_of_courses*sizeof (int));
    if(new_hacker->desired_courses == NULL){
        free(new_hacker->hacker_id);
        free(new_hacker);
        return NULL;
    }
    new_hacker->friends_id = (char**)malloc(num_of_friends* sizeof(char*));
    if(new_hacker->friends_id != NULL){
        for(int i = 0; i < num_of_friends; i++;){
            new_hacker->friends_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
            if(new_hacker->friends_id[i] == NULL){
                for(int j = 0; j < i; j++){
                    free(new_hacker->friends_id[j]);
                }
                free(new_hacker->hacker_id);
                free(new_hacker);
                return NULL;
            }
        }
    }

    new_hacker->rivals_id = (char**)malloc(num_of_rivals* sizeof(char*));
    if(new_hacker->rivals_id != NULL){
        for(int i = 0; i < num_of_rivals; i++;){
            new_hacker->rivals_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
            if(new_hacker->rivals_id[i] == NULL){
                for(int j = 0; j < i; j++){
                    free(new_hacker->rivals_id[j]);
                }
                for(int k = 0; k < num_of_friends; k++){
                    free(new_hacker->friends_id[k]);
                }
                free(new_hacker->hacker_id);
                free(new_hacker);
                return NULL;
            }
        }
    }
    return new_hacker;
    //its pretty disgusting but it is what it is
}

void destroyHacker(Hacker hacker)
{

}

void parseLineToHacker(Hacker hacker, char* line, int line_number)
{
    if(line[0] != '\n'){
        char* space = " ";
        char* token;
        switch (line_number)
        {
            case 0:
                token = strtok(line, space);
                hacker->hacker_id = strdup(token);
                break;
            case 1:
                int num_of_courses = getNumOfStringsInTheLine(line);
                hacker->desired_courses = (int*)malloc(num_of_courses * sizeof(int));
                if(hacker->desired_courses == NULL)
                    destroyHacker(hacker);
                break;
            case 2:
                bool success;
                int num_of_friends = getNumOfStringsInTheLine(line);
                success = mallocHackerFriendsOrRivals(hacker, num_of_friends, 'f');
                if(!success) {
                    destroyHacker(hacker);
                    return;
                }
                for(int i = 0;i < num_of_friends; i++){
                    token = strtok(line, space);//need to progress the line
                    hacker->friends_id[i] = strdup(token);
                }

        }
    }
}

bool mallocHackerFriendsOrRivals(Hacker hacker, int num, char type)
{
    if(type == 'f')
    {
        hacker->friends_id = (char **) malloc(num * sizeof(char *));
        if (hacker->friends_id != NULL) {
            for (int i = 0; i < num; i++;){
                hacker->friends_id[i] = (char *) malloc((ID_LENGTH + 1) * sizeof(char));
                if (hacker->friends_id[i] == NULL) {
                    destroyHacker(hacker);
                    return false;
                }
            }
        }
        else {
            destroyHacker(hacker);
            return false;
        }
    }
    //if we want to allocate rivals
    else
    {
        hacker->rivals_id = (char**)malloc(num * sizeof(char*));
        if(hacker->rivals_id != NULL){
            for(int i = 0; i < num; i++;){
                new_hacker->rivals_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
                if(new_hacker->rivals_id[i] == NULL){
                    destroyHacker(hacker);
                    return false;
                }
            }
        }
        else {
            destroyHacker(hacker);
            return false;
        }
    }
    return true;
}