#include "gpio.h"
#include "defines.h"

// The following macros are used to decode the gpio_e enum value
// to extract the port and pin information. The enum values are structured
// with the port in bits 3-4 and the pin in bits 0-2.

// Number of GPIO ports handled by this driver
#define IO_PORT_CNT (2u)
// Bit offset for the port number within the enum value
#define IO_PORT_OFFSET (3u)
// Mask to isolate the port number
#define IO_PORT_MASK (0x3u << IO_PORT_OFFSET)
// Mask to isolate the pin number
#define IO_PIN_MASK (0x7u)

// Array of pointers to the Port Direction registers (P1DIR, P2DIR)
/*
* const: This keyword, placed after the asterisk, means that the pointers themselves are constant. 
* You can't change the address stored in port_dir_regs[0] to point to a different register after initialization. 
* However, it does not prevent you from modifying the value at the address being pointed to (i.e., you can still change the contents of P1DIR and P2DIR).
*/
static volatile uint8_t *const port_dir_regs[IO_PORT_CNT] = { &P1DIR, &P2DIR };
// Array of pointers to the Port Resistor Enable registers (P1REN, P2REN)
static volatile uint8_t *const port_ren_regs[IO_PORT_CNT] = { &P1REN, &P2REN };
// Array of pointers to the Port Output registers (P1OUT, P2OUT)
static volatile uint8_t *const port_out_regs[IO_PORT_CNT] = { &P1OUT, &P2OUT };
// Array of pointers to the Port Input registers (P1IN, P2IN)
static volatile uint8_t *const port_in_regs[IO_PORT_CNT] = { &P1IN, &P2IN };
// Array of pointers to the Port Select 1 registers (P1SEL, P2SEL)
static volatile uint8_t *const port_sel1_regs[IO_PORT_CNT] = { &P1SEL, &P2SEL };
// Array of pointers to the Port Select 2 registers (P1SEL2, P2SEL2)
static volatile uint8_t *const port_sel2_regs[IO_PORT_CNT] = { &P1SEL2, &P2SEL2 };

/**
 * @brief Extracts the port number from a generic GPIO enum value.
 * @param gpio The generic pin identifier.
 * @return The port number (0 for Port 1, 1 for Port 2, etc.).
 */
static uint8_t gpio_port(gpio_e gpio)
{
    return (gpio & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

/**
 * @brief Extracts the pin index (0-7) from a generic GPIO enum value.
 * @param gpio The generic pin identifier.
 * @return The 0-based index of the pin.
 */
static inline uint8_t gpio_pin_idx(gpio_e gpio)
{
    return gpio & IO_PIN_MASK;
}

/**
 * @brief Generates a bitmask for a specific pin.
 * @param gpio The generic pin identifier.
 * @return A bitmask with a single bit set for the corresponding pin.
 */
static uint8_t gpio_pin_bit(gpio_e gpio)
{
    return 1 << gpio_pin_idx(gpio);
}

/**
 * @brief Sets the data direction of a specific GPIO pin.
 * @param gpio The generic pin identifier.
 * @param direction The desired direction (input or output).
 */
void gpio_set_direction(gpio_e gpio, gpio_dir_e direction)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(direction)
    {
        case IO_DIR_INPUT:
            // Clear the corresponding bit in the direction register to set as input.
            *port_dir_regs[port] &= ~pin;
            break;
        case IO_DIR_OUTPUT:
            // Set the corresponding bit in the direction register to set as output.
            *port_dir_regs[port] |= pin;
            break;
    }
}

/**
 * @brief Enables or disables the internal pull-up/pull-down resistor.
 * @param gpio The generic pin identifier.
 * @param resistor The desired resistor state (enabled or disabled).
 */
void gpio_set_resistor(gpio_e gpio, gpio_resistor_e resistor)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(resistor)
    {
        case IO_RESISTOR_DISABLED:
            // Clear the corresponding bit in the resistor enable register.
            *port_ren_regs[port] &= ~pin;
            break;
        case IO_RESISTOR_ENABLED:
            // Set the corresponding bit in the resistor enable register.
            *port_ren_regs[port] |= pin;
            break;
    } 
}

/**
 * @brief Sets the function of a specific GPIO pin (GPIO or alternate).
 * * This function modifies the PxSEL and PxSEL2 registers to select the desired function.
 * * @param gpio The generic pin identifier.
 * @param select The desired pin function.
 */
void gpio_set_select(gpio_e gpio, gpio_select_e select)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(select)
    {
        case IO_SELECT_GPIO:
            // SEL = 0, SEL2 = 0
            *port_sel1_regs[port] &= ~pin;
            *port_sel2_regs[port] &= ~pin;
            break;
        case IO_SELECT_ALT1:
            // SEL = 1, SEL2 = 0
            *port_sel1_regs[port] |= pin;
            *port_sel2_regs[port] &= ~pin;
            break;  
        case IO_SELECT_ALT2:
            // SEL = 0, SEL2 = 1
            *port_sel1_regs[port] &= ~pin;
            *port_sel2_regs[port] |= pin;    
            break;
        case IO_SELECT_ALT3:
            // SEL = 1, SEL2 = 1
            *port_sel1_regs[port] |= pin;
            *port_sel2_regs[port] |= pin;
            break;
    }
}

/**
 * @brief Sets the output state (high or low) of a specific GPIO pin.
 * @param gpio The generic pin identifier.
 * @param out The desired output state.
 */
void gpio_set_out(gpio_e gpio, gpio_out_e out)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(out)
    {
        case IO_OUT_LOW:
            // Clear the corresponding bit in the output register.
            *port_out_regs[port] &= ~pin;
            break;
        case IO_OUT_HIGH:
            // Set the corresponding bit in the output register.
            *port_out_regs[port] |= pin;
    }
}

/**
 * @brief Reads the input state of a specific GPIO pin.
 * @param gpio The generic pin identifier.
 * @return The input state (IO_IN_HIGH or IO_IN_LOW).
 */
gpio_in_e gpio_get_input(gpio_e gpio)
{
    // Check if the pin's bit is set in the input register.
    return (*port_in_regs[gpio_port(gpio)] & gpio_pin_bit(gpio)) ? IO_IN_HIGH : IO_IN_LOW;
}

/**
 * @brief Configures a single GPIO pin using a configuration structure.
 * * This function calls the individual setter functions to configure the pin's
 * select, resistor, direction, and output state based on the provided struct.
 * * @param gpio The generic pin identifier.
 * @param config A pointer to the configuration structure.
 */
void gpio_configure(gpio_e gpio, const gpio_config_t *config)
{
    gpio_set_select(gpio, config->select);
    gpio_set_resistor(gpio, config->resistor);
    gpio_set_direction(gpio, config->dir);
    gpio_set_out(gpio, config->out);
}
