
//
// Created by Jyoti Prakash on 14.12.20.
//

#include <stdio.h>

extern void writeBBInfo(char*, char*);

void writeBBInfo(char* file, char* blkinfo) {
    FILE *fp = fopen(file, "a");
    fprintf(fp, "%s\n", blkinfo);
    fclose(fp);
}
