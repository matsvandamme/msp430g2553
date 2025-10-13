#ifndef GPIO_H
#define GPIO_H

// This header is required for access to MSP430-specific register definitions
#include <msp430.h>
// This header is required for standard integer types like uint8_t
#include <stdint.h>

/**
 * @enum gpio_e
 * @brief Generic enumeration for GPIO pins, abstracting the port and pin number.
 * * The enum values are structured to encode both the port and pin information.
 * For example, IO_10 represents Port 1, Pin 0, while IO_27 represents Port 2, Pin 7.
 */
typedef enum {
    IO_10, IO_11, IO_12, IO_13, IO_14, IO_15, IO_16, IO_17,
    IO_20, IO_21, IO_22, IO_23, IO_24, IO_25, IO_26, IO_27,
} gpio_generic_e;

typedef enum
{
    IO_UART_RX = IO_11,
    IO_UART_TX = IO_12,
    IO_LED_RED = IO_10,
    IO_LED_GREEN = IO_16,
} gpio_e;

/**
 * @enum gpio_select_e
 * @brief Enumeration for selecting the pin's function.
 * * Pins can be configured as a general-purpose I/O (GPIO) or for an alternate function
 * such as a peripheral (e.g., ADC, Timer).
 */
typedef enum
{
    IO_SELECT_GPIO,
    IO_SELECT_ALT1,
    IO_SELECT_ALT2,
    IO_SELECT_ALT3,
} gpio_select_e;

/**
 * @enum gpio_dir_e
 * @brief Enumeration for setting the pin's data direction.
 */
typedef enum
{
    IO_DIR_INPUT,
    IO_DIR_OUTPUT,
} gpio_dir_e;

/**
 * @enum gpio_resistor_e
 * @brief Enumeration for enabling or disabling the internal pull-up/pull-down resistor.
 */
typedef enum
{
    IO_RESISTOR_DISABLED,
    IO_RESISTOR_ENABLED,
} gpio_resistor_e;

/**
 * @enum gpio_out_e
 * @brief Enumeration for setting the output state of a pin.
 * * This also determines the state of the internal resistor (pull-up/pull-down)
 * when configured as an input with the resistor enabled.
 */
typedef enum
{
    IO_OUT_LOW, // (pull-down)
    IO_OUT_HIGH, // (pull-up)
} gpio_out_e;

/**
 * @enum gpio_in_e
 * @brief Enumeration for representing the input state of a pin.
 */
typedef enum
{
    IO_IN_LOW,
    IO_IN_HIGH,
} gpio_in_e;

/**
 * @enum gpio_trigger_e
 * @brief Enumeration for configuring interrupt triggers (not implemented in the C file).
 */
typedef enum
{
    IO_TRIGGER_RISING,
    IO_TRIGGER_FALLING
} gpio_trigger_e;

/**
 * @struct gpio_config_t
 * @brief A structure for holding the complete configuration of a GPIO pin.
 */
typedef struct 
{
    gpio_select_e select;
    gpio_resistor_e resistor;
    gpio_dir_e dir;
    gpio_out_e out;
} gpio_config_t;

/**
 * @brief Initializes the GPIO module (function not implemented in the C file).
 */
void gpio_init(void);

/**
 * @brief Configures a single GPIO pin using a configuration structure.
 * * @param gpio The generic pin to configure.
 * @param config A pointer to the configuration structure.
 */
void gpio_configure(gpio_e gpio, const gpio_config_t *config);

/**
 * @brief Sets the function of a specific GPIO pin.
 * * @param gpio The generic pin to configure.
 * @param select The desired function (e.g., GPIO, ALT1).
 */
void gpio_set_select(gpio_e gpio, gpio_select_e select);

/**
 * @brief Sets the data direction (input or output) of a specific GPIO pin.
 * * @param gpio The generic pin to configure.
 * @param direction The desired direction.
 */
void gpio_set_direction(gpio_e gpio, gpio_dir_e direction);

/**
 * @brief Enables or disables the internal pull-up/pull-down resistor.
 * * @param gpio The generic pin to configure.
 * @param resistor The desired resistor state.
 */
void gpio_set_resistor(gpio_e gpio, gpio_resistor_e resistor);

/**
 * @brief Sets the output state (high or low) of a specific GPIO pin.
 * * This function also sets the state of the pull-up/pull-down resistor
 * when the pin is configured as an input.
 * * @param gpio The generic pin to configure.
 * @param out The desired output state.
 */
void gpio_set_out(gpio_e gpio, gpio_out_e out);

/**
 * @brief Reads the input state of a specific GPIO pin.
 * * @param gpio The generic pin to read.
 * @return The input state (high or low).
 */
gpio_in_e gpio_get_input(gpio_e gpio);

#endif // GPIO_H
