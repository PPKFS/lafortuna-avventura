#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define SD1 0
#define FINAL 10

int player_pos = SD1;
char* descriptions[FINAL-1] = 
{
	"You are standing in the SD card slot.\n"
};

void print_player_pos()
{
	display_string(descriptions[player_pos]);
}

void init_game()
{
	print_player_pos();
}