#ifndef GPIO_H
#define GPIO_H

// Define a struct to hold the direction value for the pin
typedef struct
{
    INPUT,
    OUTPUT
} gpio_direction_t;

// Define a struct to hold the state of the pin resistor
typedef struct
{
    RESISTOR_ENABLED,
    RESISTOR_DISABLED
} gpio_resistor_t;

// Define a struct to hold the state of the pullup/pulldown
typedef struct gpio
{
    PULLUP,
    PULLDOWN
} gpio_pullup_pulldown_t;

#endif // GPIO_H
