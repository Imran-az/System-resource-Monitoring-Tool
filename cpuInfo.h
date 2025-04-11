#ifndef cpuInfo_header
#define cpuInfo_header
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>

struct CPUinformation {
    double currentSum;
    double currentIdleTime;
    double cpuPercentage;
    double prevSum;
    double prevIdleTime;
};

struct CPUinformation processCpuStruct(struct CPUinformation* cpuinfo, char *sample);
struct CPUinformation CpuInfoStruct(struct CPUinformation *cpuData);
void createCPUGraph(int samples, int tdelay);
void populateCPUgraph(int samples, int tdelay, int xscale, double ChangeInCPU, int row, int count);


#endif