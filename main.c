#include <msp430.h>
#include "drivers/gpio.h"
#include "drivers/led.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
}
