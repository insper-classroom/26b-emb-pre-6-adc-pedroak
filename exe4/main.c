#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int PIN_LED_B = 4;
const float conversion_factor = 3.3f / (1 << 12);

#define PERIODO_ZONA1 300
#define PERIODO_ZONA2 500

volatile int flag_led = 0;
volatile int flag_adc = 0;

bool led_timer_callback(repeating_timer_t *rt) {
    flag_led = 1;
    return true;  // continua repetindo
}

bool adc_timer_callback(repeating_timer_t *rt) {
    flag_adc = 1;
    return true;
}

int main() {
    stdio_init_all();

    gpio_init(PIN_LED_B);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);
    gpio_put(PIN_LED_B, 0);

    adc_init();
    adc_gpio_init(27);
    adc_select_input(1);

    // timer que dispara a leitura do ADC
    repeating_timer_t timer_adc;
    add_repeating_timer_ms(100, adc_timer_callback, NULL, &timer_adc);

    repeating_timer_t timer_led;
    int timer_led_ativo = 0;
    int periodo_atual = 0;
    int led_state = 0;

    while (true) {
        if (flag_adc) {
            flag_adc = 0;

            float voltage = adc_read() * conversion_factor;

            int periodo;
            if (voltage < 1.0f) {
                periodo = 0;
            } else if (voltage < 2.0f) {
                periodo = PERIODO_ZONA1;
            } else {
                periodo = PERIODO_ZONA2;
            }

            // só mexe no timer se a zona mudou
            if (periodo != periodo_atual) {
                periodo_atual = periodo;

                if (timer_led_ativo) {
                    cancel_repeating_timer(&timer_led);
                    timer_led_ativo = 0;
                }
                flag_led = 0;

                if (periodo == 0) {
                    led_state = 0;
                    gpio_put(PIN_LED_B, 0);
                } else {
                    add_repeating_timer_ms(periodo, led_timer_callback, NULL, &timer_led);
                    timer_led_ativo = 1;
                }
            }
        }

        if (flag_led) {
            flag_led = 0;
            if (periodo_atual != 0) {
                led_state = !led_state;
                gpio_put(PIN_LED_B, led_state);
            }
        }
    }
}