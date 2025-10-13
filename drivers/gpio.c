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

static uint8_t io_port(io_generic_e io)
{
    return (io & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

static inline uint8_t io_pin_idx(io_generic_e io)
{
    return io & IO_PIN_MASK;
}

static uint8_t io_pin_bit(io_generic_e io)
{
    return 1 << io_pin_idx(io);
}
