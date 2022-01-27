# Biblioteca para comunicação de display LCD com STM32

**Descrição da biblioteca:**<br>
A biblioteca permite a fácil utilização de displays LCD (*Liquid Crystal Display*) conectados de modo "paralelo" (isso significa que para cada pino de comunicação do display LCD, deverá ser utilizado um pino do microcontrolador).  
Existem outras maneiras de realizar a comunicação entre um microcontrolador e displays LCD, que serão abordadas posteriormente no meu [canal do YouTube](https://www.youtube.com/delpitec)
&nbsp;<br>

**Status do desenvolvimento:**<br>
✅ Concluído
&nbsp;<br> &nbsp;<br>

**Modo de utilização da biblioteca (passo-a-passo):**

1 - Realize o cabeamento entre o STM32F103C8T6 e os display LCD, podendo variar sua configuração (todas aceitas pela biblioteca) de acordo com a tabela a seguir.;&nbsp;<br>
| Tipo de<br>comunicação | Qtd de <br>Pinos | Pinos utilizados |
| :---:   | :-: |  :-: |
| 4bit data/2bit control | 6 | D7, D6, D5, D4, RS, EN |
| 4bit data/3bit control | 7 | D7, D6, D5, D4, RS, EN, RW |
| 8bit data/2bit control | 10 | D7, D6, D5, D4, D3, 2, D1, D0, RS, EN |
| 8bit data/3bit control | 11 | D7, D6, D5, D4, D3, 2, D1, D0, RS, EN, RW |

Na foto a seguir, é possível perceber que foi escolhida a opção *8bit data/3bit control*, pois deste modo é possível realizar os testes de todas as possibilidades facilmente.

![stm32 and display lcd _ stm32 and liquid crystal display](https://user-images.githubusercontent.com/58537514/151100045-d9c0c72d-9a7d-416b-83bf-2329d887d6be.png)
&nbsp;<br>
**OBS:** A figura acima, que é a montagem base para este passo-a-passo, respeita a seguinte sequência de ligação:

| From<br>STM32 Pin | To<br>LCD Pin / Function|
| :---:   | :-: |
| B3 | 4 / RS |
| A15 | 5 / RW |
| A12 | 6 / EN |
| A11 | 7 / D0 |
| A10 | 8 / D1 |
| A9 | 9 / D2 |
| A8 | 10 / D3 |
| B15 | 11 / D4 |
| B14 | 12 / D5 |
| B13 | 13 / D6 |
| B12 | 14 / D7 |


&nbsp;<br>

2 - Realize a configuração dos pinos no *Device Configuration Tool* de acordo com o cabeamento entre o STM32 e display LCD;&nbsp;<br>
**OBS:** A biblioteca identifica automaticamente qual a configuração de comunicaçãp que está sendo utilizada. Para isso, deve-se utilizar as seguintes nomenclaturas para os pinos do display:
*LCD_EN, LCD_RS, LCD_RW, LCD_D0, LCD_D1, LCD_D2, LCD_D3, LCD_D4, LCD_D5, LCD_D6 e LCD_D7*.
&nbsp;<br>

![stm32cubeide liquid crystal](https://user-images.githubusercontent.com/58537514/151214726-1a19aad1-7bc0-4a4f-85cf-033cd33a1fa6.png)
&nbsp;
**OBS:** Quando o pino RW do display LCD não for utilizado, deve ser aterrado.

3 - Após criar o projeto, insira os arquivos .h e .c deste repositório nas seguintes pastas do projeto a seguir **(1)** e realize a inserção da seguinte linha de código no local mostrado a seguir **(2)**:
```ruby
#include "LiquidCrystal.h"
```

![stm32cubeide display lcd stm32 lcd](https://user-images.githubusercontent.com/58537514/151248728-3c34bd36-ae5e-4fb5-9bd0-f98afc575d28.png)


4 - Usando a biblioteca:&nbsp;<br>
Primeiramente, deve-se iniciar um objeto contendo a quantidade de linhas, colunas e o tamanho do caractere do display através da função a seguir:


```ruby
// Inicia objeto referente ao LCD
void begin(LiquidCrystal *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);
```

Exemplo de inicialização:
```ruby
LiquidCrystal lcd;
begin(&lcd,16,2,LCD_5x8DOTS);
```
&nbsp;<br>

A seguir, temos as funções com comentários resumidos sobre seu funcionamento:
```ruby
// Escreve uma string no display apontado por *lcd
void print(LiquidCrystal *lcd, int8_t *value);
	
// Apaga caracteres do display apontado por *lcd e posiciona cursor na primeira linha e primeira coluna
void clear(LiquidCrystal *lcd);
	
// Posiciona o cursor do display apontado por *lcd na primeira linha e primeira coluna
void home(LiquidCrystal *lcd);

// Desabilita a exibição dos caracteres no display sem perder o texto de sua memória
void noDisplay(LiquidCrystal *lcd);

// Habilita a exibição dos caracteres no display e exibe o que estiver armazenado em sua memória.
void display(LiquidCrystal *lcd);
	
// Desabilita cursor piscante apontado por *lcd
void noBlink(LiquidCrystal *lcd);
	
// Habilita cursor piscante apontado por *lcd
void blink(LiquidCrystal *lcd);
	
// Desabilita cursor underscore (_) apontado por *lcd
void noCursor(LiquidCrystal *lcd);
	
// Habilita cursor underscore (_) apontado por *lcd
void cursor(LiquidCrystal *lcd);

// Movimenta todo o conteúdo no display apontado por *lcd uma posição para esquerda
void scrollDisplayLeft(LiquidCrystal *lcd);

// Movimenta todo o conteúdo no display apontado por *lcd uma posição para direita
void scrollDisplayRight(LiquidCrystal *lcd);

// Especifica a direção do texto escrito  no display apontado por *lcd da esquerda para a direita.
void leftToRight(LiquidCrystal *lcd);

// Especifica a direção do texto escrito  no display apontado por *lcd da direita para a esquerda.
void rightToLeft(LiquidCrystal *lcd);

// Para cada novo caractere inserido no display apontado por *lcd, o texto anterior move-se automaticamente para o lado
void autoscroll(LiquidCrystal *lcd);

// Para cada novo caractere inserido no display apontado por *lcd, o texto anterior não move-se automaticamente para o lado
void noAutoscroll(LiquidCrystal *lcd);

// Cria caractere personalizado na memória interna do display apontado por *lcd
void createChar(LiquidCrystal *lcd, uint8_t location, uint8_t charmap[]);

// Posiciona o cursor do display apontado por *lcd na posição indicada por col e row (primeira linha e primeira coluna: 0,0)
void setCursor(LiquidCrystal *lcd, uint8_t col, uint8_t row);

// Escreve caractere no display apontado por *lcd
void write(LiquidCrystal *lcd, uint8_t value);

// Escreve comando no display apontado por *lcd (verificar comandos disponíveis no datasheet do display LCD)
void command(LiquidCrystal *lcd, uint8_t value);```
```
&nbsp;<br><br>

**Possíveis melhorias:**
&nbsp;<br>
🎯 Possibilidade de utilizar mais de um display sendo reconhecido de forma automática após configuração no *Device Configuration Tool*
 &nbsp;<br><br> 

## Contact me:
💼[LinkedIn](https://br.linkedin.com/in/rafaeldelpino)&nbsp;&nbsp;&nbsp;
📹[Youtube](https://www.youtube.com/delpitec)&nbsp;&nbsp;&nbsp;
📸[Instagram](https://www.instagram.com/delpitec_/)&nbsp;&nbsp;&nbsp;
📧[E-mail](delpitec@gmail.com)&nbsp;&nbsp;&nbsp;