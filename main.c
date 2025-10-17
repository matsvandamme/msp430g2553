#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"
#include "drivers/mcu_init.h"

int main(void)
{
    mcu_init();
    gpio_init();
    led_init();
    for (uint8_t i = 0; i < 2; i++) {
        static uint8_t adder = 85;
        led_start_blinking((led_t *)&leds[i], LED_ON_PERIOD_MS + adder*i, LED_OFF_PERIOD_MS + 2*adder*i);
    }

    while (1) 
    { 
        led_handle_blinking();
    }
}
