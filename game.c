#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define SD1 0

int player_pos = SD1;
char** descriptions = 
{
	"You are standing in the SD card slot."
};

void print_player_pos()
{
	display_string(descriptions[player_pos]+"\n");
}

void init_game()
{
	print_player_pos();
}