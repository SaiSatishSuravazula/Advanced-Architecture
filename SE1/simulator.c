#define __STDC_FORMAT_MACROS
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void simulate(FILE* inputFile, FILE* outputFile, FILE* branchFile, FILE* branchInstFile)
{
  // See the documentation to understand what these variables mean.
  int32_t microOpCount;
  uint64_t instructionAddress;
  int32_t sourceRegister1;
  int32_t sourceRegister2;
  int32_t destinationRegister;
  char conditionRegister;
  char TNnotBranch;
  char loadStore;
  int64_t immediate;
  uint64_t addressForMemoryOp;
  uint64_t fallthroughPC;
  uint64_t targetAddressTakenBranch;
  char macroOperation[12];
  char microOperation[23];

  int64_t totalMicroops = 0;
  int64_t totalMacroops = 0;
  int64_t loadStorecount = 0;
  int64_t branch_taken_count = 0;
  int64_t branch_nt_count = 0; 
  int64_t uncon_jump_count = 0;
  int64_t alu_count = 0;
 
  
  fprintf(outputFile, "Processing trace...\n");
  
  while (true) {
    int result = fscanf(inputFile, 
                        "%" SCNi32
                        "%" SCNx64 
                        "%" SCNi32
                        "%" SCNi32
                        "%" SCNi32
                        " %c"
                        " %c"
                        " %c"
                        "%" SCNi64
                        "%" SCNx64
                        "%" SCNx64
                        "%" SCNx64
                        "%11s"
                        "%22s",
                        &microOpCount,
                        &instructionAddress,
                        &sourceRegister1,
                        &sourceRegister2,
                        &destinationRegister,
                        &conditionRegister,
                        &TNnotBranch,
                        &loadStore,
                        &immediate,
                        &addressForMemoryOp,
                        &fallthroughPC,
                        &targetAddressTakenBranch,
                        macroOperation,
                        microOperation);
                        
    if (result == EOF) {
      break;
    }

    if (result != 14) {
      fprintf(stderr, "Error parsing trace at line %" PRIi64 "\n", totalMicroops);
      abort();
    }

    // For each micro-op
    totalMicroops++;

    // For each macro-op:
    if (microOpCount == 1) {
      totalMacroops++;
    }

    switch(loadStore)
    {
      case '-':
          switch(TNnotBranch)
          {
            case 'T':
            if(conditionRegister == 'R'){
              branch_taken_count++;
              fprintf(branchFile, "T");
              fprintf(branchInstFile, "%" PRIi64 "  %" PRIi64 " T\n", instructionAddress, targetAddressTakenBranch);
            }
            else
              uncon_jump_count++;
            break;

            case 'N':
              branch_nt_count++;
              fprintf(branchFile, "N");
              fprintf(branchInstFile, "%" PRIi64 "  %" PRIi64 " N\n", instructionAddress, targetAddressTakenBranch);
              break;

            default:
              alu_count++;
              break;
          }
          break;

      default:
        loadStorecount++;
        break;
    }
  }
  
  fprintf(outputFile, "Processed %" PRIi64 " trace records.\n", totalMicroops);
  fprintf(outputFile, "Micro-ops: %" PRIi64 "\n", totalMicroops);
  fprintf(outputFile, "Macro-ops: %" PRIi64 "\n", totalMacroops);
  fprintf(outputFile, "Load&Store: %" PRIi64 "\n", loadStorecount);
  fprintf(outputFile, "Branch Taken: %" PRIi64 "\n", branch_taken_count);
  fprintf(outputFile, "Branch NotTaken: %" PRIi64 "\n", branch_nt_count);
  fprintf(outputFile, "Unconditional Jump: %" PRIi64 "\n", uncon_jump_count);  
  fprintf(outputFile, "ALU: %" PRIi64 "\n", alu_count);
}

int main(int argc, char *argv[]) 
{
  FILE *inputFile = stdin;
  FILE *outputFile = stdout;
  FILE *branchFile = stdout;
  FILE *branchInstFile = stdout;
  
  if (argc >= 2) {
    inputFile = fopen(argv[1], "r");
    assert(inputFile != NULL);
  }
  if (argc >= 3) {
    outputFile = fopen(argv[2], "w");
    assert(outputFile != NULL);

    branchFile = fopen(argv[3], "w");
    assert(branchFile != NULL);

    branchInstFile = fopen(argv[4], "w");
    assert(branchInstFile != NULL);
  }
  
  simulate(inputFile, outputFile, branchFile, branchInstFile);
  return 0;
}