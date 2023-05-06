#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 100
#define FRIENDSHIP_THRESHOLD 20
#define RIVAL_THRESHOLD 0
#define FRIEND_SCORE 20
#define RIVAL_SCORE -20


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
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

}

Course parseLineToCourse(char* line)
{
    Course course = (Course)malloc(sizeof (*course));
    if(course == NULL)
        return NULL;
    FriendshipFunction* friendship_measures = (FriendshipFunction*)malloc(sizeof(FriendshipFunction));
    friendship_measures[0] = NULL;
    course->course_queue = IsraeliQueueCreate(friendship_measures, compare_id, FRIENDSHIP_THRESHOLD, RIVAL_THRESHOLD);
    if(course->course_queue == NULL){
        free(course);
        return NULL;
    }
    char* space = " ";
    char* token = strtok(line, space);
    course->course_number = stringToInt(token);
    token = strtok(NULL, space);
    course->size = stringToInt(token);
    return course;
}

bool is_hacker_friend(Hacker hacker, Student student){
    for(int i = 0; i < hacker->size_friends_id; ++i)
    {
        if(strcmp(hacker->friends_id[i], student->student_id) == 0)
        {
            return true;
        }
    }
    return false;
}

bool is_hacker_rival(Hacker hacker, Student student){
    for(int i = 0; i < hacker->size_rivals_id; ++i)
    {
        if(strcmp(hacker->rivals_id[i], student->student_id) == 0)
        {
            return true;
        }
    }
    return false;
}

int is_friend_or_rival(Hacker hacker, Student student){
//Israeli queue friendship measure
    if(is_hacker_friend(hacker, student)){
        return FRIEND_SCORE;
    }
    if(is_hacker_rival(hacker, student))
    {
        return RIVAL_SCORE;
    }
    return 0;
}

int compare_id(Hacker hacker, Student student)
//Israeli queue comparison function
{
    if(strcmp(hacker->hacker_id, student->student_id) == 0)
        return 1;
    return 0;
}

int nameDifferences(Student first, Student second)
//Israeli queue friendship measure
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
//Israeli queue friendship measure
{
    int index = 0, first_id = 0, second_id = 0;
    for(int i = 0;i < ID_LENGTH; i++){
        first_id = first_id * 10 + first->student_id[i] - '0';
        second_id = second_id * 10 + second->student_id[i] - '0';
    }
    return abs(first_id - second_id);
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

EnrollmentSystem mallocEnrollmentSystem(FILE* students, FILE* courses, FILE* hackers)
{
    int students_length, courses_length, hackers_length;
    students_length = getNumOfLines(students);
    courses_length = getNumOfLines(courses);
    hackers_length = getNumOfLines(hackers);
    EnrollmentSystem enrollmentSystem = (EnrollmentSystem)malloc(sizeof(*enrollmentSystem));
    if(enrollmentSystem == NULL)
        return NULL;
     enrollmentSystem->students = (Student*)malloc(students_length*sizeof(Student));
    if(enrollmentSystem->students == NULL) {
        free(enrollmentSystem);
        return NULL;
    }
    enrollmentSystem->courses = (Course*)malloc(courses_length*sizeof(Course));
    if(enrollmentSystem->courses == NULL) {
        free(enrollmentSystem->students);
        free(enrollmentSystem);
        return NULL;
    }
    enrollmentSystem->hackers = (Hacker*)malloc(hackers_length*sizeof(Hacker));
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


Student find_student_hacker(Hacker hacker, EnrollmentSystem sys)
{
    for(int i = 0; i < sys->index_students; ++i){
        Student student = sys->students[i];
        if(strcmp(student->student_id, hacker->hacker_id) == 0){
            return student;
        }
    }
}

Course get_course(int course_number, EnrollmentSystem sys){
    for(int k = 0; k < sys->index_courses; ++k){
        Course course = sys->courses[k];
        if(course->course_number == course_number){
            return course;
        }
    }
    return NULL;
}

IsraeliQueueError enqueue_hacker(Hacker hacker, EnrollmentSystem sys){
    Student student_hacker = find_student_hacker(hacker, sys);
    IsraeliQueueError success = ISRAELI_QUEUE_ERROR;
    for(int j = 0; j < hacker->size_desired_courses; ++j){
        int desired_course = hacker->desired_courses[j];
        Course course = get_course(desired_course, sys);
        assert(course);
        if(course->course_number == desired_course)
        {
            IsraeliQueueError success = IsraeliQueueEnqueue(course->course_queue, student_hacker);
        }
    }
    return success;
}

bool student_in_course(Student student, Course course){
    int counter = 0;
    IsraeliQueue checking_queue = IsraeliQueueClone(course->course_queue);
    while(counter < course->size){
        Student head = IsraeliQueueDequeue(checking_queue);
        if(head == student){
            IsraeliQueueDestroy(checking_queue);
            return true;
        }
        ++counter;
    }
    IsraeliQueueDestroy(checking_queue);
    return false;

}

bool hacker_got_in(Hacker hacker, EnrollmentSystem sys){
    Student student_hacker = find_student_hacker(hacker, sys);
    int required_courses = hacker->size_desired_courses >= 2 ? 2 : 1;
    for(int j = 0; j < hacker->size_desired_courses; ++j){
        Course course = get_course(hacker->desired_courses[j], sys);
        if(student_in_course(student_hacker, course)){
            --required_courses;
            if(required_courses == 0){
                return true;
            }
        }
    }
    return false;
}

Hacker hacker_left_out(EnrollmentSystem sys){
    for(int i = 0; i < sys->index_hackers; ++i){
        Hacker hacker = sys->hackers[i];
        if(!hacker_got_in(hacker, sys)){
            return hacker;
        }
    }
    return NULL;
}

void write_enrollment_queue(FILE* out, Course course){
    fprintf(out, "%d", course->course_number);
    Student head = IsraeliQueueDequeue(course->course_queue);
    while(head){
        fprintf(out, " %s", head);
        head = IsraeliQueueDequeue(course->course_queue);
    }
    fprintf(out, "\n");
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    for(int i = 0; i < sys->index_hackers; ++i)
    {
        Hacker hacker = sys->hackers[i];
        IsraeliQueueError enqueue_success = enqueue_hacker(hacker, sys);
        assert(enqueue_success == ISRAELIQUEUE_SUCCESS);
    }

    Hacker left_out = hacker_left_out(sys);
    if(left_out){
        fprintf("Cannot satisfy constraints for %s", left_out->hacker_id);
        return;
    }
    for(int i = 0; i < sys->index_courses; ++i){
        Course course = sys->courses[i];
        write_enrollment_queue(out, course);
    }
}