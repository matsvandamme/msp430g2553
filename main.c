#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"

int main(void)
{
    gpio_init();
    led_init();

    while (1) 
    { 
        led_handle_blinking();
    }
}
