#include "hal/temperature_sensor.h"
#include "hal/general_command.h"
#include "hal/gpio.h"
#include "hal/display.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#define OUTPUT3_GPIO "/sys/class/gpio/gpio50/value"
#define OUTPUT3_GPIO_NUM "50"
#define OUTPUT3_PIN "p9.14"

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

static bool shutdown = false;
static int temperatureReading = 0;
static pthread_t temperatureSensorThread;

static bool celciusUnit = true;

void initialize_temperature_sensor()
{
    // gpio 50 set to output
    config_pin_to_gpio(OUTPUT3_PIN);
    export_gpio(OUTPUT3_GPIO_NUM);
    set_gpio_direction(OUTPUT3_GPIO_NUM, "out");
    writeToFile(OUTPUT3_GPIO, "1");
}

bool getUnitCelsius()
{
    return celciusUnit;
}

void setUnitCelsius(bool a)
{
    celciusUnit = a;
}

void *updateTemperatureReading(void *args)
{
    // Open file
    (void)args;
    while (!shutdown)
    {
        FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
        if (!f)
        {
            printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
            printf("       Check /boot/uEnv.txt for correct options.\n");
            exit(-1);
        }
        // Get reading

        int itemsRead = fscanf(f, "%d", &temperatureReading);
        // print itemsRead
        if (itemsRead < 0)
        {
            printf("ERROR: Unable to read values from voltage input file.\n");
            exit(-1);
        }
        fclose(f);

        if (celciusUnit)
        {
            setDisplay(getCelciusTemperature());
        }
        else
        {
            setDisplay(getFarhenheitTemperature());
        }
        sleepForMs(5000);
    }

    return NULL;
}

int getTemperatureReading()
{
    return temperatureReading;
}

static double getTemperatureVoltage()
{
    return (temperatureReading * A2D_VOLTAGE_REF_V) / A2D_MAX_READING;
}

double getCelciusTemperature()
{
    double temperatureVoltage = getTemperatureVoltage();
    return (temperatureVoltage * 1000 - 500) / 10;
}

double getFarhenheitTemperature()
{
    return (getCelciusTemperature() * 9 / 5) + 32;
}

void createTemperatureSensorThread()
{
    pthread_create(&temperatureSensorThread, NULL, updateTemperatureReading, NULL);
}

void shutdownTemperatureSensor()
{
    shutdown = true;
    pthread_join(temperatureSensorThread, NULL);
}