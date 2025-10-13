#include "gpio.h"
#include "defines.h"

uint8_t gpio_set_direction(gpio_pin_e pin, gpio_direction_e direction);
uint8_t gpio_set_resistor(gpio_pin_e pin, gpio_resistor_e resistor_state);
uint8_t gpio_set_function(gpio_pin_e pin, gpio_function_e function);