#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAXFNAME 100

char* get_input_file(int argc, char **argv, char *fname, int flen, char *day)
{
    char flag = '\0',
         *rel_path = "../inputs/",
         *test = "test-",
         *extension = ".txt";

    strcpy(fname, rel_path);

    while (--argc && (*++argv)[0] == '-')
        switch ((flag = (*argv)[1])) {
        case 't':
            strcat(fname, test);
            break;
        case 'i':
            // use default input
            break;
        case 'f':
            assert(argc > 1);
            strcpy(fname, argv[1]);
            return fname;
        default:
            printf("Unrecognized flag: %c\n", flag);
            fname[0] = '\0';
            return fname;
        }

    strcat(fname, "input");
    strcat(fname, day);
    strcat(fname, extension);
    return fname;
}

int main(int argc, char* argv[])
{
    printf("Command Line Args\n");

    for (int i = 0; i < argc; ++i)
        printf("%s\n", argv[i]);

    char fname[MAXFNAME];
    get_input_file(argc, argv, fname, MAXFNAME, "07");
    printf("File name: %s\n", fname);
}
