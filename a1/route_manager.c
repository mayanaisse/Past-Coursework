/** @file route_manager.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process airline routes.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Saasha J.
 *  @author Victoria L.
 *  @author STUDENT_NAME
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{
    // TODO: your code.
    if(argc == 0){
        FILE *fp;
            fp = fopen("output.txt", "w");
            fputs("NO RESULTS FOUND.\n", fp);
        fclose(fp);
    }else{
            int i=1;
            printf("%d\n",argc);

        for(i=1;i<argc;i++)
    {
        printf("%s",argv[i]);
    }



    }
    

    exit(0);
}
