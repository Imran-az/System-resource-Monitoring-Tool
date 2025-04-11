#ifndef coresInfo_header
#define coresInfo_header
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>


int coresInfo();
double coreGHz();
int createCoresGraph(int samples, int tdelay, int row, int cores, double ghz);

#endif