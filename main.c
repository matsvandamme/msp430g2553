#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"

int main(void)
{
    const gpio_config_t cfg = 
    {
        .dir = IO_DIR_OUTPUT,
        .out = IO_OUT_LOW,
        .resistor = IO_RESISTOR_DISABLED,
        .select = IO_SELECT_GPIO,
    };

    gpio_configure(IO_LED_GREEN, &cfg);
    gpio_configure(IO_LED_RED, &cfg);
    gpio_set_out(IO_LED_GREEN, IO_OUT_HIGH);
    gpio_set_out(IO_LED_RED, IO_OUT_HIGH);

    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    while(1)
    {
        
    }
}
