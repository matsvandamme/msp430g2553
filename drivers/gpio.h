#ifndef GPIO_H
#define GPIO_H

// Define a struct to hold the direction value for the pin
typedef enum
{
    INPUT,
    OUTPUT
} gpio_direction_e;

// Define a struct to hold the state of the pin resistor
typedef enum
{
    RESISTOR_ENABLED,
    RESISTOR_DISABLED
} gpio_resistor_e;

// Define a struct to hold the state of the pullup/pulldown
typedef enum
{
    PULLUP,
    PULLDOWN
} gpio_pullup_pulldown_e;

#endif // GPIO_H
