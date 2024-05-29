#include "hal/general_command.h"
#include <time.h> 
#include <sys/time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

// Function to get the current time in milliseconds
long long getTimeInMs(void) {    
    struct timespec spec;    
    clock_gettime(CLOCK_REALTIME, &spec);    
    long long seconds = spec.tv_sec;    
    long long nanoSeconds = spec.tv_nsec;    
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;    
    return milliSeconds; 
}

// Function to sleep for a given number of milliseconds
void sleepForMs(long long delayInMs) {    
    const long long NS_PER_MS = 1000 * 1000;    
    const long long NS_PER_SECOND = 1000000000;    
    long long delayNs = delayInMs * NS_PER_MS;    
    int seconds = delayNs / NS_PER_SECOND;    
    int nanoseconds = delayNs % NS_PER_SECOND;    
    struct timespec reqDelay = {seconds, nanoseconds};    
    nanosleep(&reqDelay, (struct timespec *) NULL); 
}

void runCommand(char* command) {    
    // Execute the shell command (output into pipe)    
    FILE *pipe = popen(command, "r");    
    // Ignore output of the command; but consume it     
    // so we don't get an error when closing the pipe.    
    char buffer[1024];    
    while (!feof(pipe) && !ferror(pipe)) {        
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)            
        break;        
        // printf("--> %s", buffer);  // Uncomment for debugging   
        // Get the exit code from the pipe; non-zero is an error:    
        int exitCode = WEXITSTATUS(pclose(pipe));    
        if (exitCode != 0) {        
            perror("Unable to execute command:");        
            printf("  command:   %s\n", command);        
            printf("  exit code: %d\n", exitCode);    
        } 
    }
}

void writeToFile(const char *file, const char *value) {
    FILE *f = fopen(file, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, "%s", value);
    fclose(f);
}

void sendEmail(const char *subject, const char *body, const char *to) {
    char command[1024];
    sprintf(command, "echo \"%s\" | mail -s \"%s\" %s", body, subject, to);
    runCommand(command);
}
void readFromFileToScreen(char *fileName)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    printf("Read: '%s'\n", buff);
}

long long getTimeInNs(void)
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long total_nanoSeconds = seconds * 1000000000 + nanoSeconds;
    return total_nanoSeconds;
}

int getDataFromFile(char *fileName)
{
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    return buff[0];
}

int read_value_from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    int value;
    if (fscanf(file, "%d", &value) != 1) {
        perror("Error reading from file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);

    return value;
}
