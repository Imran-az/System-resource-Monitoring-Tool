#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <signal.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h> 
#include "memInfo.h"
#include "cpuInfo.h"
#include "coresInfo.h"

/**************************************** */
//Function to Create Graphs
///_|> descry: This function is used to create the basic graphs for both Memory and CPU
///_|> samples: Indicates the number of samples the program should run for
///_|> tdelay: Indicates the delay for which each sample/iteration should run for
///_|> arr[5]: Represents the detectedFlags array and which flags were requested by the user
///_|> row: Represents which row the graph should print on
///_|> returning: returns a int type representing the new row that shall be used for the next iteration
int createGraph(int samples, int tdelay, int arr[5], int row){
   printf("\033[2J");
   printf("\033[H");
   printInfo(samples, tdelay);
   int cpuTotal = 100;
   float total_memory = totalMemory();
  
  //MEMORY GRAPH
    if (arr[2] == 1){
        printf("%0.2f GB |", total_memory);
        for (int i = 0; i < 12; i++){
            printf("\x1b[%d;%df", 4 + i, 10);
            printf("|\n");
        }

        printf("\x1b[%d;%df", 15, 5);
        printf("0 GB");
        printf("\x1b[%d;%df", 15, 11);
        for (int i = 0; i < samples; i++){
            printf("_");
        }
        printf("\n\n");
        if (arr[3] == 1){
            row = 18;
        }
        else{
            row = 15;
        }
    }

   //CPU GRAPH
   if (arr[3] == 1){
       printf("\x1b[%d;%df", row, 4);
       printf("%d %%    ", cpuTotal);
   for (int i = 0; i < 10; i++){
       printf("\x1b[%d;%df", row, 10);
       printf("|\n");
       row++;
   }
   printf("\x1b[%d;%df", row - 1, 5);
   printf("0 %%");
   printf("\x1b[%d;%df", row - 1, 11);
   for (int i = 0; i < samples; i++){
       printf("_");
   }
   printf("\n\n");
   }
   return row;
}
//***************************************** */
//VALID DELAY AND SAMPLES
//***************************************** */

///_|> descry: This function checks if delay and samples are valid, when not in positional arguments slots
///_|> sample: refers to the sample/tdelay the user provided as a flag
///_|> returning: returns a int type. -1 is returned if the samples/tdelay is not valid, and 1 represents when it is valid
int valid_delay_samples(char *sample){
   char *token = strtok(sample, "=");
   token = strtok(NULL, "=");


   if (token != NULL){
       for (int i = 0; token[i] != '\0'; i++){
           if(!(isdigit(token[i]))){
               return -1;
           }
       }
       return atoi(token);
   } 
   return -1;
}

///_|> descry: This function checks if delay and samples are valid in the positional arguments spots
///_|> num_flag: refers to the sample/tdelay the user provided as a flag in the positional argument spot
///_|> returning: returns a int type. -1 is returned if the samples/tdelay is not valid, and returns the actual number inputted by the user
int valid_delay_samplesPos(char *num_flag){
   for (int i = 0 ; i < strlen(num_flag); i++){
       if (!(isdigit(num_flag[i]))){
           return -1;
       }
   }
   return atoi(num_flag);
}
///_|> descry: This function checks to see if the flag already exists
///_|> flags: refers to the flagsDetected array that represents all the flags requested by the user
///_|> key: represents the index in the array
///_|> returning: returns a int type. Returns 1 if the flag has already been flagged and 0 if the flag doesnt exist
int flag_already_exists(int *flags, int key){
   if (flags[key] == 1){
       return 1;
   }
   return 0;
}

///_|> descry: This function is used for when the ctrl-c flag is detected and allows the user to exit the program
///_|> signo: represents the signal passed in
///_|> returning: This function is a void type.
void catch_function1(int signo){
    char answer[1024];

    while(true){
    printf("\033[30;1H\033[2K");
    printf("\nAre you sure you want to end the program? (y/n) ");
    fflush(stdout);
    fgets(answer, sizeof(answer), stdin);
    answer[strcspn(answer, "\n")] = '\0';
    
    if (strcmp("y", answer) == 0){
        kill(0, SIGKILL);
    }
    if (strcmp("n", answer) == 0){
        printf("\033[A");
        printf("\033[2K\r");
        fflush(stdout);
        return;
    }
    else{
        printf("\033[A");
        printf("\033[2K\r"); 
    }
}
}

