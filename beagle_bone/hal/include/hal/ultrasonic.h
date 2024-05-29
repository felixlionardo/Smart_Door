#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_

/* Module to initialize and cleanup the ultrasonic distance sensor thread.

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

#define allowInputTrig "config-pin p9.15 gpio" // GPIO 9_15, gpio48
#define allowInputEcho "config-pin p9.17 gpio" // GPIO 9_17, gpio5

#define A1raw_value "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define Trig_direction "/sys/class/gpio/gpio48/direction"
#define Echo_direction "/sys/class/gpio/gpio5/direction"
#define Echo_active_low "/sys/class/gpio/gpio5/active_low"
#define Trig_value "/sys/class/gpio/gpio48/value"
#define Echo_value "/sys/class/gpio/gpio5/value"

void initializeUltrasonic();
void createUltrasonicThread();
void shutdownUltrasonic();
void joinUltrasonicThread();

long double get_distance_cm();



#endif