#include "led.h"
#include <msp430.h>
#include <stddef.h>
#include "../common/defines.h" // Assuming ARRAY_SIZE and GPIO definitions are here

// --- Private Module Constants ---
#define SMCLK_FREQ_HZ       16000000UL
#define TIMER_CLK_DIVIDER   8
#define TIMER_FREQ_HZ       (SMCLK_FREQ_HZ / TIMER_CLK_DIVIDER) // 2 MHz
#define TIMER_PERIOD_MS     1
#define TIMER_COUNTS_PER_MS (TIMER_FREQ_HZ / 1000) // 2000 counts

// --- Private Module Variables ---

/**
 * @brief Millisecond system tick counter, incremented by the Timer A0 ISR.
 * @warning This variable is modified by an ISR. Access from the main context
 * must be atomic (i.e., performed with interrupts disabled).
 */
static volatile uint32_t system_tick_ms = 0;

/**
 * @brief Array of LED control structures. Declared 'static' to encapsulate it
 * within this module, preventing direct external access.
 */
static volatile led_t leds[] = {
    {
        .io = IO_LED_GREEN,
        .state = LED_OFF,
        .on_period_ms = LED_ON_PERIOD_MS_DEFAULT,
        .off_period_ms = LED_OFF_PERIOD_MS_DEFAULT,
        .is_blinking = false,
        .last_toggle_time = 0,
    },
    {
        .io = IO_LED_RED,
        .state = LED_OFF,
        .on_period_ms = LED_ON_PERIOD_MS_DEFAULT,
        .off_period_ms = LED_OFF_PERIOD_MS_DEFAULT,
        .is_blinking = false,
        .last_toggle_time = 0,
    }
};

// --- Public Function Definitions ---

void led_init(void)
{
    // Configure GPIO pins for all LEDs as outputs, initially turned off for power saving
    gpio_config_t cfg = {
        .dir = IO_DIR_OUTPUT,
        .out = IO_OUT_LOW,
        .resistor = IO_RESISTOR_DISABLED,
        .select = IO_SELECT_GPIO,
    };

    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_configure(leds[i].io, &cfg);
    }

    /*
     * Configure Timer A0 to generate a 1ms interrupt for the system tick.
     * - Clock Source: SMCLK (TASSEL_2)
     * - Clock Divider: /8 (ID_3), resulting in a 2MHz timer clock.
     * - Mode: Up mode (MC_1), counts from 0 up to TA0CCR0.
     * - Clear Timer: TACLR, to start from a known state.
     *
     * To get a 1ms period:
     * TA0CCR0 = (Ticks per ms * Period in ms) - 1
     * TA0CCR0 = (2000 * 1) - 1 = 1999
     */
    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR;
    TA0CCR0 = (TIMER_COUNTS_PER_MS * TIMER_PERIOD_MS) - 1;
    TA0CCTL0 |= CCIE; // Enable the Capture/Compare Register 0 interrupt
}

led_t* led_get_handle(gpio_e io)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        if (leds[i].io == io) {
            // Cast away volatile as the handle is intended for use in the
            // non-interrupt context, where access is controlled.
            return (led_t*)&leds[i];
        }
    }
    return NULL; // Return NULL if no matching LED is found
}

void led_set_state(led_t *led, led_state_e state)
{
    if (led == NULL) return;

    if (state == LED_ON) {
        gpio_set_out(led->io, IO_OUT_HIGH);
        led->state = LED_ON;
    } else {
        gpio_set_out(led->io, IO_OUT_LOW);
        led->state = LED_OFF;
    }
}

void led_start_blinking(led_t *led, uint16_t on_period_ms, uint16_t off_period_ms)
{
    if (led == NULL) return;

    led->on_period_ms = on_period_ms;
    led->off_period_ms = off_period_ms;
    
    // --- Critical Section: Atomically read the system tick ---
    __disable_interrupt(); // Or use compiler intrinsic like _DINT()
    led->last_toggle_time = system_tick_ms;
    __enable_interrupt(); // Or _EINT()
    // --- End Critical Section ---

    led->is_blinking = true;
    led_set_state(led, LED_ON); // Start the blinking sequence with the LED ON
}

void led_stop_blinking(led_t *led)
{
    if (led == NULL) return;

    led->is_blinking = false;
    led_set_state(led, LED_OFF); // Ensure LED is off when blinking stops
}

void led_handle_blinking(void)
{
    uint32_t current_time;

    // --- Critical Section: Atomically read the shared system tick variable ---
    __disable_interrupt();
    current_time = system_tick_ms;
    __enable_interrupt();
    // --- End Critical Section ---

    for (uint8_t i = 0; i < ARRAY_SIZE(leds); i++) {
        // Use a non-volatile pointer for manipulation within this function
        led_t *led = (led_t*)&leds[i];

        if (led->is_blinking) {
            // Unsigned subtraction correctly handles timer rollover
            uint32_t elapsed_time = current_time - led->last_toggle_time;

            if ((led->state == LED_ON) && (elapsed_time >= led->on_period_ms)) {
                led_set_state(led, LED_OFF);
                led->last_toggle_time = current_time;
            } else if ((led->state == LED_OFF) && (elapsed_time >= led->off_period_ms)) {
                led_set_state(led, LED_ON);
                led->last_toggle_time = current_time;
            }
        }
    }
}

// --- Interrupt Service Routine ---

/**
 * @brief Timer A0 Interrupt Service Routine (ISR).
 *
 * This ISR is triggered every 1ms by the TA0CCR0 match. It increments
 * the global millisecond counter, which serves as the system tick for
 * time-dependent tasks like LED blinking.
 */
__attribute__((interrupt(TIMER0_A0_VECTOR)))
void Timer0_A0_ISR(void)
{
    // For CCR0, the hardware automatically clears the interrupt flag (CCIFG).
    system_tick_ms++;
}