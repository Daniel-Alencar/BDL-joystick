# Dispositivo display ssd1306

## Descrição
Este projeto tem como objetivo interagir com o periférico ADC da BitDogLab através do joystick presente na mesma.

## Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/Daniel-Alencar/BDL-joystick
   ```
2. Navegue até o diretório do projeto:
   ```bash
   cd BDL-joystick
   ```
3. Abra o projeto no VS Code.
  
4. Compile e execute o projeto usando a placa BitDogLab.

5. Ao executar o projeto, perceba que um pequeno quadrado é exibido no display ssd1306. A medida que o joystick se move, o quadrado se move na tela nos eixos x e y.

6. A medida que o joystick é movido perceba a intensidade da luminosidade dos leds AZUL e VERMELHO. Ao mover o joystick no eixo X, a intensidade do led vermelho aumenta. Ao mover o joystick no eixo Y, a intensidade do led azul aumenta. Você pode desligar este comportamento ao clicar no botão A.

7. O botão do joystick realiza duas funções liga/desliga o led VERDE da placa e adiciona uma pequena borda a mais no display ssd1306.

## Vídeo de Demonstração
```bash
   Link: 
```

## Adendos

Foram utilizadas as funções auxiliares compute_pwm_parameters() e compute_pwm_frequency() para o cálculo exato dos parâmetros de dividor inteiro, divisor fracionário e wrap para uma frequência desejada do PWM.

Esssas funções estão definidas no arquivo pwm/pwm.h.

## Licença
Este projeto é licenciado sob a licença MIT – consulte o arquivo LICENSE para mais detalhes.

