#include <msp430.h>
#include "mcu_init.h"

inline static void disable_WDT(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
}

inline static void configure_clocks(void)
{
    // --- DCO Clock Configuration ---
    // Use factory calibration values to set the DCO and SMCLK to 16MHz.
    // DCOCTL and BCSCTL1 are the registers to set the DCO frequency.
    // Check if the 16MHz DCO calibration constants are valid
    if (CALBC1_16MHZ != 0xFF) {
        // Load the calibration values to set the DCO to 16MHz
        DCOCTL = CALDCO_16MHZ;
        BCSCTL1 = CALBC1_16MHZ;
    }

    // Set SMCLK to be sourced from the DCO, with a divider of 1.
    // This is often the default, but it's good practice to be explicit.
    BCSCTL2 = DIVS_0; // SMCLK Divider of 1
}

inline static void enable_interrupts(void)
{
    __enable_interrupt(); // Enable global interrupts
}

void mcu_init(void)
{
    disable_WDT();
    configure_clocks();
    enable_interrupts();
}
