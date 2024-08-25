// Initially assume branch is strongly not taken. We have a branchFile that contains sequence of "T" & "N"'s

#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

typedef enum {NN, NT, TN, TT} state;


//Initially we are at strongly not taken state
state curr_state = NN;

/*
NN - Strongly Not taken
NT - Weakly Not taken
TN - Weakly taken
TT - Strongly taken

Taken or Not Taken is predicted based on the first(previous) character
*/

void simulate(FILE* branchFile)
{
    // Predict based on current state
    char TNnotBranch;
    char mychar;
    int False_pred = 0;
    int True_pred = 0;

    while((mychar = fgetc(branchFile)) != EOF)
    {
        TNnotBranch = (curr_state == TT || curr_state == TN) ? 'T' : 'N';
        if(TNnotBranch != mychar){
            False_pred++;
        }
        else
            True_pred++;

        // Update state based on actual outcome
        switch (curr_state) {
            case NN:
                curr_state = (mychar == 'T') ? NT : NN; // Move to NT if taken
                break;
            case NT:
                curr_state = (mychar == 'T') ? TT : NN; // Move to TT if taken, NN if not taken
                break;
            case TN:
                curr_state = (mychar == 'T') ? TT : NN; // Move to TT if taken, NN if not taken
                break;
            case TT:
                curr_state = (mychar == 'T') ? TT : TN; // Stay in TT if taken, move to TN if not taken
                break;
        }

    }

    printf("False predictions: %d\n", False_pred);
    printf("True predictions: %d\n", True_pred);
    printf("Accuracy: %f percentage\n", ((float) True_pred/(True_pred + False_pred))*100);

}

int main(int argc, char *argv[])
{
    FILE *branchFile = stdin;

    if (argc >= 2) {
        branchFile = fopen(argv[1], "r");
        assert(branchFile != NULL);
    }

    simulate(branchFile);
    return 0;
}