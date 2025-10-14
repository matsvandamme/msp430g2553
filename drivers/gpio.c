/**
 * @file gpio.c
 * @brief Implementation of the GPIO (General-Purpose Input/Output) driver.
 *
 * This file contains the function definitions for configuring and controlling
 * GPIO pins on an MSP430 microcontroller. It interacts directly with the
 * hardware registers.
 */
#include "gpio.h"
#include "../common/defines.h"

// MACROS to decode the port and pin information from a gpio_e enum value.
// The enum is structured such that:
// - Bits 3-4 represent the port index (0 for Port 1, 1 for Port 2).
// - Bits 0-2 represent the pin index (0-7).

#define IO_PORT_CNT (2u) ///< Number of GPIO ports handled by this driver (P1, P2).
#define IO_PIN_CNT_PER_PORT (8u)
#define IO_PORT_OFFSET (3u) ///< Bit offset for the port number within the enum value.
#define IO_PORT_MASK (0x3u << IO_PORT_OFFSET) ///< Mask to isolate the port number bits.
#define IO_PIN_MASK (0x7u) ///< Mask to isolate the pin number bits.

#define UNUSED_CONFIG                                                                              \
    {                                                                                              \
        IO_SELECT_GPIO, IO_RESISTOR_ENABLED, IO_DIR_OUTPUT, IO_OUT_LOW                             \
    }

// These arrays store pointers to the memory-mapped hardware registers for each port.
// - 'volatile' is used to ensure the compiler does not optimize away register accesses,
//   as their values can change unexpectedly by hardware.
// - 'const' (after the '*') makes the pointers themselves constant; they will always
//   point to the same hardware register address after initialization.
// This array-based approach allows functions to access registers for any port
// using a simple index, making the code more scalable and readable.

/// @brief Array of pointers to the Port Direction registers (P1DIR, P2DIR).
static volatile uint8_t *const port_dir_regs[IO_PORT_CNT] = { &P1DIR, &P2DIR };
/// @brief Array of pointers to the Port Resistor Enable registers (P1REN, P2REN).
static volatile uint8_t *const port_ren_regs[IO_PORT_CNT] = { &P1REN, &P2REN };
/// @brief Array of pointers to the Port Output registers (P1OUT, P2OUT).
static volatile uint8_t *const port_out_regs[IO_PORT_CNT] = { &P1OUT, &P2OUT };
/// @brief Array of pointers to the Port Input registers (P1IN, P2IN).
static volatile uint8_t *const port_in_regs[IO_PORT_CNT] = { &P1IN, &P2IN };
/// @brief Array of pointers to the Port Select 1 registers (P1SEL, P2SEL).
static volatile uint8_t *const port_sel1_regs[IO_PORT_CNT] = { &P1SEL, &P2SEL };
/// @brief Array of pointers to the Port Select 2 registers (P1SEL2, P2SEL2).
static volatile uint8_t *const port_sel2_regs[IO_PORT_CNT] = { &P1SEL2, &P2SEL2 };

// This array holds the initial configuration for all IO pins.
static const gpio_config_t io_initial_configs[IO_PORT_CNT * IO_PIN_CNT_PER_PORT] = {
    // Output
    [IO_UART_RX] = { IO_SELECT_GPIO, IO_RESISTOR_DISABLED, IO_DIR_OUTPUT, IO_OUT_LOW },
    [IO_UART_TX] = { IO_SELECT_GPIO, IO_RESISTOR_DISABLED, IO_DIR_OUTPUT, IO_OUT_LOW },
    [IO_LED_GREEN] = { IO_SELECT_GPIO, IO_RESISTOR_DISABLED, IO_DIR_OUTPUT, IO_OUT_LOW },
    [IO_LED_RED] = { IO_SELECT_GPIO, IO_RESISTOR_DISABLED, IO_DIR_OUTPUT, IO_OUT_LOW },
    [IO_UNUSED_1] = UNUSED_CONFIG,
    [IO_UNUSED_2] = UNUSED_CONFIG,
    [IO_UNUSED_3] = UNUSED_CONFIG,
    [IO_UNUSED_4] = UNUSED_CONFIG,
    [IO_UNUSED_5] = UNUSED_CONFIG,
    [IO_UNUSED_6] = UNUSED_CONFIG,
    [IO_UNUSED_7] = UNUSED_CONFIG,
    [IO_UNUSED_8] = UNUSED_CONFIG,
    [IO_UNUSED_9] = UNUSED_CONFIG,
    [IO_UNUSED_10] = UNUSED_CONFIG,
    [IO_UNUSED_11] = UNUSED_CONFIG,
    [IO_UNUSED_12] = UNUSED_CONFIG,
};

/**
 * @brief Extracts the port index from a gpio_e enum value.
 * @param gpio The GPIO pin identifier.
 * @return The port index (0 for Port 1, 1 for Port 2).
 */
