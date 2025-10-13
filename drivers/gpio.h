#ifndef GPIO_H
#define GPIO_H

#include <msp430.h>
#include <stdint.h>

typedef enum {
    IO_10, IO_11, IO_12, IO_13, IO_14, IO_15, IO_16, IO_17,
    IO_20, IO_21, IO_22, IO_23, IO_24, IO_25, IO_26, IO_27,
} gpio_generic_e;

typedef enum
{
    IO_SELECT_GPIO,
    IO_SELECT_ALT1,
    IO_SELECT_ALT2,
    IO_SELECT_ALT3,
} gpio_select_e;

typedef enum
{
    IO_DIR_INPUT,
    IO_DIR_OUTPUT,
} gpio_dir_e;

typedef enum
{
    IO_RESISTOR_DISABLED,
    IO_RESISTOR_ENABLED,
} gpio_resistor_e;

typedef enum
{
    IO_OUT_LOW, // (pull-down)
    IO_OUT_HIGH, // (pull-up)
} gpio_out_e;

typedef enum
{
    IO_IN_LOW,
    IO_IN_HIGH,
} gpio_in_e;

typedef enum
{
    IO_TRIGGER_RISING,
    IO_TRIGGER_FALLING
} gpio_trigger_e;

struct io_config
{
    gpio_select_e select;
    gpio_resistor_e resistor;
    gpio_dir_e dir;
    gpio_out_e out;
};

void io_init(void);
void io_configure(gpio_generic_e gpio, const struct io_config *config);
void io_set_select(gpio_generic_e gpio, gpio_select_e select);
void io_set_direction(gpio_generic_e gpio, gpio_dir_e direction);
void io_set_resistor(gpio_generic_e gpio, gpio_resistor_e resistor);
void io_set_out(gpio_generic_e gpio, gpio_out_e out);
gpio_in_e io_get_input(gpio_generic_e gpio);

#endif // GPIO_H
