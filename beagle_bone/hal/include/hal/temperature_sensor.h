#ifndef _TEMPERATURE_SENSOR_H_
#define _TEMPERATURE_SENSOR_H_

/* Module to initialize and cleanup the temperature sensor thread.

*/

#include <stdint.h>
#include <stdbool.h>

void initialize_temperature_sensor();
int getTemperatureReading();
double getCelciusTemperature();
double getFarhenheitTemperature();
void createTemperatureSensorThread();
void shutdownTemperatureSensor();
bool getUnitCelsius();
void setUnitCelsius(bool a);


#endif