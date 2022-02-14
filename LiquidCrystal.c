#include "LiquidCrystal.h"
#include "main.h"

	// commandos
	#define LCD_CLEARDISPLAY 0x01
	#define LCD_RETURNHOME 0x02
	#define LCD_ENTRYMODESET 0x04
	#define LCD_DISPLAYCONTROL 0x08
	#define LCD_CURSORSHIFT 0x10
	#define LCD_FUNCTIONSET 0x20
	#define LCD_SETCGRAMADDR 0x40
	#define LCD_SETDDRAMADDR 0x80

	// flags para display entry mode
	#define LCD_ENTRYRIGHT 0x00
	#define LCD_ENTRYLEFT 0x02
	#define LCD_ENTRYSHIFTINCREMENT 0x01
	#define LCD_ENTRYSHIFTDECREMENT 0x00

	// flags para controle display on/off
	#define LCD_DISPLAYON 0x04
	#define LCD_DISPLAYOFF 0x00
	#define LCD_CURSORON 0x02
	#define LCD_CURSOROFF 0x00
	#define LCD_BLINKON 0x01
	#define LCD_BLINKOFF 0x00

	// flags para display/cursor shift
	#define LCD_DISPLAYMOVE 0x08
	#define LCD_CURSORMOVE 0x00
	#define LCD_MOVERIGHT 0x04
	#define LCD_MOVELEFT 0x00

	// flags for function set
	#if defined(LCD_D0_GPIO_Port) && defined(LCD_D1_GPIO_Port) && defined(LCD_D2_GPIO_Port) && defined(LCD_D3_GPIO_Port) \
		&& defined(LCD_D0_Pin) && defined(LCD_D1_Pin) && defined(LCD_D2_Pin) && defined(LCD_D3_Pin)
	#define LCD_8BITMODE 0x10
	#endif
	#define LCD_4BITMODE 0x00
	#define LCD_2LINE 0x08
	#define LCD_1LINE 0x00

	// Private functions
	void send(LiquidCrystal *lcd, uint8_t value, uint8_t mode);
	void write4bits(uint8_t value);
	void pulseEnable(void);
	void setRowOffsets(LiquidCrystal *lcd, int row1, int row2, int row3, int row4);

	#if defined(LCD_8BITMODE)
	void write8bits(uint8_t value);
	#endif


