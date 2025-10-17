#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"
#include "drivers/mcu_init.h"
#include <stddef.h>

int main(void)
{
    mcu_init();
    gpio_init();
    led_init();

    led_start_blinking(led_get_handle(IO_LED_RED), 700, 100);
    led_start_blinking(led_get_handle(IO_LED_GREEN), 500, 500);

    while (1) 
    { 
        led_handle_blinking();
    }
}
