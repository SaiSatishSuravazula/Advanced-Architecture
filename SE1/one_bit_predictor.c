// Initially assume branch is always taken. We have a branchFile that contains sequence of "T" & "N"'s

#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void simulate(FILE* branchFile)
{
    char TNnotBranch = 'T';
    char mychar;
    int False_pred = 0;
    int True_pred = 0;

    while((mychar = fgetc(branchFile)) != EOF)
    {
        if(TNnotBranch != mychar){
            False_pred++;
            TNnotBranch = mychar;
        }
        else
            True_pred++;
    }

    printf("False predictions: %d\n", False_pred);
    printf("True predictions: %d\n", True_pred);
}

int main(int argc, char *argv[])
{
    FILE *branchFile = stdout;

    if (argc >= 2) {
        branchFile = fopen(argv[1], "r");
        assert(branchFile != NULL);
    }

    simulate(branchFile);
    return 0;
}