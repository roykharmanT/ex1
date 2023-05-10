#include "HackEnrollment.h"
#include "Student.h"
#include <string.h>


static char toLower(char c){
    if(c >= 'A' && c <= 'Z'){
        return c - ('a' - 'A');
    }
    return c;
}

static void lowerNames(EnrollmentSystem sys){
    for(int i = 0; i < sys->index_students; ++i){
        Student student = sys->students[i];
        for(int j = 0; student->first_name[j]; ++j){
            student->first_name[j] = toLower(student->first_name[j]);
        }

        for(int j = 0; student->surname[j]; ++j){
            student->surname[j] = toLower(student->surname[j]);
        }
    }
}

int main(int argc, char** argv)
{
    int cur_arg = 1;
    bool ignore_caps = false;
    if(strcmp(argv[cur_arg], "-i") == 0){
        ignore_caps = true;
        cur_arg++;
    }
    FILE* students = fopen(argv[cur_arg++], "r");
    FILE* courses = fopen(argv[cur_arg++], "r");
    FILE* hackers = fopen(argv[cur_arg++], "r");
    FILE* queues = fopen(argv[cur_arg++], "r");
    FILE* target = fopen(argv[cur_arg++], "a");

    EnrollmentSystem sys = createEnrollment(students, courses, hackers);
    sys = readEnrollment(sys, queues);
    
    if(ignore_caps){
        lowerNames(sys);
    }
    
    hackEnrollment(sys, target);
    destroyEnrollment(sys);

    fclose(students);
    fclose(courses);
    fclose(hackers);
    fclose(queues);
    fclose(target);

    return 0;
}