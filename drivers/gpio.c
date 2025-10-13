#include "gpio.h"
#include "defines.h"

#define IO_PORT_CNT (2u)
#define IO_PORT_OFFSET (3u)
#define IO_PORT_MASK (0x3u << IO_PORT_OFFSET)
#define IO_PIN_MASK (0x7u)

static volatile uint8_t *const port_dir_regs[IO_PORT_CNT] = { &P1DIR, &P2DIR };
static volatile uint8_t *const port_ren_regs[IO_PORT_CNT] = { &P1REN, &P2REN };
static volatile uint8_t *const port_out_regs[IO_PORT_CNT] = { &P1OUT, &P2OUT };
static volatile uint8_t *const port_in_regs[IO_PORT_CNT] = { &P1IN, &P2IN };
static volatile uint8_t *const port_sel1_regs[IO_PORT_CNT] = { &P1SEL, &P2SEL };
static volatile uint8_t *const port_sel2_regs[IO_PORT_CNT] = { &P1SEL2, &P2SEL2 };

static uint8_t gpio_port(gpio_generic_e gpio)
{
    return (gpio & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

static inline uint8_t gpio_pin_idx(gpio_generic_e gpio)
{
    return gpio & IO_PIN_MASK;
}

static uint8_t gpio_pin_bit(gpio_generic_e gpio)
{
    return 1 << gpio_pin_idx(gpio);
}

void gpio_set_direction(gpio_generic_e gpio, gpio_dir_e direction)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(direction)
    {
        case IO_DIR_INPUT:
            *port_dir_regs[port] &= ~pin;
            break;
        case IO_DIR_OUTPUT:
            *port_dir_regs[port] |= pin;
            break;
    }
}

void io_set_resistor(gpio_generic_e gpio, gpio_resistor_e resistor)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(resistor)
    {
        case IO_RESISTOR_DISABLED:
            *port_ren_regs[port] &= ~pin;
            break;
        case IO_RESISTOR_ENABLED:
            *port_ren_regs[port] |= pin;
            break;
    } 
}

void io_set_select(gpio_generic_e gpio, gpio_select_e select)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(select)
    {
        case IO_SELECT_GPIO:
            *port_sel1_regs[port] &= ~pin;
            *port_sel2_regs[port] &= ~pin;
            break;
        case IO_SELECT_ALT1:
            *port_sel1_regs[port] |= pin;
            *port_sel2_regs[port] &= ~pin;
            break;  
        case IO_SELECT_ALT2:
            *port_sel1_regs[port] &= ~pin;
            *port_sel2_regs[port] |= pin;    
            break;
        case IO_SELECT_ALT3:
            *port_sel1_regs[port] |= pin;
            *port_sel2_regs[port] |= pin;
            break;
    }
}

void io_set_out(gpio_generic_e gpio, gpio_out_e out)
{
    uint8_t port = gpio_port(gpio);
    uint8_t pin = gpio_pin_bit(gpio);

    switch(out)
    {
        case IO_OUT_LOW:
            *port_out_regs[port] &= ~pin;
            break;
        case IO_OUT_HIGH:
            *port_out_regs[port] |= pin;
    }
}

gpio_in_e io_get_input(gpio_generic_e gpio)
{
    return (*port_in_regs[gpio_port(gpio)] & gpio_pin_bit(gpio)) ? IO_IN_HIGH : IO_IN_LOW;
}