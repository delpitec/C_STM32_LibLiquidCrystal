   #include "main.h"
   #include "lcd_v1.1.h"

   // Configura os pinos de saída
   #define Output_RS_PIN(state) HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, state)
   #define Output_EN_PIN(state) HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, state)
   #define Output_D4_PIN(state) HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, state)
   #define Output_D5_PIN(state) HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, state)
   #define Output_D6_PIN(state) HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, state)
   #define Output_D7_PIN(state) HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, state)

   void lcd_escreve_dado(char dado);  		// escrever um caractere individualmente
   void lcd_escreve_instrucao(char instr); 	// escreve um dado de controle (clear display, pos. cursor, etc)

void lcd_escreve_dado(char dado)
{
	Output_RS_PIN(1);
	Output_EN_PIN(0);

	// Posiciona os bits da variável de entrada nos pinos de saída (apenas 8 bits são enviados -> 4 EN 4 EN)
	Output_D7_PIN(dado & 0x80);
	Output_D6_PIN(dado & 0x40);
	Output_D5_PIN(dado & 0x20);
	Output_D4_PIN(dado & 0x10);

	// Pulso Enable
	Output_EN_PIN(1);
    Output_EN_PIN(0);

    Output_D7_PIN(dado & 0x08);
    Output_D6_PIN(dado & 0x04);
    Output_D5_PIN(dado & 0x02);
    Output_D4_PIN(dado & 0x01);

    // Pulso Enable
    Output_EN_PIN(1);
    Output_EN_PIN(0);
    
    //espera_flag
    HAL_Delay(2);

}


void lcd_escreve_instrucao(char instr)
{
	Output_RS_PIN(0u);
	Output_EN_PIN(0u);

	// Posiciona os bits da variável de entrada nos pinos de saída (apenas 8 bits são enviados -> 4 EN 4 EN)
	Output_D7_PIN(instr & 0x80);
	Output_D6_PIN(instr & 0x40);
	Output_D5_PIN(instr & 0x20);
	Output_D4_PIN(instr & 0x10);

	Output_EN_PIN(1);
	Output_EN_PIN(0);

	Output_D7_PIN(instr & 0x08);
	Output_D6_PIN(instr & 0x04);
	Output_D5_PIN(instr & 0x02);
	Output_D4_PIN(instr & 0x01);

	// Pulso Enable
	Output_EN_PIN(1);
	Output_EN_PIN(0);

	//espera flag
	HAL_Delay(2);

}

void lcd_posicao_do_cursor(char lin, char col)
{
	char addr;
	  switch(lin)
	  {
	     case 0: addr = 0x00; break; 	// Primeira Linha
	     case 1: addr = 0x40; break; 	// Segunda Linha
	     case 2: addr = 0x14; break; 	// Terceira Linha
	     case 3: addr = 0x54; break; 	// Quarta linha
	     default: ;
	  }

	  addr +=col;

	  lcd_escreve_instrucao(0x80 | addr);
}

void lcd_20x4_4bits_Init(void)        			 // Comunicação de 4 bits, display de 2 ou mais linhas, mtx 8x5,
{

	HAL_Delay(40);

	lcd_escreve_instrucao_Init(0x30);
	HAL_Delay(5);
	lcd_escreve_instrucao_Init(0x30);
	HAL_Delay(1);
	lcd_escreve_instrucao_Init(0x30);
	HAL_Delay(1);
	lcd_escreve_instrucao_Init(0x20);
	HAL_Delay(1);

	lcd_escreve_instrucao(0x28);                // 4 bists, display de 2 ou mais linhas e mtx 8x5
	lcd_escreve_instrucao(0x08);                // Desliga o display
	lcd_escreve_instrucao(0x0E);                // Liga o display e cursor
	lcd_escreve_instrucao(0x06);                // Desloca cursor para direita qunado caract. � inserido

	//lcd_escreve_instrucao(0x0C);              // Desliga o cursor, caso deseja deixar ligado, comentar essa linha

	lcd_escreve_instrucao(0x01);                // Limpa a tela do display e desloca para posi��o 1x1
 
}    

void lcd_escreve_instrucao_Init(char opcode){

	// Posiciona os bits da varíavel de entrada nos pinos de saída (apenas 4 bits são enviados)
	Output_D7_PIN(opcode & 0x80);
	Output_D6_PIN(opcode & 0x40);
	Output_D5_PIN(opcode & 0x20);
	Output_D4_PIN(opcode & 0x10);

	// Pulso no enable
	Output_EN_PIN(1);
	Output_EN_PIN(0);

}
    
void lcd_limpa_display(void)
{
    lcd_escreve_instrucao(0x01);            // Limpa a tela do display e desloca para posição 0x0
}

void lcd_escreve_string(char* c){
	while (*c != '\r'){
		lcd_escreve_dado((char)*c);
		c++;
	}   
}