static uint8_t gpio_port(gpio_e gpio)
{
    return (gpio & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

/**
 * @brief Extracts the pin index (0-7) from a gpio_e enum value.
 * @param gpio The GPIO pin identifier.
 * @return The pin index within its port (0-7).
 */
static inline uint8_t gpio_pin_idx(gpio_e gpio)
{
    return gpio & IO_PIN_MASK;
}

/**
 * @brief Creates a bitmask for a specific pin from a gpio_e enum value.
 *
 * This is used to read or modify the specific bit corresponding to a pin
 * within a hardware register.
 *
 * @param gpio The GPIO pin identifier.
 * @return A bitmask with a single bit set (e.g., 0b00000001 for pin 0).
 */
static uint8_t gpio_pin_bit(gpio_e gpio)
{
    return 1 << gpio_pin_idx(gpio);
}

void gpio_init(void)
{
    for (gpio_e io = (gpio_e)IO_10; io < ARRAY_SIZE(io_initial_configs); io++) {
        gpio_configure(io, &io_initial_configs[io]);
    }
}

/**
 * @brief Sets the data direction (input or output) of a specific GPIO pin.
 * @param gpio The application-specific pin to configure.
 * @param direction The desired direction (IO_DIR_INPUT or IO_DIR_OUTPUT).
 */
void gpio_set_direction(gpio_e gpio, gpio_dir_e direction)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch (direction) {
    case IO_DIR_INPUT:
        // Clear the corresponding bit in the Port Direction register (PxDIR) to set as input.
        *port_dir_regs[port] &= ~pin;
        break;
    case IO_DIR_OUTPUT:
        // Set the corresponding bit in the Port Direction register (PxDIR) to set as output.
        *port_dir_regs[port] |= pin;
        break;
    }
}

/**
 * @brief Enables or disables the internal pull-up/pull-down resistor for a pin.
 * @param gpio The application-specific pin to configure.
 * @param resistor The desired resistor state (IO_RESISTOR_ENABLED or IO_RESISTOR_DISABLED).
 */
void gpio_set_resistor(gpio_e gpio, gpio_resistor_e resistor)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch (resistor) {
    case IO_RESISTOR_DISABLED:
        // Clear the corresponding bit in the Port Resistor Enable register (PxREN).
        *port_ren_regs[port] &= ~pin;
        break;
    case IO_RESISTOR_ENABLED:
        // Set the corresponding bit in the Port Resistor Enable register (PxREN).
        *port_ren_regs[port] |= pin;
        break;
    }
}

/**
 * @brief Sets the function of a specific GPIO pin (GPIO or peripheral).
 *
 * This function modifies the PxSEL and PxSEL2 registers to select the desired
 * function according to the device datasheet.
 *
 * @param gpio The application-specific pin to configure.
 * @param select The desired function (e.g., IO_SELECT_GPIO, IO_SELECT_ALT1).
 */
void gpio_set_select(gpio_e gpio, gpio_select_e select)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch (select) {
    case IO_SELECT_GPIO:
        // PxSEL=0, PxSEL2=0 for General-purpose I/O.
        *port_sel1_regs[port] &= ~pin;
        *port_sel2_regs[port] &= ~pin;
        break;
    case IO_SELECT_ALT1:
        // PxSEL=1, PxSEL2=0 for Primary peripheral module function.
        *port_sel1_regs[port] |= pin;
        *port_sel2_regs[port] &= ~pin;
        break;
    case IO_SELECT_ALT2:
        // PxSEL=0, PxSEL2=1 for Secondary peripheral module function.
        *port_sel1_regs[port] &= ~pin;
        *port_sel2_regs[port] |= pin;
        break;
    case IO_SELECT_ALT3:
        // PxSEL=1, PxSEL2=1 for Tertiary peripheral module function.
        *port_sel1_regs[port] |= pin;
        *port_sel2_regs[port] |= pin;
        break;
    }
}

/**
 * @brief Sets the output logic level of a specific GPIO pin.
 *
 * If the pin is an output, this drives the pin high or low via the PxOUT register.
 * If the pin is an input with the resistor enabled, this selects between
 * pull-up (IO_OUT_HIGH) and pull-down (IO_OUT_LOW) via the same PxOUT register.
 *
 * @param gpio The application-specific pin to configure.
 * @param out The desired output state (IO_OUT_HIGH or IO_OUT_LOW).
 */
void gpio_set_out(gpio_e gpio, gpio_out_e out)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch (out) {
    case IO_OUT_LOW:
        // Clear the corresponding bit in the Port Output register (PxOUT).
        *port_out_regs[port] &= ~pin;
        break;
    case IO_OUT_HIGH:
        // Set the corresponding bit in the Port Output register (PxOUT).
        *port_out_regs[port] |= pin;
        break;
    }
}

/**
 * @brief Reads the current logic level of a specific GPIO pin.
 * @param gpio The application-specific pin to read.
 * @return The input state (IO_IN_HIGH or IO_IN_LOW).
 */
gpio_in_e gpio_get_input(gpio_e gpio)
{
    // Read the Port Input register (PxIN) and check if the pin's bit is set.
    return (*port_in_regs[gpio_port(gpio)] & gpio_pin_bit(gpio)) ? IO_IN_HIGH : IO_IN_LOW;
}

/**
 * @brief Configures a single GPIO pin using a configuration structure.
 *
 * This function provides a convenient way to set all major properties of a pin
 * by calling the individual configuration functions.
 *
 * @param gpio The application-specific pin to configure (from gpio_e).
 * @param config A pointer to the gpio_config_t structure containing the desired settings.
 */
void gpio_configure(gpio_e gpio, const gpio_config_t *config)
{
    gpio_set_select(gpio, config->select);
    gpio_set_resistor(gpio, config->resistor);
    gpio_set_direction(gpio, config->dir);
    gpio_set_out(gpio, config->out);
}
