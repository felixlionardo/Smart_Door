#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#include "hal/general_command.h"
#include "hal/gpio.h"

#define OUTPUT_GPIO "/sys/class/gpio/gpio30/value"
#define OUTPUT_GPIO_NUM "30"
#define OUTPUT_PIN "p9.11"

#define BUTTON_GPIO "/sys/class/gpio/gpio31/value"
#define BUTTON_GPIO_NUM "31"
#define BUTTON_PIN "p9.13"

// Initialize shared variables
pthread_mutex_t button_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t button_pressed_cond = PTHREAD_COND_INITIALIZER;

static bool shutdown = false;
static int buttonState = 0;
static pthread_t buttonThread;

pthread_mutex_t* get_button_mutex() {
    return &button_mutex;
}

pthread_cond_t* get_button_pressed_cond() {
    return &button_pressed_cond;
}


int read_button_state()
{
    // Read the GPIO pin for button state
    return read_value_from_file(BUTTON_GPIO) == 1 ? 1 : 0;
}

void initialize_button()
{
    // gpio 30 set to output
    config_pin_to_gpio(OUTPUT_PIN);
    export_gpio(OUTPUT_GPIO_NUM);
    set_gpio_direction(OUTPUT_GPIO_NUM, "out");
    writeToFile(OUTPUT_GPIO, "1");

    // Configure button GPIO pin
    config_pin_to_gpio(BUTTON_PIN);
    export_gpio(BUTTON_GPIO_NUM);
    set_gpio_direction(BUTTON_GPIO_NUM, "in");
}

void *update_button_state(void *args)
{
    (void)args;

    while (!shutdown)
    {
        buttonState = read_button_state();

        sleepForMs(10);
    }

    return NULL;
}

int get_button_state()
{
    return buttonState;
}

void create_button_thread()
{
    pthread_create(&buttonThread, NULL, update_button_state, NULL);
}

void join_button_thread()
{
    pthread_join(buttonThread, NULL);
}

void shutdown_button()
{
    shutdown = true;
}
