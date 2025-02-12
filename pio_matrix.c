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

#define SPEED 1
#define resolution_ADC 4096
#define WIDTH_RECT 8
#define HEIGHT_RECT 8

#define Botao_A 5 
#define Botao_B 6

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
  if (current_time - last_time > 100000) {
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

  //habilitar o pino GPIO como PWM
  gpio_set_function(PWM_BLUE_LED, GPIO_FUNC_PWM);
  //obter o canal PWM da GPIO
  uint slice = pwm_gpio_to_slice_num(PWM_BLUE_LED); 
  //definir o valor de wrap
  pwm_set_wrap(slice, WRAP);
  // Realiza o cálculo de frequência
  compute_pwm_frequency(F_CLOCK, WRAP, D_I, D_F);
  //define o divisor de clock do PWM
  pwm_set_clkdiv(slice, D_I + (D_F) / 10.0);
  //habilita o pwm no slice correspondente
  pwm_set_enabled(slice, true);

  //habilitar o pino GPIO como PWM
  gpio_set_function(PWM_RED_LED, GPIO_FUNC_PWM);
  //obter o canal PWM da GPIO
  slice = pwm_gpio_to_slice_num(PWM_RED_LED); 
  //definir o valor de wrap
  pwm_set_wrap(slice, WRAP);
  // Realiza o cálculo de frequência
  compute_pwm_frequency(F_CLOCK, WRAP, D_I, D_F);
  //define o divisor de clock do PWM
  pwm_set_clkdiv(slice, D_I + (D_F) / 10.0);
  //habilita o pwm no slice correspondente
  pwm_set_enabled(slice, true);


  gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
  
  while (true) {
    // Seleciona o ADC para eixo X. O pino 26 como entrada analógica
    adc_select_input(0); 
    adc_value_x = adc_read();
    // Seleciona o ADC para eixo Y. O pino 27 como entrada analógica
    adc_select_input(1); 
    adc_value_y = adc_read();

    uint16_t aux = adc_value_x;
    adc_value_x = adc_value_y;
    adc_value_y = aux;

    sprintf(str_x, "%d", adc_value_x);  // Converte o inteiro em string
    sprintf(str_y, "%d", adc_value_y);  // Converte o inteiro em string

    int increment_x = adc_value_x - (2118);
    increment_x = increment_x / 32.0;
    int increment_y = adc_value_y - (1997);
    increment_y = increment_y / 64.0;

    if(increment_x >  WIDTH / 2 - WIDTH_RECT / 2) {
      increment_x = WIDTH / 2 - WIDTH_RECT / 2;
    } else if(increment_x < - (WIDTH / 2 - WIDTH_RECT / 2)) {
      increment_x = - (WIDTH / 2 - WIDTH_RECT / 2);
    }
    if(increment_y > HEIGHT / 2 - HEIGHT_RECT / 2) {
      increment_y = HEIGHT / 2 - HEIGHT_RECT / 2;
    } else if(increment_y < - (HEIGHT / 2 - HEIGHT_RECT / 2)) {
      increment_y = - (HEIGHT / 2 - HEIGHT_RECT / 2);
    }

    // printf("X: %s | Y: %s | IncrementoX: %d | IncrementoY: %d\n", str_x, str_y, increment_x, increment_y);

    if(allow_pwm_changes) {
      float percentage_x;
      if(increment_x < 0) {
        percentage_x = - (increment_x * 1.0) / (WIDTH / 2 - WIDTH_RECT / 2);
      } else {
        percentage_x = + (increment_x * 1.0) / (WIDTH / 2 - WIDTH_RECT / 2);
      }
      float percentage_y;
      if(increment_y < 0) {
        percentage_y = - (increment_y * 1.0) / (HEIGHT / 2 - HEIGHT_RECT / 2);
      } else {
        percentage_y = + (increment_y * 1.0) / (HEIGHT / 2 - HEIGHT_RECT / 2);
      }

      // printf("percentage_x: %.2f | percentage_y: %.2f\n", percentage_x, percentage_y);
      
      //definir o ciclo de trabalho (duty cycle) do pwm
      pwm_set_gpio_level(PWM_RED_LED, WRAP * percentage_x);
      pwm_set_gpio_level(PWM_BLUE_LED, WRAP * percentage_y);
    }
    
    // Atualiza o conteúdo do display
    display_fill(false);
    if(green_led_state) {
      // Desenha um retângulo a mais
      if(true) {
        display_draw_rectangle(3, 3, 123, 59, cor, !cor);
      }
    }
    display_draw_rectangle(1, 1, 127, 63, cor, !cor);
    display_draw_rectangle(
      position_rectangle_y - increment_y, 
      position_rectangle_x + increment_x, 
      WIDTH_RECT, HEIGHT_RECT, cor, cor
    );
    display_send_data();
    sleep_ms(20);
  }
}