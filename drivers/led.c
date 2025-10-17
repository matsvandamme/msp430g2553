#include <msp430.h>
#include "led.h"
#include "../common/defines.h"

volatile bool led_state = false;
volatile uint32_t counter = 0;

volatile led_t leds[] = {
    {
        .io = IO_LED_GREEN,
        .state = LED_OFF,
        .on_period_ms = LED_ON_PERIOD_MS,
        .off_period_ms = LED_OFF_PERIOD_MS,
        .is_blinking = false,
    },
    {
        .io = IO_LED_RED,
        .state = LED_OFF,
        .on_period_ms = LED_ON_PERIOD_MS,
        .off_period_ms = LED_OFF_PERIOD_MS,
        .is_blinking = false,
    }
};

void led_init(void)
{
    // Declare config struct that is common to led and green leds
    gpio_config_t cfg = {
        .dir = IO_DIR_OUTPUT, // Set direction to output
        .out = IO_OUT_LOW, // Set output to low by default
        .resistor = IO_RESISTOR_DISABLED, // Disable pullup/pulldown resistor
        .select = IO_SELECT_GPIO, // Select gpio option
    };

    // Call the gpio configure function for all leds
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_configure(leds[i].io, &cfg);
    }

    /* Timer A0 Configuration
    *  TASSEL_2: Use SMCLK as the clock source (16MHz)
    *  ID_3: Use a clock divider of 8 (16MHz / 8 = 2MHz)
    *  MC_1: Count up to the value in TA0CCR0
    *  TACLR: Clear the timer to start from 0
    */
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

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

    // Enable the timer interrupt
    TA0CCTL0 |= CCIE; // Enable interrupt for capture/compare register
}

void led_set_state(led_t * led, led_state_e state)
{
    switch (state)
    {
    case LED_OFF:
        // Set led to off state
        gpio_set_out(led->io, IO_OUT_LOW);
        led->state = LED_OFF;
        break;
    case LED_ON:
        // Set led to on state
        gpio_set_out(led->io, IO_OUT_HIGH);
        led->state = LED_ON;
        break;
    }
}

void led_start_blinking(led_t * led, uint16_t on_period_ms, uint16_t off_period_ms)
{
    led->on_period_ms = on_period_ms;
    led->off_period_ms = off_period_ms;
    led->is_blinking = true;
    led->last_toggle_time = counter;
    // Start with LED ON
    led_set_state(led, LED_ON);
}

void led_stop_blinking(led_t * led)
{
    led->is_blinking = false;
    // Ensure LED is turned off when stopping blinking
    led_set_state(led, LED_OFF);
}

void led_handle_blinking(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        led_t * led = (led_t *)&leds[i];
        if (led->is_blinking) {
            uint32_t elapsed_time = counter - led->last_toggle_time;
            if (led->state == LED_ON && elapsed_time >= led->on_period_ms) {
                // Turn off the LED
                led_set_state(led, LED_OFF);
                led->last_toggle_time = counter;
            } else if (led->state == LED_OFF && elapsed_time >= led->off_period_ms) {
                // Turn on the LED
                led_set_state(led, LED_ON);
                led->last_toggle_time = counter;
            }
        }
    }
}

// Timer A0 interrupt service routine - MSP-GCC style
// This ISR is called every 1ms
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void Timer0_A0_ISR(void)
{
    // For CCR0 the interrupt flag is cleared automatically by the hardware.
    // Increment the millisecond counter.
    counter++;
}