int main(int argc, char *argv[]) {
       int samples = 20;
       int tdelay = 500000;
       //index 0: sample, index 1: tdelay, index 2: memory, index 3: cpu, index 4: cores, index 5: file
       int flagsDetected[5] = {0,0,0,0,0};
       int fileFlag = 0;

       if (signal(SIGINT, catch_function1) == SIG_ERR){
        fputs ( " An ␣ error ␣ occurred ␣ while ␣ setting ␣ a ␣ signal ␣ handler .\n " , stderr ) ;
        return EXIT_FAILURE;
        }

        if (signal(SIGTSTP, SIG_IGN) == SIG_ERR){
            fputs ( " An ␣ error ␣ occurred ␣ while ␣ setting ␣ a ␣ signal ␣ handler .\n " , stderr ) ;
            return EXIT_FAILURE;
        }


       //if there are no flags inputted and only the output file
       if (argc == 1){
           printf("1 flag detected\n\n");
           fileFlag = 1;
       }

       //finding the valid flags
       for (int i = 1; i < argc; i++) {
            //if samples flag is passes into positional argument spot
            if (i == 1 && valid_delay_samplesPos(argv[i]) != -1 && strncmp(argv[i], "--samples=", 10) != 0){
                flagsDetected[0] = 1;
                samples = valid_delay_samplesPos(argv[i]);

                if (samples == -1){
                    printf("invalid flag1\n\n");
                    return 0;
                }
            }

            //if tdealy flag is passed into positional argument spot
            else if (i == 2 && valid_delay_samplesPos(argv[i]) != -1 && (strncmp(argv[i], "--tdelay=", 9) != 0)){
                flagsDetected[1] = 1;
                tdelay = valid_delay_samplesPos(argv[i]);

                if (tdelay == -1){
                printf("invalid flag2\n\n");
                return 0;
                }
            }

            //memory flag
            else if (strcmp(argv[i], "--memory") == 0) {
                if (flag_already_exists(flagsDetected, 2) == 1){
                    printf("invalid flag3\n\n");
                    return 0;
                }

                flagsDetected[2] = 1;
            }

            //CPU flag
            else if (strcmp(argv[i], "--cpu") == 0) {
                if (flag_already_exists(flagsDetected, 3) == 1){
                    printf("invalid flag4\n\n");
                    return 0;
                }

                flagsDetected[3] = 1;
            }

            //Cores flag
            else if (strcmp(argv[i], "--cores") == 0) {
                if (flag_already_exists(flagsDetected, 4) == 1){
                    printf("invalid flag5\n\n");
                    return 0;
                }
                flagsDetected[4] = 1;
            }

            //Samples flag
            else if (strncmp(argv[i], "--samples=", 10) == 0) {
                //checks if positional argument exists
                if (flagsDetected[0] == 1){
                    printf("invalid samples values\n");
                    return 0;
                }
                
                char sample_char[256];
                strncpy(sample_char, argv[i], sizeof(sample_char) - 1);

                samples = valid_delay_samples(sample_char);
                if (samples == -1){
                    printf("invalid samples values\n");
                    return 0;
                }
                flagsDetected[0] = 1;
            }

            //tdelay flag
            else if ((strncmp(argv[i], "--tdelay=", 9) == 0)) {

                //checks if positional arguments exist
                if (flagsDetected[1] == 1){
                    printf("invalid tdelay value\n");
                    return 0;
                }

                char tdelay_char[256];
                strncpy(tdelay_char, argv[i], sizeof(tdelay_char) - 1);

                tdelay = valid_delay_samples(tdelay_char);
                if (tdelay == -1){
                    printf("invalid tdelay value\n");
                    return 0;
                }
                flagsDetected[1] = 1;
            }

            else {
                printf("invalid input\n");
                return 0;
            }
        }

   int row = 4;
   if (fileFlag == 1 || (flagsDetected[0] == 1 && flagsDetected[1] == 0 && flagsDetected[2] == 0 && flagsDetected[3] == 0 && flagsDetected[4] == 0) || (flagsDetected[0] == 1 && flagsDetected[1] == 1 && flagsDetected[2] == 0 && flagsDetected[3] == 0 && flagsDetected[4] == 0)){
        flagsDetected[2] = 1;
        flagsDetected[3] = 1;
        flagsDetected[4] = 1;
   }
   int currentRow = createGraph(samples, tdelay, flagsDetected, row);
   int xscale = 11;
   struct CPUinformation* cpuData = malloc(sizeof(struct CPUinformation));

   //CORES
   int pipe3[2];
   int pipe4[2];
   int cores = 0;
   double ghz = 0.0;
   pid_t p3, p4;

   if (pipe(pipe3) == -1 || pipe(pipe4) == -1){
    perror("pipe");
    exit(1);
    }   

   if (flagsDetected[4] == 1){ 
    fflush(stdout);
    p3 = fork();
    //checking fork 
    if (p3 < 0){
        fprintf(stderr, "Fork Fail");
        exit(1);
    }
    //CHILD PROCESS
    if (p3 == 0){
        close(pipe3[0]);
        cores = coresInfo();
        //ghz = coreGHz();

        write(pipe3[1], &cores, sizeof(int));
        //write(pipe3[1], &ghz, sizeof(double));
        close(pipe3[1]);
        exit(0);
    }
    p4 = fork();

    if (p4 < 0){
        fprintf(stderr, "Fork Fail");
        exit(1);
    }
    //CHILD PROCESS
    if (p4 == 0){
        close(pipe4[0]);
        //cores = coresInfo();
        ghz = coreGHz();

        //write(pipe4[1], &cores, sizeof(int));
        write(pipe4[1], &ghz, sizeof(double));
        close(pipe4[1]);
        exit(0);
    }

}
   if (flagsDetected[2] == 1 || flagsDetected[3] == 1){
        for (int i = 0; i < samples; i++){
            int pipe1[2];
            int pipe2[2];
            pid_t p1, p2;

            //checking pipe creation
            if (pipe(pipe1) == -1 || pipe(pipe2) == -1){
                perror("pipe");
                exit(1);
            }

            //Memory Flag Detected
            if (flagsDetected[2] == 1){
                fflush(stdout);
                p1 = fork();
                //checking fork 
                if (p1 < 0){
                    fprintf(stderr, "Fork Fail");
                    exit(1);
                }
                //CHILD PROCESS
                else if (p1 == 0){
                    close(pipe1[0]);

                    double totalMem = totalMemory();
                    double memVal = MemoryInfo();

                    write(pipe1[1], &totalMem, sizeof(double));
                    write(pipe1[1], &memVal, sizeof(double));
                    close(pipe1[1]);
                    exit(0);
                }
            }

            //CPU flag detected 
            if (flagsDetected[3] == 1){
                fflush(stdout);
                p2 = fork();

                //checking fork 
                if (p2 < 0){
                    fprintf(stderr, "Fork Fail");
                    exit(1);
                }
                //CHILD PROCESS
                else if (p2 == 0){
                    close(pipe2[0]);
                    double currentSum = 0.0;
                    double currentIdletime = 0.0;
                    double differenceSum = 0.0;
                    double differenceIdle = 0.0;
                    double ChangeInCPU = 0.0;
                    double prevSum = 0.0;
                    double pastIdle = 0.0;

                    //FIRST ITERATION
                    if (i == 0){
                        CpuInfoStruct(cpuData);
                        currentSum = cpuData->currentSum;
                        currentIdletime = cpuData->currentIdleTime;
                        ChangeInCPU = cpuData->cpuPercentage;

                        differenceSum = currentSum - prevSum;
                        differenceIdle = currentIdletime - pastIdle;
                    }
                    //ANY ITERATION != FIRST
                    else {
                        //make call to the current struct obtain sum and idle then make new call
                        prevSum = cpuData -> currentSum;
                        pastIdle = cpuData->currentIdleTime;

                        CpuInfoStruct(cpuData);
                        currentSum = cpuData->currentSum;
                        currentIdletime = cpuData->currentIdleTime;
                        differenceSum = cpuData->currentSum - prevSum;
                        differenceIdle = cpuData->currentIdleTime - pastIdle;

                        ChangeInCPU = ((differenceSum - differenceIdle) /  differenceSum) * 100;
                    }
                    
                    //WRITE INFORMATION TO THE STRUCT
                    cpuData->currentSum = currentSum;
                    cpuData->currentIdleTime = currentIdletime;
                    cpuData->prevSum = prevSum;
                    cpuData->prevIdleTime = pastIdle;
                    cpuData->cpuPercentage = ChangeInCPU;

                    write(pipe2[1], cpuData, sizeof(struct CPUinformation));

                    close(pipe2[1]);
                    exit(0);
                }
            }
            if (flagsDetected[2] == 1){
                waitpid(p1, NULL, 0);
                close(pipe1[1]);

                row = 4;
                double totalMem = 0.0;
                double memVal = 0.0;
                read(pipe1[0], &totalMem, sizeof(double));
                read(pipe1[0], &memVal, sizeof(double));
                populateMemGraph(samples, tdelay, xscale, row, totalMem, memVal);
            }
            if (flagsDetected[3] == 1){
                waitpid(p2, NULL, 0);
                close(pipe2[1]);
                if (flagsDetected[2] == 0 ){
                    row = 4;
                }
                //CPU and Memory graph exists
                else{
                    row = 18;
                }
                read(pipe2[0], cpuData, sizeof(struct CPUinformation));
                populateCPUgraph(samples, tdelay, xscale, cpuData->cpuPercentage, row, i);
            }            
        xscale++;
        printf("\x1b[%d;%df", currentRow + 2, 1);  
        usleep(tdelay);
        }
    }
    if (flagsDetected[4] == 1){
        waitpid(p3, NULL, 0);
        waitpid(p4, NULL, 0);
        close(pipe3[1]);
        close(pipe4[1]);
        cores = 0;
        ghz = 0.0;
        read(pipe3[0], &cores, sizeof(int));
        read(pipe4[0], &ghz, sizeof(double));
        close(pipe3[0]);
        close(pipe4[0]);

        if (flagsDetected[2] == 1){
            currentRow = createCoresGraph(samples, tdelay, currentRow + 2, cores, ghz);
            printf("\x1b[%d;%df", 47, 1);
        }
        else if (flagsDetected[2] == 1 || flagsDetected[3] == 1){
            createCoresGraph(samples, tdelay, currentRow + 2, cores, ghz);
            printf("\x1b[%d;%df", 34, 1);
        }
        else {
            createCoresGraph(samples, tdelay, currentRow, cores, ghz);
            printf("\x1b[%d;%df", 22, 1);
        }
    }

   free(cpuData);
   return 0;
}





