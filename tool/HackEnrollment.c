#include "HackEnrollment.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 100
#define FRIENDSHIP_THRESHOLD 20
#define RIVAL_THRESHOLD 0
#define FRIEND_SCORE 20
#define RIVAL_SCORE -20
#define MIN_COURSES_FULFILLED 2


EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    EnrollmentSystem enrollmentSystem = mallocEnrollmentSystem(students, courses, hackers);
    if(enrollmentSystem == NULL)
        return NULL;
    putStudentInEnrollment(students, enrollmentSystem);
    putCoursesInEnrollment(courses, enrollmentSystem);
    putHackersInEnrollment(hackers, enrollmentSystem);
    adjustHackerToStudent(enrollmentSystem);
    return enrollmentSystem;

}

void putStudentInEnrollment(FILE* students, EnrollmentSystem enrollmentSystem)
{
    if (students != NULL) {
        char *result = NULL;
        int max_line_length = getMaxLineLength(students);
        int max_str_length = getMaxStrLength(students);
        char *student_line = (char *) malloc(max_line_length * sizeof(char));
        if(student_line == NULL)
            return;
        while (true) {
            result = fgets(student_line, max_line_length, students);
            if (result == NULL)
                break;
            Student student_into_enrollment = parseLineToStudent(result, max_str_length);
            if(student_into_enrollment == NULL)
                break;
            enrollmentSystem->students[enrollmentSystem->index_students] = student_into_enrollment;
            enrollmentSystem->index_students++;
        }
        free(student_line);
        rewind(students);
    }
}

void putCoursesInEnrollment(FILE* courses, EnrollmentSystem enrollmentSystem)
{
    if(courses != NULL) {
        char *result = NULL;
        int max_line_length = getMaxLineLength(courses);
        char *course_line = (char *) malloc(max_line_length * sizeof(char));
        if(course_line == NULL)
            return;
        while (true) {
            result = fgets(course_line, max_line_length, courses);
            if (result == NULL)
                break;
            Course course_into_enrollment = parseLineToCourse(course_line);
            enrollmentSystem->courses[enrollmentSystem->index_courses] = course_into_enrollment;
            enrollmentSystem->index_courses++;
        }
        free(course_line);
        rewind(courses);
    }

}


void putHackersInEnrollment(FILE* hackers, EnrollmentSystem enrollmentSystem)
{
    if(hackers == NULL)
        return;
    int max_line_length = getMaxLineLength(hackers);
    char* line = (char *) malloc(max_line_length * sizeof(char));
    if(line == NULL) {
        return;
    }

    int index = 0;
    char* result = NULL;
    while(true){
        result = fgets(line, max_line_length, hackers);
        if(result == NULL)
            break;
        parseLineToHacker(enrollmentSystem->hackers[enrollmentSystem->index_hackers], result, (index % 4));
        index++;
        if(index % 4 == 0)
            enrollmentSystem->index_hackers++;
    }
    rewind(hackers);
}

