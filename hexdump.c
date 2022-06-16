/*
 *      Ferit Yiğit BALABAN <f@fybx.dev>, 2022
 *
 *      Description
 *      simple hexdump with ASCII representation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RES "\x1B[0m"

void print_binary(char);
int get_digits(int);
void print_special(char);

int main(int argc, char* argv[]) {
    FILE* file;
    fpos_t fstart; /* fpos_t struct pointing to start of file */
    long fsize; /* file size  */
    
    int ccount = 4; /* column count */
    long lcount = 0; /* line count */
    int pIter, cIter; /* print iterator & character iterator */
    char* cv; /* character vector */
    char c; /*character read from file */
    int viter; /* argument vector iterator */
    int pformat = -1; /* print format, hex or bin */
    int pctrl = 1; /* print control codes */
    int color = 1; /* print colored output */

    char* argipctrl; /* argument input for pctrl */
    char* argicolor; /* argument input for color */


    argipctrl = calloc(3, sizeof(char));
    argicolor = calloc(3, sizeof(char));
    if (argc == 1) {
        puts("hexdump by fyb."); 
        return(0);
    } 
    for (viter = 2; viter < argc; ++viter) {
        pformat = strcmp("-bin", argv[viter]) == 0 || pformat == 0;
        pformat = strcmp("-hex", argv[viter]) == 1 || pformat == 1; /* last seen -hex or -bin will set pformat */
        if (strstr(argv[viter], "-pctrl=") != NULL) {
            sscanf(argv[viter], "-pctrl=%s", argipctrl);
            pctrl = !strcmp("yes", argipctrl);
        
        } else if (strstr(argv[viter], "-ncol=") != NULL) {
            sscanf(argv[viter], "-ncol=%d", &ccount);
        } else if (strstr(argv[viter], "-color=") != NULL) {
            sscanf(argv[viter], "-color=%s", argicolor);
            color = !strcmp("yes", argicolor);     
        }
    }

    file = fopen(argv[1], "r");
    if (file == NULL) {
        puts("file cannot be opened");
        return(-1);
    }
    fgetpos(file, &fstart);

    cv = calloc(ccount, sizeof(char));
    /* get file size by seeking to end of file and reading position */
    fseek(file, 0L, SEEK_END);
    fsize = ftell(file); /* ftell returns in bytes */
    lcount = fsize / ccount; /* ccount gives count of bytes to print each line */
    
    fsetpos(file, &fstart); /* reset position to start of file */
    for (pIter = 0; pIter < lcount; pIter++) {
        printf(GRN "%*d " RES, get_digits(lcount), pIter + 1);
        for (cIter = 0; cIter < ccount; cIter++) {
            if (!feof(file)) {
                c = fgetc(file);
                *(cv + cIter) = c;
                print_binary(c);
            }
            printf(" ");
        }
        for (cIter = 0; cIter < ccount; cIter++) {
            if (*(cv + cIter) == 0x7F || (0x1F >= *(cv + cIter) && *(cv + cIter) >= 0))
                print_special(*(cv + cIter));
            else
                printf("%c", *(cv + cIter));
        }

        printf("\n");
    }

    free(cv);
    fclose(file);
    return(0);
}

void print_binary(char c) {
    char i = 0;
    for (; i < 8; i++)
        printf("%d", !!((c << i) & 0x80));
}

int get_digits(int n) {
    int c = 0;
    while (n) {
        n /= 10;
        c++;
    }
    return c;
}

void print_special(char c) {
    char* controlCode[32] = {
        "^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G",
        "^H", "^I", "^J", "^K", "^L", "^M", "^N", "^O",
        "^P", "^Q", "^R", "^S", "^T", "^U", "^V", "^W",
        "^X", "^Y", "^Z", "^[", "^\\", "^]", "^^", "^_"
    };
    if (c == 0x7F)
        printf("^?");
    else
        printf(RED "%s" RES, controlCode[(int)c]);
}
