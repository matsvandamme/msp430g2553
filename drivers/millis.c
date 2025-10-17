#include <msp430.h>
#include <stdint.h>
#include "millis.h"

// Volatile is crucial: tells the compiler that this value can change at any time
// (e.g., from an interrupt) and prevents incorrect optimizations.
static volatile uint32_t millis_counter = 0;

// This variable handles the fractional part of a millisecond.
static volatile uint16_t millis_remainder_us = 0;

/**
 * @brief Initializes the watchdog timer as a millisecond interval timer.
 */
void millis_init(void) {
    // Stop WDT to prevent reset
    WDTCTL = WDTPW | WDTHOLD;

    // --- CHANGE 2: Configure WDT Divider for 16MHz Clock ---
    // We want an interrupt interval close to what we had before.
    // SMCLK is now 16MHz. We use the /8192 divider.
    // Interval = 8192 cycles / 16,000,000 cycles/sec = 0.000512 seconds = 512 microseconds.
    // This gives us the same 512µs tick rate as the 1MHz version.
    WDTCTL = WDT_MDLY_8;

    // Enable WDT interrupt
    IE1 |= WDTIE;
}

/**
 * @brief Returns the number of milliseconds since program start.
 */
uint32_t millis(void) {
    uint32_t ms;

    // Disable interrupts to ensure we get a consistent, multi-byte value.
    // This prevents the ISR from modifying millis_counter while we are reading it.
    __disable_interrupt();
    ms = millis_counter;
    __enable_interrupt();

    return ms;
}

/**
 * @brief Pauses the program for a number of milliseconds (busy-wait).
 */
void delay_ms(uint32_t ms) {
    uint32_t start = millis();
    // Unsigned subtraction handles counter rollover correctly.
    while (millis() - start < ms);
}

/**
 * @brief Watchdog Timer Interrupt Service Routine.
 *
 * This ISR is triggered every 512 microseconds.
 * It accumulates these short intervals to count full milliseconds.
 * NOTE: This logic remains IDENTICAL to the 1MHz version because we
 * adjusted the WDT divider to keep the same 512µs interval.
 */
__attribute__((interrupt(WDT_VECTOR))) void watchdog_timer(void) {
    // Add the 512 microsecond interval to our remainder accumulator.
    millis_remainder_us += 512;

    // If we have accumulated at least a full millisecond (1000µs)...
    if (millis_remainder_us >= 1000) {
        // ...increment the main millisecond counter...
        millis_counter++;
        // ...and subtract exactly one millisecond from the remainder.
        millis_remainder_us -= 1000;
    }
}
