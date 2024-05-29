#include "hal/general_command.h"
#include "hal/servo.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#define PWM_LED_PATH "/dev/bone/pwm/0/b"

bool locked = false;

// wires on top
void unlockServo()
{
    writeToFile(PWM_LED_PATH "/duty_cycle", "1500000");
    locked = false;
}

void lockServo()
{
    writeToFile(PWM_LED_PATH "/duty_cycle", "2500000");
    locked = true;
}

bool isLocked()
{
    return locked;
}

void setIsLocked(bool a) {
    locked = a;
}

void initializeServo()
{
    runCommand("config-pin P9_21 pwm");
    writeToFile(PWM_LED_PATH "/enable", "0");
    writeToFile(PWM_LED_PATH "/enable", "1");
    writeToFile(PWM_LED_PATH "/period", "20000000");
}