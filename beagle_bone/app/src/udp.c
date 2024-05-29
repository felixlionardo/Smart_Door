#include "udp.h"
#include "hal/temperature_sensor.h"
#include "hal/servo.h"
#include "hal/led.h"
#include "ultrasonic_detector.h"
#include "shutdown.h"

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define FILE_UPTIME "/proc/uptime"
#define MSG_MAX_LEN 200000
#define PORT 3000
#define RPORT 12345

static pthread_t UDPThread;

static char *lockStatus;


void *displayUDPcommands(void *args)
{
    (void)args;
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;                // Connection may be from network
    sin.sin_addr.s_addr = htonl(INADDR_ANY); // Host to   Network long
    sin.sin_port = htons(RPORT);              // Host to Network short

    // Create the socket for UDP
    int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

    // Bind the socket to the port (PORT) that we specify
    if (bind(socketDescriptor, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        // Get the data (blocking)
        // Will change sin (the address) to be the address of the client.
        // Note: sin passes information in and out of call!
        struct sockaddr_in sinRemote;

        unsigned int sin_len = sizeof(sinRemote);

        char messageRx[MSG_MAX_LEN];

        // Pass buffer size - 1 for max # bytes so room for the null (string data)
        int bytesRx = recvfrom(socketDescriptor,
                               messageRx, MSG_MAX_LEN - 1, 0,
                               (struct sockaddr *)&sinRemote, &sin_len);

        // Check for errors (-1)

        // Make it null terminated (so string functions work)
        // - recvfrom given max size - 1, so there is always room for the null
        messageRx[bytesRx] = 0;


        // change mode to "mode_none"
        if (strncmp(messageRx, "lock", strlen("lock")) == 0)
        {
            turn_on_led();
            //lockServo();
            
        }

        else if (strncmp(messageRx, "unlock", strlen("unlock")) == 0)
        {
            turn_off_led();
            //unlockServo();
        }

        else if (strncmp(messageRx, "celcius", strlen("celcius")) == 0)
        {
            setUnitCelsius(true);
        }

        else if (strncmp(messageRx, "fahrenheit", strlen("fahrenheit")) == 0)
        {
            setUnitCelsius(false);
        }
        else if (strncmp(messageRx, "clear", strlen("clear")) == 0)
        {
            clearEmails();
        }

        //get information about the current state of the program
        else if (strncmp(messageRx, "info", strlen("info")) == 0)
        {

            bool status = isLocked();
            if (status == true)
                lockStatus = "Locked";
            else
                lockStatus = "Unlocked";

            double temperature;

            if (getUnitCelsius() == true)
                temperature = getCelciusTemperature();
            else
                temperature = getFarhenheitTemperature();


            
            char messageTx[MSG_MAX_LEN];
            sprintf(messageTx, "Temperature:%.1f\nLock Status:%s\n", temperature, lockStatus);
            sin_len = sizeof(sinRemote);
            sendto(socketDescriptor, messageTx, strlen(messageTx), 0, (struct sockaddr *)&sinRemote, sin_len);
        }

        // command for any other command
        else
        {
            addEmail(messageRx);
        }

    }
    close(socketDescriptor);
    return NULL;
}

void createUDPThread()
{
    pthread_create(&UDPThread, NULL, displayUDPcommands, NULL);
}

void joinUDPThread()
{
    pthread_join(UDPThread, NULL);
}