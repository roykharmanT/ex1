#include "HackEnrollment.h"
#include "Student.h"
#include <string.h>

int main(int argc, char** argv)
{
    int cur_arg = 0;
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