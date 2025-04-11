CC = gcc
CFLAGS = -Wall -g -std=c99 -Werror

myMonitoringTool: main.o memInfo.o cpuInfo.o coresInfo.o
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CGLAGS) -c $< -o $@

memInfo.o: memInfo.c memInfo.h
	$(CC) $(CGLAGS) -c $< -o $@

cpuInfo.o: cpuInfo.c cpuInfo.h
	$(CC) $(CGLAGS) -c $< -o $@

coresInfo.o: coresInfo.c coresInfo.h
	$(CC) $(CGLAGS) -c $< -o $@

.PHONY: clean
	clean:
		rm -f *.o main