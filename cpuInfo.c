#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "cpuInfo.h"
#include "memInfo.h"

//processes cpu data and returns it in a struct
///_|> descry: This Function writes all the necessary information on CPU to the CPU struct
///_|> samples: Indicates the line we are reading from in the proc/stat file
///_|> returning: returns a struct CPUinformation type, as we are updating our struct CPUinformation
struct CPUinformation processCpuStruct(struct CPUinformation* cpuinfo, char *sample){
    char *token = strtok(sample, " ");
    token = strtok(NULL, " ");
    long sum = 0.0;
    double cpu_percentage = 0.0;
    long idleTime = 0.0;
    double fraction = 0.0;
    int idleTime_key = 3;
    int count = 0;

    while (token != NULL){
        if (count == idleTime_key){
            idleTime = strtol(token, NULL, 10);
        }
        long cpu_num = strtol(token, NULL, 10);
        sum += cpu_num;
        token = strtok(NULL, " ");
        count++;
    }
    cpuinfo -> currentSum = sum;
    cpuinfo -> currentIdleTime = idleTime;

    fraction = (double)(sum - idleTime)/(double)sum;
    cpu_percentage = fraction * 100;
    cpuinfo -> cpuPercentage = cpu_percentage;
    return *cpuinfo;
}

//Function to get all cpu information and return info in struct
///_|> descry: This Function reads from proc/stat file
///_|> cpuData: represents the struct we want to save the data into
///_|> returning: returns a struct CPUinformation type, as we are updating our struct CPUinformation
struct CPUinformation CpuInfoStruct(struct CPUinformation *cpuData){
    char line[256];
    int lineNum = 1;
    double cpuVal = 0.9;
 
    FILE *file = fopen("/proc/stat", "r");
 
     if (file == NULL){
         printf("Error opening file\n");
         exit(1);
     }
        while(fgets(line, sizeof(line), file)){
            if (lineNum == 1){
                processCpuStruct(cpuData, line);
                cpuVal = cpuData -> cpuPercentage;
                break;
            }
            else{
                lineNum++;
            }
        }
     fclose(file);
     return *cpuData;
 }

///_|> descry: This Function creates our CPU graph
///_|> samples: Indicates the number of samples the program should run for used for the axises
///_|> tdelay: Indicates the delay for which each sample/iteration should run for used for the axises
///_|> returning: returns a void type. This function is used just for creating the CPU graph
void createCPUGraph(int samples, int tdelay){
    int cpuTotal = 100;

    printf("\x1b[%d;%df", 19, 1);
    printf("%d %% |", cpuTotal);
    for (int i = 0; i < 11; i++){
        printf("\x1b[%d;%df", 4 + i, 10);
        printf("|\n");
    }

    printf("\x1b[%d;%df", 14, 1);
    printf("0 GB");
    printf("\x1b[%d;%df", 14, 11);
    for (int i = 0; i < samples; i++){
        printf("_");
    }
    printf("\n\n");
}

///_|> descry: This function plots a single point on the CPU graph
///_|> samples: Indicates the number of samples the program should run for
///_|> tdelay: Indicates the delay for which each sample/iteration should run for
///_|> xscale: Represents the current position on the x axis
///_|> row: Represents the current row we are on
///_|> total_Memory: Represents the total memory obtained from the proc/meminfo file
///_|> ChangeInCPU: Represents the value we want to plot on our grpah
///_|> count: Represents what iteration we are on relative to the samples
///_|> returning: returns a void type. This function is used just for printing a point on the CPU graph
void populateCPUgraph(int samples, int tdelay, int xscale, double ChangeInCPU, int row, int count){
    printf("\033[H");
    //divide each section for y-axis
    int xScaleCounter = xscale;
    double currentSum = 0.0;
    double currentIdletime = 0.0;
    double differenceSum = 0.0;
    double differenceIdle = 0.0;

    printf("\x1b[%d;%df", row - 1, 1);
    printf("\033[K");
    printf("V CPU: %0.2f", ChangeInCPU);

    
    if (90 < ChangeInCPU  && ChangeInCPU <= 100){
        printf("\x1b[%d;%df", row, xScaleCounter);
        printf(":\n");
    }

    else if (80 < ChangeInCPU && ChangeInCPU <= 90){
        printf("\x1b[%d;%df", row + 1, xScaleCounter);
        printf(":\n");
    }

    else if (70 < ChangeInCPU  && ChangeInCPU <= 80){
        printf("\x1b[%d;%df", row + 2, xScaleCounter);
        printf(":\n");
    }

    else if (60 < ChangeInCPU && ChangeInCPU <= 70){
        printf("\x1b[%d;%df", row + 3, xScaleCounter);
        printf(":\n");
    }

    else if (50 < ChangeInCPU && ChangeInCPU <= 60){
        printf("\x1b[%d;%df", row + 4, xScaleCounter);
        printf(":\n");
    }

    else if (40 < ChangeInCPU && ChangeInCPU <= 50){
        printf("\x1b[%d;%df", row + 5, xScaleCounter);
        printf(":\n");
    }

    else if (30 < ChangeInCPU && ChangeInCPU <= 40){
        printf("\x1b[%d;%df", row + 6, xScaleCounter);
        printf(":\n");
    }

    else if (20 < ChangeInCPU && ChangeInCPU <= 30){
        printf("\x1b[%d;%df", row + 7, xScaleCounter);
        printf(":\n");
    }

    else if (10 < ChangeInCPU && ChangeInCPU <= 20){
        printf("\x1b[%d;%df", row + 8, xScaleCounter);
        printf(":\n");
    }

    else if (0 <= ChangeInCPU && ChangeInCPU <= 10){
    printf("\x1b[%d;%df", row + 9, xScaleCounter);
    printf(":\n");
    }
}
    
