#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"
#include "drivers/mcu_init.h"
#include "drivers/millis.h"
#include <stddef.h>

int main(void)
{
    mcu_init();
    gpio_init();
    led_init();
    millis_init();

    led_start_blinking(led_get_handle(IO_LED_RED), 200, 800);
    led_start_blinking(led_get_handle(IO_LED_GREEN), 500, 500);

    uint32_t current_time = millis();

    while (1) 
    { 
        led_handle_blinking();

        // Check if 10 seconds has passed
        if (millis() - current_time >= 10000) {
            // Toggle LED states
            led_stop_blinking(led_get_handle(IO_LED_RED));
            led_stop_blinking(led_get_handle(IO_LED_GREEN));
        }
    }
}

