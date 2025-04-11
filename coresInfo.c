#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "cpuInfo.h"
#include "memInfo.h"

///_|> descry: This Function gets the cores info from the proc/cpuinfo file
///_|> returning: returns a int type, returning the cores value
int coresInfo(){
    char line[256];
    int lineNum = 0;
    int coreValue = 0;
   
    FILE *file = fopen("/proc/cpuinfo", "r");
 
    if (file == NULL){
        printf("Error opening file\n");
    }
    else{
        while(fgets(line, sizeof(line), file)){
            if (strncmp("processor", line, 9) == 0){
                coreValue++;
            }
            lineNum++;
        }
    }
    fclose(file);
    return coreValue;
 }

///_|> descry: This Function gets the cores Ghz info from the /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq file
///_|> returning: returns a double type, returning the Ghz value
double coreGHz(){
    char line[256];
    double coresInghz = 0.0;
    FILE *file = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
 
    if (file == NULL){
        printf("Error opening file\n");
    }
    else{
        while(fgets(line, sizeof(line), file)){
            coresInghz = strtol(line, NULL, 10);
        }
    }
    coresInghz = coresInghz / 1000000;
    fclose(file);
    return coresInghz;
 }

///_|> descry: This Function creates our cores graph
///_|> samples: Indicates the number of samples the program should run for used for the axises
///_|> tdelay: Indicates the delay for which each sample/iteration should run for used for the axises
///_|> row: Represents the current row we are on
///_|> cores: Represents the cores value we have
///_|> ghz: represents the coresGhz/frequency value we have
///_|> returning: returns a int type, as we are returning the current row we are on
int createCoresGraph(int samples, int tdelay, int row, int cores, double ghz){
printf("\x1b[%d;%df", 1, 1);
printInfo(samples, tdelay);
int numofCores = cores;
int fullRow = numofCores / 4;
int remainder_boxes = numofCores - (fullRow * 4);

printf("\x1b[%d;%df", row, 1);
printf("V Number of Cores: %d @ %0.2f GHz\n", numofCores, ghz);

//printing full rows of 4
for (int i = 0; i < fullRow; i++){
    for (int i = 0; i < 4; i++){
        printf("+ -- +  ");
        
    }
    printf("\n");
    for (int i = 0; i < 4; i++){
        printf("|    |  ");
        
    }
    printf("\n");
    for (int i = 0; i < 4; i++){
        printf("+ -- +  ");
        
    }
    printf("\n");
    row++;
}

//Printing leftover boxes
for (int i = 0; i < remainder_boxes; i++){
        printf("+ -- +  ");
        
    }
    printf("\n");
    for (int i = 0; i < remainder_boxes; i++){
        printf("|    |  ");
        
    }
    printf("\n");
    for (int i = 0; i < remainder_boxes; i++){
        printf("+ -- +  ");
        
    }
    printf("\n");
    row++;
    
    return row;
}