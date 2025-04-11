#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "memInfo.h"

///_|> descry: This Function prints out the header information that appears before the graphs
///_|> samples: Indicates the number of samples the program should run for
///_|> tdelay: Indicates the delay for which each sample/iteration should run for
///_|> returning: returns a void type. This function is used just for printing
void printInfo(int samples, int tdelay){
    double tdelay_sec = tdelay * pow(10, -6);
    printf("Nbr of samples: %d -- every %d microSecs (%0.3f secs)\n\n\n", samples, tdelay, tdelay_sec);
}

///_|> descry: This Function process the memory and returns it as int type
///_|> samples: Indicates memory line from file
///_|> returning: returns an int type. This function will return the memory integer if successful or return -1 if not successful
int processMemory(char *sample) {
    int memVal; 

    if (sscanf(sample, "%*s %d", &memVal) == 1) {
        return memVal;
    }
    return -1; 
}

///_|> descry: This Function gets the memory info from the proc/meminfo file and reads in the total and available memory
///_|> returning: returns a float type, returning the calculated memory used after the calculations
float MemoryInfo(){
    char line[256]; 
    int currentline_num = 1;
    int wantedline1 = 1;
    int wantedline2 = 3;
    long totalMem_num = 0;
    long availableMem_num = 0;
    double memoryUsed = 0.0;
  
  
    FILE * file = fopen("/proc/meminfo", "r");
    if (file == NULL){
        printf("Error opening file\n");
    }
  
  
    if (file != NULL){
        while (fgets(line, sizeof(line), file) != NULL){
            if (currentline_num == wantedline1){
                totalMem_num = processMemory(line);
                currentline_num++;
            }
            else if (currentline_num == wantedline2){
                availableMem_num = processMemory(line);
                currentline_num++;
            }
            else{
                currentline_num++;
            }
        }
    }

    memoryUsed = ((double)(totalMem_num - availableMem_num) /1000000);
  
    fclose(file);
    return memoryUsed;
  }

///_|> descry: This function obtains the totalMemory from the proc/meminfo file
///_|> returning: returns a double type, returning the total memory obtained as a float
  double totalMemory(){
    int wantedLine = 1;
    int currentline_num = 1;
    int linelength = 0;
    char line[256];
    char storedMem[256];
    double totalMem_num = 0.0;
 
    FILE * file = fopen("/proc/meminfo", "r");
    if (file == NULL){
        printf("Error opening file\n");
    }
    else{
        while (fgets(line, sizeof(line), file)){
            if (currentline_num == wantedLine){
                linelength = strlen(line);
                strcpy(storedMem, line);
                break;
            }
        }
    }
    totalMem_num = processMemory(storedMem) /1000000;
    fclose(file);
    return totalMem_num;
 }

///_|> descry: This function plots a single point on the memory graph
///_|> samples: Indicates the number of samples the program should run for
///_|> tdelay: Indicates the delay for which each sample/iteration should run for
///_|> xscale: Represents the current position on the x axis
///_|> row: Represents the current row we are on
///_|> total_Memory: Represents the total memory obtained from the proc/meminfo file
///_|> val: Represents the used memory value we want to plot on the graph
///_|> returning: returns a void type. This function is used just for printing a point on the graph
  void populateMemGraph(int samples, int tdelay, int xscale, int row, double total_Memory, double val){
    printf("\033[H");
    double sections = 12.0;

    //divide each section for y-axis
    double Yscale = total_Memory/sections;
    int xScaleCounter = xscale;
 

    //go to line 3 and update value
    printf("\x1b[%d;%df", 3, 1);
    printf("\033[K");
    printf("V Memory %0.2f GB \n", val);
 
    if (Yscale * 11 <= val && val < Yscale * 12){
        printf("\x1b[%d;%df", row, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 10 <= val && val < Yscale * 11){
        printf("\x1b[%d;%df", row + 1, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 9 <= val && val < Yscale * 10){
        printf("\x1b[%d;%df", row + 2, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 8 <= val && val < Yscale * 9){
        printf("\x1b[%d;%df", row + 3, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 7 <= val && val < Yscale * 8){
        printf("\x1b[%d;%df", row + 4, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 6 <= val && val < Yscale * 7){
        printf("\x1b[%d;%df", row + 5, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 5 <= val && val< Yscale * 6){
        printf("\x1b[%d;%df", row + 6, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 4 <= val && val< Yscale * 5){
        printf("\x1b[%d;%df", row + 7, xScaleCounter);
        printf("#\n");

    }
    else if (Yscale * 3 <= val && val < Yscale * 4){
        printf("\x1b[%d;%df", row + 8, xScaleCounter);
        printf("#\n");
    }
    else if (Yscale * 2 <= val && val < Yscale * 3){
        printf("\x1b[%d;%df", row + 9, xScaleCounter);
        printf("#\n");

    }
    else if (Yscale <= val && val < Yscale * 2){
        printf("\x1b[%d;%df", row + 10, xScaleCounter);
        printf("#\n");

    }
    else if (0 <= val && val < Yscale){
        printf("\x1b[%d;%df", row + 11, xScaleCounter);
        printf("#\n");

    }
 }