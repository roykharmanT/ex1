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
    new_student->total_credits = stringToInt(token);
    token = strtok(NULL, space);
    new_student->gpa = stringToInt(token);
    token = strtok(NULL, space);
    new_student->first_name = strdup(token);
    token = strtok(NULL, space);
    new_student->surname = strdup(token);
    token = strtok(NULL, space);
    new_student->city = strdup(token);
    token = strtok(NULL, space);
    new_student->department = strdup(token);
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
// I think we don't really need it
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

void parseLineToHacker(Hacker hacker, char* line, int line_number)
{
    if(line[0] != '\n'){
        char* space = " ";
        char* token;
        bool success;
        switch (line_number)
        {
            case 0:
                token = strtok(line, space);
                hacker->hacker_id = strdup(token);
                break;
            case 1:
                int num_of_courses = getNumOfStringsInTheLine(line);
                hacker->desired_courses = (int*)malloc(num_of_courses * sizeof(int));
                if(hacker->desired_courses == NULL) {
                    destroyHacker(hacker);
                    return;
                }
                else{
                    putCoursesLineIntoHacker(hacker, line);
                }
                break;
            case 2:
                int num_of_friends = getNumOfStringsInTheLine(line);
                success = mallocHackerFriendsOrRivals(hacker, num_of_friends, 'f');
                if(!success) {
                    destroyHacker(hacker);
                    return;
                }
                putLineInIdArray(hacker, line, 'f');
                break;
            case 3:
                int num_of_rivals = getNumOfStringsInTheLine(line);
                success = mallocHackerFriendsOrRivals(hacker, num_of_rivals, 'r');
                if(!success) {
                    destroyHacker(hacker);
                    return;
                }
                putLineInIdArray(hacker, line, 'r');
                break;
            default:
                return;
        }


        }
    }
}

void putCoursesLineIntoHacker(Hacker hacker, char* line)
{
    char temp[7];
    int index = 0, current_letter = 0, current_course = 0;
    while(line[index] != '\n'){
        if(line[index] == ' '){
            hacker->desired_courses[current_course] = stringToInt(temp);
            current_course++;
            current_letter = 0;
        }
        else {
            temp[current_letter] = line[index];
            current_letter++;
        }
        index++;
    }
}
void putLineInIdArray(Hacker hacker, char* line, char type)
{
    int index = 0, current_item = 0, current_ch = 0 ;
    while(line[index] != '\n'){
        if (line[index] == ' ') {
            current_item++;
            current_ch = 0;
        }
        else {
            if (type == 'f') {
                hacker->friends_id[current_item][current_ch] = line[index];
            }
            else {
                hacker->rivals_id[current_item][current_ch] = line[index];
            }
            current_ch++;
        }
        index++;
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