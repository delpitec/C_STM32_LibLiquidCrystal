#include <stdint.h>
#include <string.h>

#ifndef __LIQUIDCRISTAL_H
#define __LIQUIDCRISTAL_H

#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

struct LCD{
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _numlines;
    uint8_t _row_offsets[4];
};
typedef struct LCD LiquidCrystal;

// Inicia objeto referente ao LCD
void begin(LiquidCrystal *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);

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
void command(LiquidCrystal *lcd, uint8_t value);

#endif
