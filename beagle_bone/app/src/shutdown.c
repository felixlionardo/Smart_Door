#include "hal/temperature_sensor.h"
#include "hal/general_command.h"
#include "hal/capture.h"

#include "hal/button.h"
#include "hal/servo.h"
#include "hal/led.h"
#include "hal/display.h"

#include "hal/ultrasonic.h"
#include "ultrasonic_detector.h"
#include "button_locker.h"
#include "udp.h"

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shutdownCond = PTHREAD_COND_INITIALIZER;

void createThreads()
{
    initialize_led();
    initialize_button();
    initializeUltrasonic();
    // initializeServo();
    initialize_temperature_sensor();
    //initializeDisplay();

    create_button_thread();
    createUltrasonicThread();
    createUltrasonicDetectorThread();
    createButtonLockerThread();
    // createDisplayThread();
    createTemperatureSensorThread();
    createUDPThread();

    openConnectionT();
    open_device();
    init_device();
    start_capturing();
    mainloop();
}

void joinThreads()
{

    // shutdown_button();
    // shutdownButtonLocker();
    // shutdownUltrasonic();
    // shutdownUltrasonicDetector();
    // shutdownTemperatureSensor();
    // shutdownDisplay();

    // join_button_thread();
    // joinButtonLockerThread();
    // joinUltrasonicThread();
    // joinUltrasonicDetectorThread();
    // joinUDPThread();
    // joinDisplayThread();

    // stop_capturing();
    // uninit_device();
    // close_device();
    // fprintf(stderr, "\n");
    // closeConnectionT();
}

void waitShutdown()
{
    pthread_mutex_lock(&shutdownMutex);
    pthread_cond_wait(&shutdownCond, &shutdownMutex);
    pthread_mutex_unlock(&shutdownMutex);
}

void signalShutdown()
{
    pthread_mutex_lock(&shutdownMutex);
    pthread_cond_signal(&shutdownCond);
    pthread_mutex_unlock(&shutdownMutex);
}