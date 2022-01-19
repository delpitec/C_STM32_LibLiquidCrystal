# Biblioteca para comunicação de display LCD com STM32

**Descrição da biblioteca:**<br>
A biblioteca está documentada [neste vídeo](https://www.youtube.com/watch?v=KvlmtifSAIs), que possui um código exemplo devidamente documentado no repositório [deste link](https://github.com/delpitec/C_STM32_Exemplo_LibLiquidCrystal/tree/19332d95474b097a10e418d68165d494155e7690). Permite a fácil utilização de displays LCD (*Liquid Crystal Display*)  conectados de modo "paralelo" (isso significa que para cada pino de comunicação do display LCD, deverá ser utilizado um pino do microcontrolador). 
Possui limitação da configuração máxima de 1 display LCD de 20 colunas x 4 linhas - outras quantidades não foram testadas - e caracteres com matriz de 5x8 limitando diversas operações no display(possibilidade de melhoria). 
Existem outras maneiras de realizar a comunicação entre um microcontrolador e displays LCD, que serão abordadas posteriormente no meu [canal do YouTube](https://www.youtube.com/delpitec)
&nbsp;<br>

**Status do desenvolvimento:**<br>
✅ Concluído
&nbsp;<br> 

**Modo de utilização da biblioteca (passo-a-passo):**

1 - Verifique se o seu display possui 20 colunas x 4 linhas e caracteres com matriz de 5x8;&nbsp;<br>
2 - Realize o cabeamento entre o STM32F103C8T6 e os display LCD.;&nbsp;<br>
![Esquemático](https://user-images.githubusercontent.com/58537514/149973458-3530abfb-2d97-4749-9ffe-af8a8a8ae480.png)
&nbsp;<br>
**OBS:** A figura acima, que é a montagem base para este passo-a-passo, respeita a seguinte sequência de ligação:

| From<br>STM32 Pin | To<br>LCD Pin / Function|
| :---:   | :-: |
| A2 | 11 / D4 |
| A3 | 12 / D5 |
| A11 | 13 / D6 |
| A12 | 14 / D7 |
| B0 | 4 / RS |
| B1 | 6 / EN |

&nbsp;<br>

3 - Realize a configuração dos pinos no Device Configuration Tool de acordo com o cabeamento entre o STM32 e display LCD;&nbsp;<br>
&nbsp;<br>
![dct](https://user-images.githubusercontent.com/58537514/149976647-df5111f1-ab85-4a29-a6f5-5312720aec2f.png)


4 - Após criar o projeto, insira os arquivos .h e .c deste repositório nas seguintes pastas do projeto a seguir **(1)** e realize a inserção da seguinte linha de código no local mostrado a seguir **(2)**:
```ruby
#include "lcd_v1.1.h"
```

![includes](https://user-images.githubusercontent.com/58537514/149981020-1d475fbe-a2db-4d58-8c76-e7cd9c8018ed.png)


5 - No cabeçalho do arquivo lcd_v1.1.c, verifique se as macros estão com os mesmos nomes declarados no Device Configuration Tool conforme a seguir (caso não estejam, devem alterar ou no device configuration tool ou no cabeçalho para que fiquem iguais). Lembrando que a macro Output_RS_PIN(state) deve estar associado com o pino do STM32 conectado ao pino de RS do display LCD, a macro Output_EN_PIN(state) deve estar associado com o pino do STM32 conectado ao pino de EN do display LCD, e assim por diante.&nbsp;<br><br>
#define Output_RS_PIN(state) HAL_GPIO_WritePin(**LCD_RS**_GPIO_Port, **LCD_RS**_Pin, state) &nbsp;<br>
#define Output_EN_PIN(state) HAL_GPIO_WritePin(**LCD_EN**_GPIO_Port, **LCD_EN**_Pin, state) &nbsp;<br>
#define Output_D4_PIN(state) HAL_GPIO_WritePin(**LCD_D4**_GPIO_Port, **LCD_D4**_Pin, state) &nbsp;<br>
#define Output_D5_PIN(state) HAL_GPIO_WritePin(**LCD_D5**_GPIO_Port, **LCD_D5**_Pin, state) &nbsp;<br>
#define Output_D6_PIN(state) HAL_GPIO_WritePin(**LCD_D6**_GPIO_Port, **LCD_D6**_Pin, state) &nbsp;<br>
#define Output_D7_PIN(state) HAL_GPIO_WritePin(**LCD_D7**_GPIO_Port, **LCD_D7**_Pin, state) &nbsp;<br>

```ruby
   // Configura os pinos de saída
   #define Output_RS_PIN(state) HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, state) // Mapeia pino RS
   #define Output_EN_PIN(state) HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, state) // Mapeia pino EN
   #define Output_D4_PIN(state) HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, state) // Mapeia pino D4
   #define Output_D5_PIN(state) HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, state) // Mapeia pino D5
   #define Output_D6_PIN(state) HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, state) // Mapeia pino D6
   #define Output_D7_PIN(state) HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, state) // Mapeia pino D7
```

&nbsp;<br>
6 - Usando a biblioteca:
&nbsp;<br>
&nbsp;<br>
A seguir, um exemplo de código que irá escrever uma mensagem de **Hello World** assim que o microcontrolador for inicializado, aguarda 2 segundos e limpa o display:
```ruby
lcd_20x4_4bits_Init();                 // Inicia display 
lcd_posicao_do_cursor(0, 0);           // posição do cursor na tela (linha = 0 , coluna = 0)
lcd_escreve_string("Hello\r");         // escreve uma string Hello
lcd_posicao_do_cursor(1, 0);           // posição do cursor na tela (linha = 1 , coluna = 0)
lcd_escreve_string("world!!!\r");      // escreve uma string world
HAL_Delay(2000);                       // aguarda 2 segundos
lcd_limpa_display();                   // Limpa caracteres do display
```

**OBS:** Para escrever um texto com valores numéricos, a única forma é adaptando a string com valor numérico dentro da string a ser escrita.
A forma mais fácil de se fazer, é através da utilização da função **sprintf()** da biblioteca padrão **stdio.h** (detalhes de seu funcionamento [aqui](https://www.cplusplus.com/reference/cstdio/sprintf/)).
Uma maneira de escrever uma data seria:
```ruby
int dia = 16;
int mes = 9;
int ano = 2020;
char buffer[10];

sprintf(buffer, "%02d/%02d/%04d\r", dia, mes, ano); // armazena dia, mes e ano no formato nn/nn/nnnn

lcd_20x4_4bits_Init();                 // Inicia display 
lcd_posicao_do_cursor(0, 0);           // posição do cursor na tela (linha = 0 , coluna = 0)
lcd_escreve_string("Data:\r");         // escreve uma string "Data"
lcd_posicao_do_cursor(1, 0);           // posição do cursor na tela (linha = 1 , coluna = 0)
lcd_escreve_string(buffer);            // escreve uma string formatada em nn/nn/nnnn
HAL_Delay(2000);                       // aguarda 2 segundos
lcd_limpa_display();                   // Limpa caracteres do display
```
&nbsp;<br><br>

**Possíveis melhorias:**
&nbsp;<br>
🎯 Utilização de displays de tamanhos diferentes a partir de uma função de inicialização;&nbsp;<br>
🎯 Utilização de displays com caracteres de matriz de 5x10;&nbsp;<br>
🎯 Inclusão de funções padrão do display LCD (cursor piscante, movimentação de tela, criação de caracteres personalizados, etc...);&nbsp;<br>

 &nbsp;<br><br> 

## Contact me:
💼[LinkedIn](https://br.linkedin.com/in/rafaeldelpino)&nbsp;&nbsp;&nbsp;
📹[Youtube](https://www.youtube.com/delpitec)&nbsp;&nbsp;&nbsp;
📸[Instagram](https://www.instagram.com/delpitec_/)&nbsp;&nbsp;&nbsp;
📧[E-mail](delpitec@gmail.com)&nbsp;&nbsp;&nbsp;