Course parseLineToCourse(char* line)
{
    Course course = (Course)malloc(sizeof(*course));
    if(course == NULL)
        return NULL;
    FriendshipFunction* friendship_measures = (FriendshipFunction*)malloc(sizeof(FriendshipFunction));
    friendship_measures[0] = NULL;
    course->course_queue = IsraeliQueueCreate(friendship_measures, compareId, FRIENDSHIP_THRESHOLD, RIVAL_THRESHOLD);
    //need to add the friendship functions
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

bool isHackerFriend(Hacker hacker, Student student){
    for(int i = 0; i < hacker->size_friends_id; ++i)
    {
        if(strcmp(hacker->friends_id[i], student->student_id) == 0)
        {
            return true;
        }
    }
    return false;
}

bool isHackerRival(Hacker hacker, Student student){
    for(int i = 0; i < hacker->size_rivals_id; ++i)
    {
        if(strcmp(hacker->rivals_id[i], student->student_id) == 0)
        {
            return true;
        }
    }
    return false;
}

int isFriendOrRival(void* student_hacker, void* void_student){
//Israeli queue friendship measure
    Student student = (Student) void_student;
    Hacker hacker = ((Student)student_hacker)->is_hacker;
    if(isHackerFriend(hacker, student)){
        return FRIEND_SCORE;
    }
    if(isHackerRival(hacker, student))
    {
        return RIVAL_SCORE;
    }
    return 0;
}
int compareId(void* student_hacker, void* student)
{
    if(strcmp(((Student)student_hacker)->student_id, ((Student)student)->student_id) == 0)
        return 1;
    return 0;
}

int nameDifferences(void* void_first, void* void_second)
//Israeli queue friendship measure
{
    Student first = (Student)void_first, second = (Student)void_second;
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


int idDifferences(void* void_first, void* void_second)
//Israeli queue friendship measure
{
    Student first = (Student)void_first, second = (Student)void_second;
    int first_id = 0, second_id = 0;
    for(int i = 0;i < ID_LENGTH; i++){
        first_id = first_id * 10 + first->student_id[i] - '0';
        second_id = second_id * 10 + second->student_id[i] - '0';
    }
    return abs(first_id - second_id);
}

int getMaxLineLength(FILE* file_to_read) {
    int max_line = 0;
    if (file_to_read != NULL) {
        int ch, cnt_line = 0;
        while (true) {
            ch = fgetc(file_to_read);
            if (ch == EOF)
                break;
            if (ch == '\n') {
                if (cnt_line > max_line) {
                    max_line = cnt_line;
                }
                cnt_line = 0;
            } else
                cnt_line++;
        }
        rewind(file_to_read);
    }
    return max_line;
}
int getMaxStrLength(FILE* file_to_read) {
    int max_str = 0;
    if (file_to_read != NULL) {
        int ch, cnt_str = 0;
        while (true) {
            ch = fgetc(file_to_read);
            if (ch == EOF)
                break;
            if (ch == ' ' || ch == '\n') {
                if (cnt_str > max_str) {
                    max_str = cnt_str;
                }
                cnt_str = 0;
            } else
                cnt_str++;

        }
        rewind(file_to_read);
    }
    return max_str;
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
    enrollmentSystem->hackers = (Hacker*)malloc((hackers_length / 4)*sizeof(Hacker));
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


Course getCourse(int course_number, EnrollmentSystem sys){
    for(int k = 0; k < sys->index_courses; ++k){
        Course course = sys->courses[k];
        if(course->course_number == course_number){
            return course;
        }
    }
    return NULL;
}

IsraeliQueueError enqueueHacker(Hacker hacker, EnrollmentSystem sys){
    Student student_hacker = findStudentById(sys, hacker->hacker_id);
    IsraeliQueueError success = ISRAELI_QUEUE_ERROR;
    for(int j = 0; j < hacker->size_desired_courses; ++j){
        int desired_course = hacker->desired_courses[j];
        Course course = getCourse(desired_course, sys);
        if(!course){
            return ISRAELI_QUEUE_ERROR;
        }
        if(course->course_number == desired_course)
        {
            success = IsraeliQueueEnqueue(course->course_queue, student_hacker);
        }
    }
    return success;
}

bool studentInCourse(Student student, Course course){
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

bool hackerGotIn(Hacker hacker, EnrollmentSystem sys){
    Student student_hacker = findStudentById(sys, hacker->hacker_id);
    int required_courses = hacker->size_desired_courses >= 2 ? 2 : 1;
    for(int j = 0; j < hacker->size_desired_courses; ++j){
        Course course = getCourse(hacker->desired_courses[j], sys);
        if(studentInCourse(student_hacker, course)){
            --required_courses;
            if(required_courses == 0){
                return true;
            }
        }
    }
    return false;
}

Hacker hackerLeftOut(EnrollmentSystem sys){
    for(int i = 0; i < sys->index_hackers; ++i){
        Hacker hacker = sys->hackers[i];
        if(!hackerGotIn(hacker, sys)){
            return hacker;
        }
    }
    return NULL;
}

void writeEnrollmentQueue(FILE* out, Course course){
    fprintf(out, "%d", course->course_number);
    Student head = IsraeliQueueDequeue(course->course_queue);
    while(head){
        fprintf(out, " %s", head->student_id);
        head = IsraeliQueueDequeue(course->course_queue);
    }
    fprintf(out, "\n");
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    for(int i = 0; i < sys->index_hackers; ++i)
    {
        Hacker hacker = sys->hackers[i];
        IsraeliQueueError enqueue_success = enqueueHacker(hacker, sys);
        if(enqueue_success != ISRAELIQUEUE_SUCCESS){
            return;
        }
    }

    Hacker left_out = hackerLeftOut(sys);
    if(left_out){
        fprintf(out, "Cannot satisfy constraints for %s", left_out->hacker_id);
        return;
    }

    for(int i = 0; i < sys->index_courses; ++i){
        Course course = sys->courses[i];
        writeEnrollmentQueue(out, course);
    }
}

Student findStudentById(EnrollmentSystem sys, char* id)
{
    for(int i = 0; i < sys->index_students; i++){
        if(strcmp(sys->students[i]->student_id, id))
            return sys->students[i];
    }
    return NULL;
}
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    int max_line = getMaxLineLength(queues);
    char* line = (char*)malloc(max_line * sizeof(char));
    if(line == NULL)
        return NULL;
    char* result = NULL;
    char* token;
    char* space = " ";
    while(true) {
        result = fgets(line, max_line, queues);// read the line
        if(result == NULL)
            break;
        token = strtok(line, space);//read the course number
        int course_num = stringToInt(token);
        Course course = getCourse(course_num, sys);
        while(true){
            token = strtok(NULL, space);//read the ID's
            if(token == NULL)
                break;
            Student student = findStudentById(sys, token);
            if(student == NULL)
                continue;//what happens if the student is not in the enrollment
            IsraeliQueueEnqueue(course->course_queue, student);

        }

    }
    return sys;
}

void adjustHackerToStudent(EnrollmentSystem sys)
{
    for(int i = 0; i < sys->index_hackers; i++)
    {
        Student student = findStudentById(sys, sys->hackers[i]->hacker_id);
        if(student != NULL)
            student->is_hacker = sys->hackers[i];
    }
}

void destroyEnrollment(EnrollmentSystem sys)
{
    for(int i = 0; i < sys->index_students; i++){
        destroyStudent(sys->students[i]);
    }
    for(int i = 0; i < sys->index_courses; i++){
        destroyCourse(sys->courses[i]);
    }
    for(int i = 0; i < sys->index_hackers; i++){
        destroyHacker(sys->hackers[i]);
    }
    free(sys);
}

void destroyCourse(Course course)
{
    IsraeliQueueDestroy(course->course_queue);
    free(course);
}