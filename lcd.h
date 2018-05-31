#define RS 1    /* BIT0 mask */ 
#define RW 2    /* BIT1 mask */ 
#define EN 4    /* BIT2 mask */


void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
