#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"

#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"

#include "display/ssd1306.c"
#include "pwm/pwm.h"
#include "buttons/buttons.h"
#include "led_rgb/led_rgb.h"
#include "joystick/joystick.h"

#define resolution_ADC 4096
#define WIDTH_RECT 8
#define HEIGHT_RECT 8

#define PWM_BLUE_LED 12
#define PWM_RED_LED 13

#define FREQUENCY 100
#define F_CLOCK 125000000
#define WRAP 65358
#define D_I 19
#define D_F 2

static volatile bool allow_pwm_changes = true;
static volatile bool green_led_state = false;
static volatile bool cor = true;
// Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time = 0;

void gpio_irq_handler(uint gpio, uint32_t events) {
  // Obtém o tempo atual em microssegundos
  uint32_t current_time = to_us_since_boot(get_absolute_time());

  // Verifica se passou tempo suficiente desde o último evento
  // 200 ms de debouncing
  if (current_time - last_time > 200000) {
    // Atualiza o tempo do último evento
    last_time = current_time;

    if(gpio == BUTTON_A) {
      printf("Botão A pressionado\n");
      set_blue(false);
      set_red(false);
      allow_pwm_changes = !allow_pwm_changes;
    } else if (gpio == BUTTON_B) {
      printf("Botão B pressionado\n");
      reset_usb_boot(0, 0);
    } else if (gpio == JOYSTICK_BUTTON) {
      printf("Botão Joystick pressionado\n");
      green_led_state = !green_led_state;
      set_green(green_led_state);
    }
  }   
}

int main() {

  // Buffer para armazenar a string
  char str_x[5];  
  // Buffer para armazenar a string 
  char str_y[5];  

  uint16_t adc_value_x;
  uint16_t adc_value_y;
  uint16_t position_rectangle_x = WIDTH / 2 - WIDTH_RECT / 2;
  uint16_t position_rectangle_y = HEIGHT / 2 - HEIGHT_RECT / 2;

  stdio_init_all();

  setup_buttons();
  setup_joystick();
  setup_led_RGB();
  setup_display_oled();

  setup_pwm(PWM_BLUE_LED, WRAP, F_CLOCK, D_I, D_F);
  setup_pwm(PWM_RED_LED, WRAP, F_CLOCK, D_I, D_F);

  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  
  while (true) {
    // Seleciona o ADC para eixo X
    adc_value_x = read_X();
    // Seleciona o ADC para eixo Y
    adc_value_y = read_Y();

    // Inverte os eixos
    uint16_t aux = adc_value_x;
    adc_value_x = adc_value_y;
    adc_value_y = aux;

    // Converte os inteiros em string
    sprintf(str_x, "%d", adc_value_x);
    sprintf(str_y, "%d", adc_value_y);

    // Calcula o valor do incrementos x e y para a posição do retângulo
    int increment_x = adc_value_x - (JOYSTICK_MIDDLE_X);
    increment_x = increment_x / (HEIGHT / 2.0);
    int increment_y = adc_value_y - (JOYSTICK_MIDDLE_Y);
    increment_y = increment_y / (WIDTH / 2.0);

    if(increment_x >  position_rectangle_x) {
      increment_x = position_rectangle_x;
    } else if(increment_x < - (position_rectangle_x)) {
      increment_x = - (position_rectangle_x);
    }
    if(increment_y > position_rectangle_y) {
      increment_y = position_rectangle_y;
    } else if(increment_y < - (position_rectangle_y)) {
      increment_y = - (position_rectangle_y);
    }

    if(allow_pwm_changes) {
      // Calcula o valor do duty cycle para os leds
      float percentage_x;
      if(increment_x < 0) {
        percentage_x = - (increment_x * 1.0) / (position_rectangle_x);
      } else {
        percentage_x = + (increment_x * 1.0) / (position_rectangle_x);
      }
      float percentage_y;
      if(increment_y < 0) {
        percentage_y = - (increment_y * 1.0) / (position_rectangle_y);
      } else {
        percentage_y = + (increment_y * 1.0) / (position_rectangle_y);
      }

      //definir o ciclo de trabalho (duty cycle) do pwm
      pwm_set_gpio_level(PWM_RED_LED, WRAP * percentage_x);
      pwm_set_gpio_level(PWM_BLUE_LED, WRAP * percentage_y);
    }
    
    // Atualiza o conteúdo do display
    display_fill(false);
    if(green_led_state) {
      // Desenha um retângulo a mais na borda
      display_draw_rectangle(1, 1, 126, 62, cor, !cor);
      display_draw_rectangle(2, 2, 124, 60, cor, !cor);
    }
    display_draw_rectangle(0, 0, 128, 64, cor, !cor);
    display_draw_rectangle(
      position_rectangle_y - increment_y, 
      position_rectangle_x + increment_x, 
      WIDTH_RECT, HEIGHT_RECT, cor, cor
    );
    display_send_data();
    sleep_ms(20);
  }
}