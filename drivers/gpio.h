#ifndef GPIO_H
#define GPIO_H

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

// Define an enum to hold the pin numbers
typedef enum
{
    GPIO0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
    GPIO9,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    GPIO18,
    GPIO19
} gpio_pins_e;

#endif // GPIO_H
