# Assignment 3 - CSCB09 

## 1. Metadata
Title: System Monitoring Tool -- Concurrency & Signals

Author: Imran Aziz

Date: 04/07/2025

PLEASE FULL SIZE TERMINAL BEFORE RUNNING!!!

## 2. Introduction/Rationale
The following project allows users to view and display their Memory, CPU, and Cores, usage, as the program displays their data in
a graph format, showing the information running for samples time with tdelay intervals.

## 3. Description of how you solved/approached the problem
The first thing I did when approaching this assignment was fix some cases I missed from my intial submission for A1. After this I intially had the code all in one file and so I spent time refractoring my code 
to be more modular, as I had one file for each compoennt. I had one file for stuff that was related to memory, I had another file for stuff that was related to CPU, I had another file for stuff that was related to cores, and a file for my main.
By having it in this format it made my code more cleaner and easier to read. After this I went about creating my makefile ensuring that I had all the dependecies, and clean rules included. After refractoring my code I spent time thinking how I
wanted to approach the forking and pipes in my code. I ultimately chose to go with the one way connection, of child communicating with the parent pipe implementation. How I structured it was after checking which flags were called by the user. I 
created a fork and pipe for the cores and retrived the data relating to the cores, passing both the cores, and cores frequency into the same pipe. After this i had a for loop running for n samples. In this for loop I created two additional forks along with defining
two additional pipes that will be used for both memory and CPU. What happens in this for loop is that it creates a pipe and a fork for memory and cpu every iteration retriving the data in the child of the fork and right before the end of the iteration in the for loop I send this 
data back to the parent and plot both memory and CPU onto their designated graph. And this continue happening for n sample iterations. After the for loop ends I then have the parent wait for the cores information that was run before the for loop and then retrieve the data
from the pipe and sends it to the parent, which will then print the cores graph. 

After I finish ensuring my program ran concurrently fetching data in the child forks and printing in the parent. I then focused on signals. I first focused on intercepting the signals coming from Ctrl-Z. By creating a signal handler that detected Ctrl-Z I had just had it continue the program not stopping.
And then for Ctrl-C I created a function that once Ctrl-C was detected it would ask the user if they want to stop the program and if yes it will then run kill(0, SIGKILL) kiiling every single process running in the same process ID group, and if no it will continue the program like normal from where it was left off.

## 4. Implementation
  1. Fixed the cases I missed from A1 grading
  2. Refractored my code to be more modular, creating a file for memory, cpu, cores, and main
  3. Then created my makefile
  4. refractored my code seperating my plotting function into two functions data collecting and plotting for each Memory, CPU, and Cores. By doing this I can the have it so when I fork I do the data collecting in my children and do the printing in my parent
  5. figured out the pipes method I wanted to use which was child to parent communication and then created 3 pipes along with forking for each component (ie: one fork and pipe was meant for Memory, another was for CPU, and finally the last one was for Cores)
  6. Implmeneted signals for Ctrl-C and Ctrl-Z
  7. Had 4 modules, one for memory, one for cpu, one for cores, and one for main
 ## Functions
 MAIN.C
 1. ```sh int createGraph(int samples, int tdelay, int arr[5], int row)```
 - This function creates the graph for both memory and cpu depending on what flag is called.
 2. ```sh int valid_delay_samples(char *sample)```
 - This function checks to see if the samples and tdelay are passed in as valid flag arguments when they are not in the        positional argument spots
 3. ```sh int valid_delay_samplesPos(char *num_flag)``` 
 - This function checks to see if the samples and tdelay are passed in as  arguments when they are passed into the      positional argument spots
 4. ```sh int flag_already_exists(int *flags, int key)```
 - This function check to see if a flag already exists in our flagsDetected array. This is mainly used for checking for invalid flags such as if a user passes in the same flag twice
 5. ```sh void catch_function1(int signo)```
 - This function is used after the user enters Ctrl-C to prompt the user for an input and either stop or continue the program based on their input

memoryInfo.c
1. ```sh void printInfo(int samples, int tdelay)```
- This function displays the simple header that shows the number of samples the program is running for along with the tdelay. Mainly meant for printing the header to the screen every time the program is run
2. ```sh int processMemory(char *sample)```
- This function is meant as a helper to read in the specific memroy total line and memory available line and basically read the line and return the value associated to each one
3. ```sh float MemoryInfo()```
- This function opens and reads the proc memory file, identifying the total memroy line and avaiable memroy line in the file and using processMemory() helper it uses these two values to calculate the memory used
4. ```sh double totalMemory()```
-  THis function grabs the total memory from the proc file and is mainly used to print the total memroy on the memroy graph access
5. ```sh void populateMemGraph(int samples, int tdelay, int xscale, int row, double total_Memory, double val)```
- This function plots a single point on the memory graph and is used when in the main we run it for n samples time and so for each iteration it will plot the point relative the iteration it is on

cpuInfo.c
1. ```sh struct CPUinformation processCpuStruct(struct CPUinformation* cpuinfo, char *sample)``` 
- This function is used so we can store all the realtive information from the proc file into a struct and is used to help keep track of the previous CPU value along with accessing the previous CPU value when we fork
2. ```sh struct CPUinformation CpuInfoStruct(struct CPUinformation *cpuData) ```
- This function gets the info from the cpu file and returns the cpu percentage as it uses processCpuStruct() function to get this data.
3. ```sh void createCPUGraph(int samples, int tdelay)```
- This function is used to create the CPU graph and is used in our createGraph() function in main
4. ```sh void populateCPUgraph(int samples, int tdelay, int xscale, double ChangeInCPU, int row, int count)```
- This function plots a single point on the CPU graph and is used when in the main we run it for n samples time and so for each iteration it will plot the point relative the iteration it is on

