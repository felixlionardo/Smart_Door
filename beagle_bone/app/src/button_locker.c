#include "hal/general_command.h"
#include "hal/ultrasonic.h"
#include "hal/servo.h"
#include "hal/button.h"
#include "hal/led.h"

#include <pthread.h>

static bool locked = false;

static bool shutdown = false;
pthread_t buttonLockerThread;

static long double distance_in_cm = 0;

static void *updateButtonLocker(void *args)
{
    (void)args;

    while (!shutdown)
    {

        if (get_button_state())
        {
            if (isLocked())
            {
                //unlockServo();
                turn_off_led();
            }
            else
            {
                //lockServo();
                turn_on_led();
            }

            sleepForMs(1000);
        }

        sleepForMs(10);
    }
    return NULL;
}

void createButtonLockerThread()
{
    pthread_create(&buttonLockerThread, NULL, updateButtonLocker, NULL);
}

void joinButtonLockerThread()
{
    pthread_join(buttonLockerThread, NULL);
}

void shutdownButtonLocker()
{
    shutdown = true;
}
