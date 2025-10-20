/**
 * @file led.h
 * @brief LED driver for handling GPIO-based LEDs, including non-blocking blinking.
 *
 * This module initializes and controls LEDs connected to GPIO pins. It relies
 * on a periodic timer interrupt to provide non-blocking blinking functionality.
 * The internal state of an LED is encapsulated and can only be manipulated
 * through the provided API using an opaque handle.
 */
#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h" // Assuming gpio.h defines gpio_e

// --- Public Constants ---

/**
 * @brief Default blinking periods, can be overridden by led_start_blinking().
 */
#define LED_ON_PERIOD_MS_DEFAULT 800
#define LED_OFF_PERIOD_MS_DEFAULT 200

// --- Public Type Definitions ---

/**
 * @brief Defines the possible physical states of an LED.
 */
typedef enum {
    LED_OFF, ///< The LED is turned off (GPIO logic low).
    LED_ON,  ///< The LED is turned on (GPIO logic high).
} led_state_e;

/**
 * @brief Opaque handle for an LED object.
 *
 * This is a pointer to an incomplete type, hiding the internal structure
 * of the LED object from the user. Use led_get_handle() to obtain a
 * valid handle to an LED instance.
 */
typedef struct led_s *led_handle_t;

// --- Public Function Prototypes ---

/**
 * @brief Initializes GPIO pins for all LEDs and the system tick timer.
 * @note This function must be called once before any other LED function.
 */
void led_init(void);

/**
 * @brief Sets the state of a specific LED (on or off) and stops any blinking.
 * @param handle Handle to the LED object. Must not be NULL.
 * @param state The desired state (LED_ON or LED_OFF).
 */
void led_set_state(led_handle_t handle, led_state_e state);

/**
 * @brief Starts blinking an LED with specified on/off periods.
 * @param handle Handle to the LED object. Must not be NULL.
 * @param on_period_ms The time in milliseconds the LED should be ON.
 * @param off_period_ms The time in milliseconds the LED should be OFF.
 */
void led_start_blinking(led_handle_t handle, uint16_t on_period_ms, uint16_t off_period_ms);

/**
 * @brief Stops a currently blinking LED and turns it off.
 * @param handle Handle to the LED object. Must not be NULL.
 */
void led_stop_blinking(led_handle_t handle);

/**
 * @brief Handles the state machine for all blinking LEDs.
 * @note This function must be called periodically in the main application loop
 * to update the state of any blinking LEDs.
 */
void led_handle_blinking(void);

/**
 * @brief Gets a handle to an LED object by its associated GPIO pin enum.
 * @param io The GPIO enum (e.g., IO_LED_GREEN) for the desired LED.
 * @return A handle to the LED's control object, or NULL if not found.
 */
led_handle_t led_get_handle(gpio_e io);

#endif // LED_H