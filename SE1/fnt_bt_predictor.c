/*Branch forward predict always not taken and branch backward predict always taken*/

#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void simulate(FILE* branchInstFile)
{       

    uint64_t instructionAddress;
    uint64_t targetAddressTakenBranch;
    char TNnotBranch;

    char mypred;
    int true_pred=0;
    int false_pred=0;

    while(true){
        int result = fscanf(branchInstFile,
                            "%" SCNi64
                            "%" SCNi64
                            " %c",
                            &instructionAddress,
                            &targetAddressTakenBranch,
                            &TNnotBranch);

        if(result == EOF)
            break;

        mypred = (instructionAddress < targetAddressTakenBranch)?'N':'T';

        if(mypred != TNnotBranch)       
            false_pred++;
        else
            true_pred++;
    }

    printf("False predictions: %d\n", false_pred);
    printf("True predictions: %d\n", true_pred);
    printf("Accuracy: %f percentage\n", ((float) true_pred/(true_pred + false_pred))*100);

}

int main(int argc, char *argv[]) 
{
  FILE *branchInstFile = stdin;
  
  if (argc >= 2) {
    branchInstFile = fopen(argv[1], "r");
    assert(branchInstFile != NULL);
  }
  
  simulate(branchInstFile);
  return 0;
}

