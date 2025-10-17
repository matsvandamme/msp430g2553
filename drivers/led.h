/**
 * @file led.h
 * @brief LED driver for handling GPIO-based LEDs, including non-blocking blinking.
 *
 * This module initializes and controls LEDs connected to GPIO pins. It relies
 * on a periodic timer interrupt (system tick) to provide non-blocking
 * blinking functionality.
 */
#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h" // Assuming gpio.h defines gpio_e, gpio_config_t, etc.

// Default blinking periods, can be overridden by led_start_blinking()
#define LED_ON_PERIOD_MS_DEFAULT 800
#define LED_OFF_PERIOD_MS_DEFAULT 200

/**
 * @brief Defines the possible states of an LED.
 */
typedef enum {
    LED_OFF, ///< The LED is turned off (GPIO logic low).
    LED_ON,  ///< The LED is turned on (GPIO logic high).
} led_state_e;

/**
 * @brief Structure to hold the state and configuration of a single LED.
 * @note The 'io' field is constant and set at compile time.
 */
typedef struct
{
    const gpio_e io;        ///< GPIO pin associated with the LED (read-only).
    led_state_e state;      ///< Current physical state of the LED (ON or OFF).
    uint16_t on_period_ms;  ///< Duration (ms) the LED stays ON during blinking.
    uint16_t off_period_ms; ///< Duration (ms) the LED stays OFF during blinking.
    bool is_blinking;       ///< Flag indicating if the LED is currently blinking.
    uint32_t last_toggle_time; ///< System time (ms) when the LED state was last changed.
} led_t;

/**
 * @brief Initializes the GPIO pins for all LEDs and configures the system tick timer.
 * @note This function must be called once before any other LED function.
 */
void led_init(void);

/**
 * @brief Sets the state of a specific LED (on or off).
 * @param led Pointer to the LED control structure. Must not be NULL.
 * @param state The desired state (LED_ON or LED_OFF).
 */
void led_set_state(led_t *led, led_state_e state);

/**
 * @brief Starts blinking an LED with specified on/off periods.
 * @param led Pointer to the LED control structure. Must not be NULL.
 * @param on_period_ms The time in milliseconds the LED should be ON.
 * @param off_period_ms The time in milliseconds the LED should be OFF.
 */
void led_start_blinking(led_t *led, uint16_t on_period_ms, uint16_t off_period_ms);

/**
 * @brief Stops a currently blinking LED and turns it off.
 * @param led Pointer to the LED control structure. Must not be NULL.
 */
void led_stop_blinking(led_t *led);

/**
 * @brief Handles the state machine for all blinking LEDs.
 * @note This function must be called periodically in the main application loop
 * to update the state of blinking LEDs.
 */
void led_handle_blinking(void);

/**
 * @brief Gets a handle to an LED structure by its GPIO pin enum.
 * @param io The GPIO enum (e.g., IO_LED_GREEN) for the desired LED.
 * @return A pointer to the LED's control structure, or NULL if not found.
 */
led_t* led_get_handle(gpio_e io);

#endif // LED_H