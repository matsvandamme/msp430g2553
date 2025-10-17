#ifndef MILLIS_H
#define MILLIS_H

#include <stdint.h>

/**
 * @brief Initializes the watchdog timer as a millisecond interval timer.
 * * This function configures the DCO to run at 1MHz and sets up the
 * watchdog timer to generate an interrupt periodically. The interrupt
 * service routine handles the counting of milliseconds.
 * * This must be called once before using millis() or delay_ms().
 * Global interrupts are enabled by this function.
 */
void millis_init(void);

/**
 * @brief Returns the number of milliseconds since the program started.
 * * This value is returned as an unsigned 32-bit integer. The counter
 * will overflow and wrap back to 0 after approximately 49.7 days.
 * * @return The number of milliseconds since millis_init() was called.
 */
uint32_t millis(void);

/**
 * @brief Pauses the program for the specified number of milliseconds.
 * * This is a busy-wait delay function. It continuously polls the millis()
 * counter until the desired time has elapsed.
 * * @param ms The number of milliseconds to delay.
 */
void delay_ms(uint32_t ms);

#endif // MILLIS_H
