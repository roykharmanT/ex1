#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include "Student.h"
#include <string.h>
#define STUDENTS_FILE_NAME "students.txt"



Student mallocStudent(int max_str_length)
{
    Student student = (Student)malloc(sizeof(*student));
    if (student == NULL) {
        return NULL;
    }
    student->first_name = (char*)malloc(sizeof(char) * max_str_length);
    if (student->first_name == NULL) {
        free(student);
        return NULL;
    }
    student->surname = (char*)malloc(sizeof(char) * max_str_length);
    if (student->surname == NULL) {
        free(student->first_name);
        free(student);
        return NULL;
    }
    student->city = (char*)malloc(sizeof(char) * max_str_length);
    if (student->city == NULL) {
        free(student->surname);
        free(student->first_name);
        free(student);
        return NULL;
    }
    student->department = (char*)malloc(sizeof(char) * max_str_length);
    if (student->department == NULL) {
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




Student parseLineToStudent(char* line, int max_str_length)
{
    Student new_student = mallocStudent(max_str_length);
    if(new_student == NULL)
        return NULL;
    char* space = " ";
    char* token = strtok(line, space);
    new_student->student_id = strdup(token);
    token = strtok(NULL, space);
    new_student->total_credits = atoi(token);
    token = strtok(NULL, space);
    new_student->gpa = atoi(token);
    token = strtok(NULL, space);
    new_student->first_name = strdup(token);
    token = strtok(NULL, space);
    new_student->surname = strdup(token);
    token = strtok(NULL, space);
    new_student->city = strdup(token);
    token = strtok(NULL, space);
    new_student->department = strdup(token);
    new_student->is_hacker = NULL;
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
    if(student)
    {
        if(student -> student_id)
            free(student->student_id);
        if(student -> surname)
            free(student->surname);
        if(student->first_name)
            free(student->first_name);
        if(student->city)
            free(student->city);
        if(student->department)
            free(student->department);
        free(student);
    }
}
// I think we don't really need it (actually we do need it... A wise man once told me 'every object under the heaven must have a constructor and a destructor')
Hacker mallocHacker(int num_of_courses, int num_of_students)
{
    Hacker new_hacker = (Hacker)malloc(sizeof(*new_hacker));
    if(new_hacker == NULL)
        return NULL;
        
    new_hacker->hacker_id = (char*)malloc((ID_LENGTH + 1)*sizeof(char));
    if(new_hacker->hacker_id == NULL){
        destroyHacker(new_hacker);
        return NULL;
    }

    new_hacker->desired_courses = (int*)malloc(num_of_courses*sizeof (int));
    if(new_hacker->desired_courses == NULL){
        destroyHacker(new_hacker);
        return NULL;
    }
    
    new_hacker->friends_id = (char**)malloc(num_of_students*sizeof(char*));
    if(new_hacker->friends_id != NULL){
        for(int i = 0; i < num_of_students; i++){
            new_hacker->friends_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
            if(new_hacker->friends_id[i] == NULL){
                destroyHacker(new_hacker);
                return NULL;
            }
        }
    }

    new_hacker->rivals_id = (char**)malloc(num_of_students*sizeof(char*));
    if(new_hacker->rivals_id != NULL){
        for(int i = 0; i < num_of_students; i++){
            new_hacker->rivals_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
            if(new_hacker->rivals_id[i] == NULL){
                destroyHacker(new_hacker);
                return NULL;
            }
        }
    }
    return new_hacker;
}

void destroyHacker(Hacker hacker)
{
    if(hacker){
        if(hacker->hacker_id)
            free(hacker->hacker_id);
        if(hacker->desired_courses)
            free(hacker->desired_courses);
        if(hacker->friends_id){
            for(int i = 0; i < hacker->size_friends_id; i++){
                if(hacker->friends_id[i])
                    free(hacker->friends_id[i]);
                else
                    break;
            }
        }
        if(hacker->rivals_id){
            for(int i = 0; i < hacker->size_rivals_id; i++){
                if(hacker->rivals_id[i])
                    free(hacker->rivals_id[i]);
                else
                    break;
            }
        }
        free(hacker);
    }
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

void parseLineToHacker(Hacker hacker, char* line, int line_number)
{
    if(line[0] != '\n'){
        char* space = " ";
        char* token;
        switch (line_number)
        {
            case 0: ;
                token = strtok(line, space);
                token[strcspn(token, "\n")] = 0;
                hacker->hacker_id = strdup(token);
                break;
            case 1: ;
                putCoursesLineIntoHacker(hacker, line);
                break;
            case 2: ;
                putLineInIdArray(hacker, line, 'f');
                break;
            case 3: ;
                putLineInIdArray(hacker, line, 'r');
                break;
            default:
                return;
        }

    }
}

void putCoursesLineIntoHacker(Hacker hacker, char* line)
{
    int index = 0;
    char* space = " ";
    char* token = strtok(line, space);
    while(token){
        token[strcspn(token, "\n")] = 0;
        hacker->desired_courses[index++] = atoi(token);
        token = strtok(NULL, space);
    }
    hacker->size_desired_courses = index;
}
void putLineInIdArray(Hacker hacker, char* line, char type)
{
    int index = 0;
    char* space = " ";
    char* token = strtok(line, space);
    while(token){
        token[strcspn(token, "\n")] = 0;
        if (type == 'f') {
            hacker->friends_id[index++] = strdup(token);
        }
        else {
            hacker->rivals_id[index++] = strdup(token);
        }
        token = strtok(NULL, space);
    }
}

bool mallocHackerFriendsOrRivals(Hacker hacker, int num, char type)
{
    if(type == 'f')
    {
        hacker->friends_id = (char **) malloc(num * sizeof(char *));
        if (hacker->friends_id) {
            for (int i = 0; i < num; i++){
                hacker->friends_id[i] = (char *) malloc((ID_LENGTH + 1) * sizeof(char));
                //hacker->friends_id[i] = char[ID_LENGTH +1];-- doesn't work
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
            for(int i = 0; i < num; i++){
                hacker->rivals_id[i] = (char *)malloc((ID_LENGTH + 1)* sizeof(char));
                if(hacker->rivals_id[i] == NULL){
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