#include "hal/ultrasonic.h"
#include "hal/general_command.h"
#include "hal/gpio.h"

static bool shutdown = false;
pthread_t ultrasonicThread;

static long double distance_in_cm = 0;

long double get_distance_cm()
{
    long double start_time = 0;
    long double end_time = 0;
    long double length_of_time = 0;
    long double distance_in_cm = 0;

    while (getDataFromFile(Echo_value) == 48) // ascii 0
    {
        start_time = getTimeInNs();
    }
    while (getDataFromFile(Echo_value) == 49) // ascii 1
    {
        end_time = getTimeInNs();
    }
    length_of_time = end_time - start_time;
    distance_in_cm = length_of_time * 0.000017150; // convert to cm
    if (distance_in_cm <= 0)
    {
        distance_in_cm = 0;
    }
    // printf("distance: %.1Lf cm \n", distance_in_cm);

    return distance_in_cm;
}

static void *updateUltrasonic(void *args)
{
    (void)args;

    while (!shutdown)
    {

        sleepForMs(200);
        writeToFile(Trig_value, "1");
        sleepForMs(0.00001);
        writeToFile(Trig_value, "0");
    }
    return NULL;
}

void initializeUltrasonic()
{
    runCommand(allowInputEcho);
    runCommand(allowInputTrig);
    writeToFile(Echo_direction, "in");
    writeToFile(Trig_direction, "out");
    writeToFile(Echo_active_low, "0");
}

void createUltrasonicThread()
{
    pthread_create(&ultrasonicThread, NULL, updateUltrasonic, NULL);
}

void joinUltrasonicThread()
{
    pthread_join(ultrasonicThread, NULL);
}

void shutdownUltrasonic()
{
    shutdown = true;
}
