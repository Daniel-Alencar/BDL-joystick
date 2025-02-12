# Dispositivo display ssd1306

## Descrição
Este projeto tem como objetivo interagir com o periférico display ssd1306 da BitDogLab por meio de comunicação serial via USB do computador.

## Como Executar
1. Clone o repositório:
   ```bash
   git clone https://github.com/Daniel-Alencar/BDL-interface-I2C
   ```
2. Navegue até o diretório do projeto:
   ```bash
   cd BDL-interface-I2C
   ```
3. Abra o projeto no VS Code.
  
4. Compile e execute o projeto usando a placa BitDogLab.

5. Abra o serial monitor do VS Code na porta COM da placa BitDogLab.

6. Envie uma string e envie também o caractere '.' para indicar o final da string. Com isso, a string enviada deve ser processada e exibida no display ssd1306 da BitDogLab.

7. Perceba que no display temos a indicação do estado dos leds verde e azul. Inicialmente, eles começam desligados (off). Para mudar o estado do led verde, clique no botão A. Para mudar o estado do led azul, clique no botão B. Perceba que ao mudar o estado dos leds, o estado atual também é exibido no monitor serial do VS Code.

## Vídeo de Demonstração
```bash
   Link: https://youtube.com/shorts/ONkFn7slkMI
```

## Adendos

Usei o programa display/make_font.c para auxiliar na implementação das letras minúsculas em font.h.

Inicialmente para desenhar as letras minúsculas, me utilizei do site [piskel](https://www.piskelapp.com/p/create/sprite). Onde pude ter mais liberdade de como exatamente queria as letras minúsculas do meu programa. 

Depois, defini todas essas letras no arquivo display/make_font.c, onde cada letra é representada por uma matriz de valores inteiros 0 e 1. Esse programa me ajudou a converter de forma adequada todos os valores em hexadecimal, para complementar os caracteres disponíveis para uso no display ssd1306.

## Licença
Este projeto é licenciado sob a licença MIT – consulte o arquivo LICENSE para mais detalhes.

