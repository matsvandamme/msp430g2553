#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#define LED_ON_PERIOD_MS 800
#define LED_OFF_PERIOD_MS 200

typedef enum {
    LED_ON,
    LED_OFF,
} led_state_e;

typedef struct
{
    gpio_e io;      // GPIO pin associated with the LED
    led_state_e state;   // Current state of the LED (ON or OFF)
    uint16_t on_period_ms;  // Duration for which the LED stays ON during blinking
    uint16_t off_period_ms; // Duration for which the LED stays OFF during blinking
    bool is_blinking; // Flag indicating if the LED is currently blinking
    uint32_t last_toggle_time; // Last time the LED was toggled
} led_t;

extern volatile led_t leds[];

void led_init(void);
void led_set_state(led_t * led, led_state_e state);
void led_start_blinking(led_t * led, uint16_t on_period_ms, uint16_t off_period_ms);
void led_stop_blinking(led_t * led);
void led_handle_blinking(void);

#endif // LED_H