#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include "hal/general_command.h"
#include "hal/gpio.h"
#include "hal/servo.h"


#define OUTPUT2_GPIO "/sys/class/gpio/gpio60/value"
#define OUTPUT2_GPIO_NUM "60"
#define OUTPUT2_PIN "p9.12"

void initialize_led()
{
    // gpio 30 set to output
    config_pin_to_gpio(OUTPUT2_PIN);
    export_gpio(OUTPUT2_GPIO_NUM);
    set_gpio_direction(OUTPUT2_GPIO_NUM, "out");

}

void turn_on_led()
{
    writeToFile(OUTPUT2_GPIO, "1");
    setIsLocked(true);
}

void turn_off_led()
{
    writeToFile(OUTPUT2_GPIO, "0");
    setIsLocked(false);

}