CoresInfo.c
1. ```sh int coresInfo()```
- This function reads inthe info from proc cpuinfo, and returns the number of cores there are 
2. ```sh double coreGHz()```
- This function reads from a system file and retrieves the max frequency 
3. ```sh int createCoresGraph(int samples, int tdelay, int row, int cores, double ghz)```
- This function creates the cores graph that will be called in main.c

## 5. Pseudo-code 
  FLOW DIAGRAM:
  
![A3Psuedocdoe drawio](https://gist.github.com/user-attachments/assets/449d19e0-cad2-4ea3-88a1-6719569662dd)




  
 ## 6. Instructions on how to compile your code
 IMPORTANT: whenever giving CLAs at the end please provide ( | less), also the positional argument must always follow the executable file and be the first argument after ./output
 FIRST: Ensure all files are open so all c files header files and makefile then: 
  1. type in make to compile the code:
  ```sh
  make
  ``` 
  2. type in ./myMonitoringTool and what ever CLA you want, if no samples or tdelay is printed the default samples and tdelay specified in the assignment will be used: Please open terminal to max size for viewing:
  ```sh
  ./myMonitoringTool  
  ```
  
  ## 7. Expected Result
  1. example of providing just samples to see graphs printed for 50 samples:
  ```sh
  ./myMonitoringTool 50 
  ```
  2. example of providing just samples and tdelay in the positonal argument spots and all graphs will be printed:
  ```sh
  ./myMonitoringTool 50 100000
  ```
  3. example of providing just samples and tdelay not in the positonal argument spots and all graphs will be printed:
  ```sh
  ./myMonitoringTool --samples=50 --tdelay=100000
  ```
  4. example of providing just memory CLA flag, which will result in only memory graph being printed:
  ```sh
  ./myMonitoringTool --memory
  ```
  5. example of providing just CPU CLA flag along with positional arguments, which will result in only CPU graph being printed for n number of samples with tdelay in between every iteration:
  ```sh
  ./myMonitoringTool 50 100000 --cpu
  ```
  6. example of providing just the executable file, all graphs will be printed, for the default samples and tdelay
  ```sh
  ./myMonitoringTool 
  ```
 
  ```
## 8. Test Cases for unexpected behaviour or ERRORS
  1. if ./myMonitoringTool is given and postional argument is given along with --samples or --tdelay (only positional arguments can be given or non positional arguments. Can not have a mix)
  ```sh
  ./myMonitoringTool 50 --samples=50  will result in an ERROR
  ```
  2. if ./myMonitoringTool is given along with one of the flags spelled incorrectly will result in an ERROR
  ```sh
  ./myMonitoringTool --memory --coresfsfgsd --cpu 
  ```

### FILES READ FROM
   - Memory - "/proc/meminfo"
   - CPU - "/proc/stat"
   - Cores - "/proc/cpuinfo"
   - Cores GHz - "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"

### FORMULAS USED 
   - Memory - (totalMem - availableMem) / 1000000 = Used Memory
   - CPU - ((sumofCPUs - IdleTimeCPU) / sumofCPUs) * 100
   - CoresGHz - kilohertz converted to gigahertz (1 KHz = 10^-6 GHz)

### GRAPH CALCULATIONS
   - For Memory graph the total memory is taken and divided by 12 to get the value of each interval on the graph, these          intervals are represented in the if statements that decide where to plot the points
   - For CPU graph The axis is divded into 10 sections with equal 10% intervals going from 0%-100%. The CPU percentage            increase/decrease is then plotted into the section of best fit

### ASSUMPTIONS MADE
   - points 0-10% range for CPU graph is on the x-axis
   - First CPU point is plotted based off the first point obtained from the CPU File, after that all points are calculated 
     based off the following formula, (currentSum - prevSum) - (CurrentIdle - PrevIdle) / (currentSum - prevSum), which            represents the increase or decrease of CPU between two snapshots of CPU
     
### DISCLAIMERS
   - For displaying one graph if user clicks ctrl-c the prompt will be displayed a little further down
   - If you spam ctrl-c or ctrl-z really fast it will continously write it to the screen and could cause overwriting of the         graph
     
### WARNINGS
   - Will get a warning for CPUVAL being set but not used, im setting it just so I can update the pointer from the struct        that represenmts CPUPercentage
   - Used Usleep() to calcualte the tdelay, warning will happen
   - Will get a warning for linelength not being used but it is used for calcualting the sizeof in the respected function

## 10. References
  1. For using kill(0, SIGKILL) - https://pubs.opengroup.org/onlinepubs/9699919799.orig/functions/kill.html#:~:text=The%20kill()%20function%20shall,no%20signal%20is%20actually%20sent.
  2. For using SIG_IGN - https://man7.org/linux/man-pages/man2/signal.2.html
  3. For fokring - Used lecture notes along with https://www.geeksforgeeks.org/fork-execute-processes-bottom-using-wait/
  4. For signals I used mainly the example provided in lecture alogn with the resources I linked above
  5. Using fgets along with strcspn to remove null character - https://stackoverflow.com/questions/74466658/reading-a-string-in-c-using-fgets
  6. Using ANSI escape codes - https://en.wikipedia.org/wiki/ANSI_escape_code
