#ifndef EX1_STUDENT_H
#define EX1_STUDENT_H

#define ID_LENGTH 9
#define MAX_STR_LENGTH 30


typedef struct Hacker{
    char* hacker_id;
    int* desired_courses;
    int size_desired_courses;
    char** friends_id;
    int size_friends_id;
    char** rivals_id;
    int size_rivals_id;
}*Hacker;

typedef struct Student{
    char* student_id;
    int total_credits;
    int gpa;
    char* first_name;
    char* surname;
    char* city;
    char* department;
    Hacker is_hacker;
}*Student;




//parse line data into student
Student parseLineToStudent(char* line, int max_str_length);
//parse line data into hacker
void parseLineToHacker(Hacker hacker, char* line, int line_number);

//allocates new student
Student mallocStudent(int max_str_length);
//allocates new hacker
Hacker mallocHacker(int num_of_courses, int num_of_friends, int num_of_rivals);
//allocates an array of friends or rivals id's
void mallocHackerFriendsOrRivals(Hacker hacker, int num, char type);
//put the line in the hackers friends or rivals array
void putLineInIdArray(Hacker hacker, char* line, char type);
//put the desired courses in the hackers courses array
void putCoursesLineIntoHacker(Hacker hacker, char* line);

void destroyStudent(Student student);

int stringToInt(char* str);


#endif //EX1_STUDENT_H
