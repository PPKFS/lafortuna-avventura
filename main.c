#include <avr/io.h>
#include <util/delay.h>
#include "lcd/lcd.h"

void init_lafortuna(void);
void set_command_pos(int pos);

void main(void)
{
    init_lafortuna();
    display_string("Welcome to LaFortuna Avventura!\n\n");
    init_game();
    set_command_pos(0);

    _delay_ms(100);
}


void init_laforuna(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}

void set_command_pos(int pos)
{

}