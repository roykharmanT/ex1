#include "HackEnrollment.h"
#include "Student.h"

int main(int argc, char** argv)
{
    bool ignore_case = false;
    char *filename = NULL;
    int cur_arg = 0;
    if(argv[cur_arg] == '-i')
    {
        ignore_case = true;
        ++cur_arg;
    }
    
    FILE* students = fopen(argv[cur_arg++], "r");
    FILE* courses = fopen(argv[cur_arg++], "r");
    FILE* hackers = fopen(argv[cur_arg++], "r");
    FILE* queues = fopen(argv[cur_arg++], "r");
    FILE* target = fopen(argv[cur_arg++], "a");

    EnrollmentSystem sys = createEnrollment(students, courses, hackers);
    sys = readEnrollment(sys, queues);
    hackEnrollment(sys, target);
    destroyEnrollment(sys);

    fclose(students);
    fclose(courses);
    fclose(hackers);
    fclose(queues);
    fclose(target);

    return 0;
}