#ifndef memInfo_header
#define memInfo_header
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>


void printInfo(int samples, int tdelay);
int processMemory(char *sample);
float MemoryInfo();
void populateMemGraph(int samples, int tdelay, int xscale, int row, double total_Memory, double val);
double totalMemory();



#endif