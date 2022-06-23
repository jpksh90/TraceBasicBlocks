
//
// Created by Jyoti Prakash on 14.12.20.
//

#include <stdio.h>
int bbCounter=0;
extern void writeBBInfo();
extern void incrementCounter();

void writeBBInfo() {
    FILE *fp = fopen("basicblock.txt", "a");
    fprintf(fp, "Number of basic blocks executed: %d\n", bbCounter);
    fclose(fp);
}

void incrementCounter() {
  bbCounter = bbCounter+1;
}
