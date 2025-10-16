#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

#define LED_ON_PERIOD_MS 800
#define LED_OFF_PERIOD_MS 200

typedef enum {
    LED_ON,
    LED_OFF,
} led_state_e;

typedef enum {
    LED_GREEN,
    LED_RED,
} led_colour_e;

void led_init(void);
void led_set_state(led_colour_e colour, led_state_e state);
void led_blink(led_colour_e colour, uint16_t on_period_ms, uint16_t off_period_ms);
void led_handle_blinking(void);

#endif // LED_H