/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Inicializa a interface com a tela LCD e especifica as dimensões (largura e altura) da tela, e o tamanho da fonte. 				  */
/*              begin() precisa ser chamado antes de qualquer outro comando da biblioteca LCD.                                                    */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Número de colunas do display                                                                                                      */
/* @param3    : Número de linhas do display 																									  */
/* @param4    : Tamanho da matriz de caractere do display (LCD_5x10DOTS ou LCD_5x8DOTS) 														  */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void begin(LiquidCrystal *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize) {

	LiquidCrystal zero = {0};
	*lcd = zero;

	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, GPIO_PIN_RESET);

  	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
  	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

	#if defined(LCD_8BITMODE)
	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin,GPIO_PIN_RESET);
	#endif

	#if defined(LCD_RW_GPIO_Port) && defined(LCD_RW_Pin)
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port,LCD_RW_Pin,GPIO_PIN_RESET);
	#endif

	if (lines > 1) {
		lcd->_displayfunction |= LCD_2LINE;
	}
	lcd->_numlines = lines;

	setRowOffsets(lcd,0x00, 0x40, 0x00 + cols, 0x40 + cols);

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
		lcd->_displayfunction |= LCD_5x10DOTS;
  	}

    // Veja Pg 45/46 para especificações de inicialização!
    // De acordo com o datasheet, precisamos de pelo menos 40ms após a alimentação atingir 2.7V
    // Vamos aguardar 50ms para ter uma pequena margem
    HAL_Delay(50);

  	// Colocamos RS e RW em low para começar os comandos
  	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
  	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

	#if defined(LCD_RW_GPIO_Port) && defined(LCD_RW_Pin)
  	HAL_GPIO_WritePin(LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
	#endif

  	// Coloca o LCD em 4bit ou 8bit mode
  	if ((lcd->_displayfunction | LCD_4BITMODE)) {
		// Sequência a seguir conforme Hitachi HD44780 datasheet
		// Figura 24, pg 46

		// Começa com modo 8bit, então tenta configurar para  modo 4bit.
		write4bits(0x03);
		HAL_Delay(5); 		// wait min 4.1ms

		// Segundo envio
		write4bits(0x03);
		HAL_Delay(5); // wait min 4.1ms

		// Terceiro envio
		write4bits(0x03);
		HAL_Delay(5);

		// Configura para interface 4bit
		write4bits(0x02);
    }
    #if defined(LCD_8BITMODE)
    else {

		// Sequência a seguir conforme Hitachi HD44780 datasheet
		// Figura 23, pg 45 

		// Envia sequência de comando
		command(lcd,LCD_FUNCTIONSET | lcd->_displayfunction);
		HAL_Delay(6);  // wait more than 4.1 ms

		// Segundo envio
		command(lcd,LCD_FUNCTIONSET | lcd->_displayfunction);
		HAL_Delay(1);

		// Terceiro envio
		command(lcd,LCD_FUNCTIONSET | lcd->_displayfunction);
		HAL_Delay(1);
    }
	#endif

    // Configura quantidade de linhas, tamanho da fonte, etc
    command(lcd,LCD_FUNCTIONSET | lcd->_displayfunction);
    HAL_Delay(1);

    // Liga display com cursor piscante por padrão
    lcd->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display(lcd);

    // Limpa display
    clear(lcd);

    // Configura direção do texto padrão
    lcd->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // Configura entry mode
    command(lcd,LCD_ENTRYMODESET | lcd->_displaymode);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Imprime a string apontada por value no display                                                          						  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Ponteiro para string contendo o texto                                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void print(LiquidCrystal *lcd, int8_t *value){

	size_t Lengh = strlen(value);

	for (uint8_t i = 0; i < Lengh; i++){
		write(lcd ,*(value+i));
	}
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Limpa a tela LCD e posiciona o cursor no canto superior esquerdo.                                         						  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void clear(LiquidCrystal *lcd)
{
    command(lcd,LCD_CLEARDISPLAY);  // limpa display e seta cursor para posição 0,0
    HAL_Delay(2);  				    // aguarda execução por parte do display
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Posiciona o cursor no canto superior esquerdo do LCD. Ou seja, use esse local na saída de texto subsequente para a exibição.      */
/*              Para também limpar a mensagem, use a função clear().                                         						  			  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void home(LiquidCrystal *lcd){
	command(lcd,LCD_RETURNHOME);  // configura a posição do cursor para 0,0
	HAL_Delay(2);		          // este comando leva bastante tempo!
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Desliga o display LCD, sem perder o texto atualmente mostrado nele.															      */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void noDisplay(LiquidCrystal *lcd){
	lcd->_displaycontrol &= ~LCD_DISPLAYON;
	command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Liga o display LCD, após ter sido desligado com noDisplay(). Isso restaurará o texto (e o cursor) que estava na tela.		      */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void display(LiquidCrystal *lcd) {
    lcd->_displaycontrol |= LCD_DISPLAYON;
    command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Desliga o cursor piscante																									      */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void noBlink(LiquidCrystal *lcd){
    lcd->_displaycontrol &= ~LCD_BLINKON;
    command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Exibe o cursor piscante. Se usado em combinação com a função cursor(), o resultado dependerá das características do display. 	  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void blink(LiquidCrystal *lcd){
	lcd->_displaycontrol |= LCD_BLINKON;
	command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Oculta o cursor underscore (_)																									  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void noCursor(LiquidCrystal *lcd){
	lcd->_displaycontrol &= ~LCD_CURSORON;
	command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Habilita o cursor underscore (_) na posição em que o próximo caractere será inserido										      */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void cursor(LiquidCrystal *lcd){
	lcd->_displaycontrol |= LCD_CURSORON;
	command(lcd,LCD_DISPLAYCONTROL | lcd->_displaycontrol);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Desloca o conteúdo da tela (texto e cursor) uma posição para a esquerda.										    			  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void scrollDisplayLeft(LiquidCrystal *lcd){
	command(lcd,LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Desloca o conteúdo da tela (texto e cursor) uma posição para a direita.											    			  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void scrollDisplayRight(LiquidCrystal *lcd){
	command(lcd,LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Defina a direção do texto escrito no LCD da esquerda para a direita. 															  */
/*              Isso significa que os caracteres seguintes exibidos irão da esquerda para a direita, mas não afetam o texto de saída anterior     */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void leftToRight(LiquidCrystal *lcd){
	lcd->_displaymode |= LCD_ENTRYLEFT;
	command(lcd,LCD_ENTRYMODESET | lcd->_displaymode);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Defina a direção do texto escrito no LCD da direita para a esquerda (o padrão é da esquerda para a direita). 					  */
/*              Isso significa que os caracteres seguintes exibidoss irão da direita para a esquerda, mas não afetam o texto de saída anterior	  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void rightToLeft(LiquidCrystal *lcd){
	lcd->_displaymode &= ~LCD_ENTRYLEFT;
	command(lcd,LCD_ENTRYMODESET | lcd->_displaymode);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Ativa a rolagem automática do LCD. Isso faz com que cada novo caractere empurre os caracteres anteriores em um espaço. 			  */
/*              Se a direção do texto atual for da esquerda para a direita (o padrão), a tela rola para a esquerda. 							  */
/*				Se a direção atual for da direita para a esquerda, a tela rolará para a direita. 												  */
/*				Isso tem o efeito de enviar cada novo caractere para o mesmo local no LCD. 														  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void autoscroll(LiquidCrystal *lcd){
	lcd->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(lcd,LCD_ENTRYMODESET | lcd->_displaymode);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Desativa a rolagem automática do LCD. 																							  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void noAutoscroll(LiquidCrystal *lcd){
	lcd->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(lcd,LCD_ENTRYMODESET | lcd->_displaymode);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Escreve um único caractere no display. 																							  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Valor do caractere que deverá ser escrito			                                                                              */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void write(LiquidCrystal *lcd, uint8_t value) {
	send(lcd,value, 1);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Escreve no display. 																							 				  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Valor que deverá ser escrito			                                                                           			      */
/* @param3    : Tipo de informação que está sendo escrito (dado ou comando)                                                      			      */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void send(LiquidCrystal *lcd, uint8_t value, uint8_t mode) {

	HAL_GPIO_WritePin(LCD_RS_GPIO_Port,LCD_RS_Pin,mode);

	#if defined(LCD_RW_GPIO_Port) && defined(LCD_RW_Pin)
	HAL_GPIO_WritePin(LCD_RW_GPIO_Port,LCD_RW_Pin,GPIO_PIN_RESET);
	#endif

	#if defined(LCD_8BITMODE)
	  if (lcd->_displayfunction & LCD_8BITMODE) {
	    write8bits(value);
	  } else {
	#endif
	    write4bits(value>>4);
	    write4bits(value);
	#if defined(LCD_8BITMODE)
	  }
	#endif

}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Realiza o pulso no pino EN do display															   							      */
/*                                                                                                                                                */
/* @param1    : void												                                                                              */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void pulseEnable(void) {
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Realiza a escrita no modo 4 bits (envia nible MSN + pulso pino EN)																  */
/*                                                                                                                                                */
/* @param1    : Dado a ser enviado												                                                                  */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void write4bits(uint8_t value) {
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin,(value >> 0) & 0x01);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin,(value >> 1) & 0x01);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin,(value >> 2) & 0x01);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin,(value >> 3) & 0x01);

	pulseEnable();
}
/**************************************************************************************************************************************************/



#if defined(LCD_8BITMODE)
/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Realiza a escrita no modo 8 bits (enviabyte + pulso pino EN)																  	  */
/*                                                                                                                                                */
/* @param1    : Dado a ser enviado												                                                                  */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void write8bits(uint8_t value){
	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin,(value >> 0) & 0x01);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin,(value >> 1) & 0x01);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin,(value >> 2) & 0x01);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin,(value >> 3) & 0x01);
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin,(value >> 4) & 0x01);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin,(value >> 5) & 0x01);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin,(value >> 6) & 0x01);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin,(value >> 7) & 0x01);

	pulseEnable();

}
/**************************************************************************************************************************************************/
#endif


/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Envia comando ao display 																									  	  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Valor do comando a ser enviado												                                                      */
/*                                                                                                												  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void command(LiquidCrystal *lcd, uint8_t value) {
  send(lcd, value, 0);
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Configura offset das linhas. 																									  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Valor de linha 0												                                                                  */
/* @param3    : Valor de linha 0												                                                                  */
/* @param4    : Valor de linha 0												                                                                  */
/* @param5    : Valor de linha 0												                                                                  */
/*                               												                                                                  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void setRowOffsets(LiquidCrystal *lcd, int row0, int row1, int row2, int row3)
{
	lcd->_row_offsets[0] = row0;
	lcd->_row_offsets[1] = row1;
	lcd->_row_offsets[2] = row2;
	lcd->_row_offsets[3] = row3;
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Cria um caractere personalizado para uso no LCD. São suportados até oito caracteres de 5x8 pixels (numerados de 0 a 7). 		  */
/*              A aparência de cada caractere personalizado é especificada por uma matriz de oito bytes, um para cada linha. 					  */
/* 				Os cinco bits menos significativos de cada byte determinam os pixels nessa linha. Para exibir um caractere personalizado na tela, */
/* 	 	 	 	use a função write() com seu número. 																							  */
/*              OBS: Ao referenciar o caractere personalizado "0", se não estiver em uma variável, você precisa convertê-lo como um byte,         */
/*              caso contrário, o compilador gera um erro.                                                                                        */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Posição que caractere personalizado será armazenado  			                                                                  */
/* @param3    : Matriz com a formatação do caractere																							  */
/*                               												                                                                  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void createChar(LiquidCrystal *lcd, uint8_t location, uint8_t charmap[]){
	location &= 0x7; 				 // Temos apenas 8 posições (0 até 7)
	command(lcd, LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(lcd,charmap[i]);
	}
}
/**************************************************************************************************************************************************/



/**************************************************************************************************************************************************/
/*                                                                                                                                                */
/* @resumo:   : Posiciona o cursor do LCD; ou seja, defina o local no qual o texto subsequente será exibido 		  							  */
/*                                                                                                                                                */
/* @param1    : Ponteiro para display que será manipulado pela função                                                                             */
/* @param2    : Valor da coluna (começa em 0)  			                                                                 						  */
/* @param3    : Valor da linha (cmeça em 0)																							 			  */
/*                               												                                                                  */
/* @ret val   : void 																															  */
/*                                                                                                                                                */
/**************************************************************************************************************************************************/
void setCursor(LiquidCrystal *lcd, uint8_t col, uint8_t row)
{
	const size_t max_lines = sizeof(lcd->_row_offsets) / sizeof(*(lcd->_row_offsets));
	if ( row >= max_lines ) {
		row = max_lines - 1;    	 // Contamos linhas iniciando do 0
	}
	if ( row >= lcd->_numlines ) {
		row = lcd->_numlines - 1;    // contamos linhas iniciando em 0
	}
	command(lcd,LCD_SETDDRAMADDR | (col + lcd->_row_offsets[row]));
}
/**************************************************************************************************************************************************/
