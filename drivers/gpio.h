#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// Define an enum to hold the direction value for the pin
typedef enum
{
    INPUT,
    OUTPUT
} gpio_direction_e;

// Define an enum to hold the state of the pin resistor
typedef enum
{
    RESISTOR_ENABLED,
    RESISTOR_DISABLED
} gpio_resistor_e;

// Define an enum to hold the state of the pullup/pulldown
typedef enum
{
    PULLUP,
    PULLDOWN
} gpio_pullup_pulldown_e;

typedef enum 
{
    IOFUNCTION,
    PRIMARY_PERIPHERAL,
    SECONDARY_PERIPHERAL
} gpio_function_e;

// Define an enum to hold the GPIO pin numbers (msp430g2553 has a total of 16 gpio pins in PDIP package)
typedef enum
{
    GPIO0 = 0x00, // 0 in decimal
    GPIO1 = 0x01, // 1 in decimal
    GPIO2 = 0x02, // 2 in decimal
    GPIO3 = 0x03, // 3 in decimal
    GPIO4 = 0x04, // 4 in decimal
    GPIO5 = 0x05, // 5 in decimal
    GPIO6 = 0x06, // 6 in decimal
    GPIO7 = 0x07, // 7 in decimal
    GPIO8 = 0x08, // 8 in decimal
    GPIO9 = 0x09, // 9 in decimal
    GPIO10 = 0x0A, // 10 in decimal
    GPIO11 = 0x0B, // 11 in decimal
    GPIO12 = 0x0C, // 12 in decimal
    GPIO13 = 0x0D, // 13 in decimal
    GPIO14 = 0x0E, // 14 in decimal
    GPIO15 = 0x0F, // 15 in decimal
} gpio_pin_e;

typedef struct gpio
{
    gpio_direction_e direction;
    gpio_function_e function;
    gpio_pullup_pulldown_e pull;
    gpio_resistor_e resistor;
} gpio_configuration_t;

uint8_t gpio_init(void);
uint8_t gpio_configure(gpio_pin_e pin, gpio_configuration_t config);

#endif // GPIO_H
