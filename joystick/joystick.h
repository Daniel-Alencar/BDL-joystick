// GPIO para eixo X
#define JOYSTICK_X_PIN 26  
// GPIO para eixo Y
#define JOYSTICK_Y_PIN 27  
// GPIO para botão do Joystick
#define JOYSTICK_BUTTON 22

void setup_joystick() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);  

    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON); 
}