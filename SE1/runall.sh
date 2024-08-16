gcc -Wall -Wextra -pedantic -std=c99 -O2 -lm simulator.c -o simulator
zcat gcc-1K.trace.gz | ./simulator
