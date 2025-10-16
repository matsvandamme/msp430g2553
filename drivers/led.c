#include <msp430.h>
#include "led.h"
#include "gpio.h"
#include "../common/defines.h"

volatile bool led_state = false;
volatile uint32_t counter = 0;

void led_init(void)
{
    // Declare coxnfig struct that is common to led and green leds
    gpio_config_t cfg = {
        .dir = IO_DIR_OUTPUT, // Set direction to output
        .out = IO_OUT_LOW, // Set output to low by default
        .resistor = IO_RESISTOR_DISABLED, // Disable pullup/pulldown resistor
        .select = IO_SELECT_GPIO, // Select gpio option
    };

    // Call the gpio configure function for both leds
    gpio_configure(IO_LED_GREEN, &cfg);
    gpio_configure(IO_LED_RED, &cfg);

    // --- DCO Clock Configuration ---
    // Use factory calibration values to set the DCO and SMCLK to 16MHz.
    // DCOCTL and BCSCTL1 are the registers to set the DCO frequency.
    DCOCTL = 0;
    BCSCTL1 = CALBC1_16MHZ;
    DCOCTL = CALDCO_16MHZ;

    // Set SMCLK to be sourced from the DCO, with a divider of 1.
    // This is often the default, but it's good practice to be explicit.
    BCSCTL2 = DIVS_0; // SMCLK Divider of 1

    /* Repurpose the watchdog timer for blinking leds using interrupts
    *  TASSEL_2: Use SMCLK as the clock source (16MHz)
    *  ID_3: Use a clock divider of 8 (16MHz / 8 = 2MHz)
    *  MC_1: Count up to the value in TA0CCR0// TASSEL_2: Use SMCLK as the clock source (16MHz) */
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR; // SMCLK, /8 divider, up mode, clear timer

    /* Set the capture/compare register for a 1ms period
    *  The timer clock is now 2MHz (16MHz / 8)
    *  2MHz / 2000 = 1kHz = 1ms period
    *  The formula is: TimerPeriod = (TimerClock / ( TA0CCR0 + 1 ))
    *  To get a 1ms period, we need counts of 2000
    *  1ms = (2,000,000 / ( TA0CCR0 + 1 ))
    *  0.001s = (2,000,000 / ( TA0CCR0 + 1 ))
    *  ( TACCR0 + 1 ) = ( 2,000,000 * 0.001s )
    *  TACCR0 = 2000 - 1
    *  TACCR0 = 1999 */
    TA0CCR0 = 1999; // 2MHz / 2000 = 1kHz = 1ms period
    // Enable global interrupts
    // Enable the timer interrupt
    TA0CCTL0 = CCIE; // Enable interrupt for capture/compare register
    __bis_SR_register(GIE);
}

void led_set_state(led_colour_e colour, led_state_e state)
{
    switch (colour)
    {
    case LED_GREEN:
        // The user wants to change the state of the green led
        switch (state)
        {
        case LED_OFF:
            // Set led to off state
            gpio_set_out(IO_LED_GREEN, IO_OUT_LOW);
            break;
        case LED_ON:
            // Set led to on state
            gpio_set_out(IO_LED_GREEN, IO_OUT_HIGH);
            break;
        }
        break;
    case LED_RED:
        // The user wants to change the state of the red led
        switch (state)
        {
        case LED_OFF:
            // Set led to off state
            gpio_set_out(IO_LED_RED, IO_OUT_LOW);
            break;
        case LED_ON:
            // Set led to on state
            gpio_set_out(IO_LED_RED, IO_OUT_HIGH);
            break;
        }
        break;
    }
}

void led_handle_blinking(void)
{
    if (led_state == true) {
        // LED is currently ON
        if (counter >= LED_ON_PERIOD_MS) {
            // Time to turn it OFF
            led_set_state(LED_GREEN, LED_OFF);
            led_state = false;
            counter = 0;
        }
    } else {
        // LED is currently OFF
        if (counter >= LED_OFF_PERIOD_MS) {
            // Time to turn it ON
            led_set_state(LED_GREEN, LED_ON);
            led_state = true;
            counter = 0;
        }
    }
}

// Timer A0 interrupt service routine
// This ISR is called every 1ms
INTERRUPT_VECTOR(TIMER0_A0_VECTOR) void WDT_ISR (void) {
    // Clear the interrupt flag (hardware does this automatically for CCR0 but good practice)
    TA0CCTL0 &= ~CCIFG;
    counter++;
}