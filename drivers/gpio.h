/**
 * @file gpio.h
 * @brief Public interface for the GPIO (General-Purpose Input/Output) driver.
 *
 * This file defines the functions, enumerations, and structures required to
 * configure and control GPIO pins on an MSP430 microcontroller. It provides an
 * abstraction layer over the hardware registers.
 */
#ifndef GPIO_H
#define GPIO_H

// This header is required for access to MSP430-specific register definitions
#include <msp430.h>
// This header is required for standard integer types like uint8_t
#include <stdint.h>

/**
 * @enum gpio_generic_e
 * @brief Generic enumeration for all available GPIO pins.
 *
 * This enum provides a unique identifier for each pin. The values are encoded
 * to contain both the port and pin number.
 * - Bits 3-4: Port number (0 for Port 1, 1 for Port 2).
 * - Bits 0-2: Pin number (0-7).
 * For example, IO_10 represents Port 1, Pin 0. IO_27 represents Port 2, Pin 7.
 */
typedef enum {
    IO_10, IO_11, IO_12, IO_13, IO_14, IO_15, IO_16, IO_17,
    IO_20, IO_21, IO_22, IO_23, IO_24, IO_25, IO_26, IO_27,
} gpio_generic_e;

/**
 * @enum gpio_e
 * @brief Application-specific aliases for GPIO pins.
 *
 * This enumeration assigns meaningful names to GPIO pins based on their
 * function in the application (e.g., controlling an LED or for UART communication).
 */
typedef enum
{
    IO_UART_RX = IO_11,     ///< UART Receive Pin
    IO_UART_TX = IO_12,     ///< UART Transmit Pin
    IO_LED_RED = IO_10,     ///< Red LED
    IO_LED_GREEN = IO_16,   ///< Green LED
} gpio_e;

/**
 * @enum gpio_select_e
 * @brief Enumeration for selecting the pin's function.
 *
 * Each pin can be configured either as a general-purpose I/O (GPIO) or for an
 * alternate peripheral function (e.g., ADC, Timer, UART). Refer to the
 * device-specific datasheet for details on which alternate functions are
 * available on each pin.
 */
typedef enum
{
    IO_SELECT_GPIO, ///< Pin is configured for general-purpose I/O.
    IO_SELECT_ALT1, ///< Pin is configured for primary peripheral module function.
    IO_SELECT_ALT2, ///< Pin is configured for secondary peripheral module function.
    IO_SELECT_ALT3, ///< Pin is configured for tertiary peripheral module function.
} gpio_select_e;

/**
 * @enum gpio_dir_e
 * @brief Enumeration for setting the pin's data direction.
 */
typedef enum
{
    IO_DIR_INPUT,  ///< The pin is configured as a digital input.
    IO_DIR_OUTPUT, ///< The pin is configured as a digital output.
} gpio_dir_e;

/**
 * @enum gpio_resistor_e
 * @brief Enumeration for enabling/disabling the internal pull-up/pull-down resistor.
 */
typedef enum
{
    IO_RESISTOR_DISABLED, ///< Internal pull-up/pull-down resistor is disabled.
    IO_RESISTOR_ENABLED,  ///< Internal pull-up/pull-down resistor is enabled.
} gpio_resistor_e;

/**
 * @enum gpio_out_e
 * @brief Enumeration for setting the output logic level of a pin.
 *
 * When a pin is configured as an output, this sets the logic level to high or low.
 * When a pin is configured as an input with the internal resistor enabled, this
 * selects between a pull-up (IO_OUT_HIGH) and a pull-down (IO_OUT_LOW) resistor.
 */
typedef enum
{
    IO_OUT_LOW,  ///< Logic low. Selects pull-down resistor if pin is an input.
    IO_OUT_HIGH, ///< Logic high. Selects pull-up resistor if pin is an input.
} gpio_out_e;

/**
 * @enum gpio_in_e
 * @brief Enumeration for representing the logic level of an input pin.
 */
typedef enum
{
    IO_IN_LOW,  ///< The pin is at a logic low level.
    IO_IN_HIGH, ///< The pin is at a logic high level.
} gpio_in_e;

/**
 * @enum gpio_trigger_e
 * @brief Enumeration for configuring interrupt edge triggers.
 * @note This functionality is declared but not implemented in the driver.
 */
typedef enum
{
    IO_TRIGGER_RISING,  ///< Interrupt on a rising edge (low to high).
    IO_TRIGGER_FALLING, ///< Interrupt on a falling edge (high to low).
} gpio_trigger_e;

/**
 * @struct gpio_config_t
 * @brief A structure for holding the complete configuration for a single GPIO pin.
 *
 * This structure is used with the gpio_configure() function to set all
 * major properties of a pin in a single function call.
 */
typedef struct
{
    gpio_select_e select;     ///< Pin function selection (GPIO or alternate).
    gpio_resistor_e resistor; ///< Internal resistor state (enabled/disabled).
    gpio_dir_e dir;           ///< Data direction (input/output).
    gpio_out_e out;           ///< Output level or pull-up/pull-down selection.
} gpio_config_t;

/**
 * @brief Initializes the GPIO module.
 * @note This function is declared but not implemented in the driver.
 * It could be used for global GPIO settings, like unlocking pins.
 */
void gpio_init(void);

/**
 * @brief Configures a single GPIO pin using a configuration structure.
 * @param gpio The application-specific pin to configure (from gpio_e).
 * @param config A pointer to the gpio_config_t structure containing the desired settings.
 */
void gpio_configure(gpio_e gpio, const gpio_config_t *config);

/**
 * @brief Sets the function of a specific GPIO pin (GPIO or peripheral).
 * @param gpio The application-specific pin to configure.
 * @param select The desired function (e.g., IO_SELECT_GPIO, IO_SELECT_ALT1).
 */
void gpio_set_select(gpio_e gpio, gpio_select_e select);

/**
 * @brief Sets the data direction (input or output) of a specific GPIO pin.
 * @param gpio The application-specific pin to configure.
 * @param direction The desired direction (IO_DIR_INPUT or IO_DIR_OUTPUT).
 */
void gpio_set_direction(gpio_e gpio, gpio_dir_e direction);

/**
 * @brief Enables or disables the internal pull-up/pull-down resistor for a pin.
 * @param gpio The application-specific pin to configure.
 * @param resistor The desired resistor state (IO_RESISTOR_ENABLED or IO_RESISTOR_DISABLED).
 */
void gpio_set_resistor(gpio_e gpio, gpio_resistor_e resistor);

/**
 * @brief Sets the output logic level of a specific GPIO pin.
 *
 * If the pin is an output, this drives the pin high or low.
 * If the pin is an input with the resistor enabled, this selects between
 * pull-up (IO_OUT_HIGH) and pull-down (IO_OUT_LOW).
 *
 * @param gpio The application-specific pin to configure.
 * @param out The desired output state (IO_OUT_HIGH or IO_OUT_LOW).
 */
void gpio_set_out(gpio_e gpio, gpio_out_e out);

/**
 * @brief Reads the current logic level of a specific GPIO pin.
 * @param gpio The application-specific pin to read.
 * @return The input state (IO_IN_HIGH or IO_IN_LOW).
 */
gpio_in_e gpio_get_input(gpio_e gpio);

#endif // GPIO_H
