#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"
#include "drivers/mcu_init.h"

int main(void)
{
    mcu_init();
    gpio_init();
    led_init();

    while (1) 
    { 
        led_handle_blinking();
    }
}
