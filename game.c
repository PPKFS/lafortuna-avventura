#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "game.h"

#define OUTSIDE 0
#define SD1 1
#define CORRIDOR_1 2
#define CORRIDOR_2 3
#define IC1 4
#define IC2 5
#define IC3 6
#define IC4 7
#define RESET 8
#define AVCC 9
#define LED_ROOM 10
#define USB 11
#define SCROLL_CORR 12
#define SCROLL_HUB 13
#define EXT_PIN_1 14
#define CORRIDOR_3 15
#define ROTARY 16
#define BATT 17
#define CAP_BANK 18
#define EXT_PIN_2 19
#define CORRIDOR_FINAL 20
#define LCD_SCREEN_ROOM 21
#define OUTSIDE_2 22

#define NO_ROOM 255

#define DIR_NORTH 0
#define DIR_SOUTH 1
#define DIR_WEST 2
#define DIR_EAST 3

int player_pos = OUTSIDE;

command commands[NUM_COMMANDS] = {
	{"north", north, 0}, 
	{"south", south, 0},
	{"east", east, 0},
	{"west", west, 0},
	{"look", look, 0},
	{"examine", examine, 1},
	{"use", use, 1},
	{"eat", eat, 1},
	{"open", open, 1},
	{"close", close, 1}};
 
command* active_commands = commands;

room rooms[NUM_ROOMS] = {
	{"Outside", "You are outside the LaFortuna. The sunlight burns you; you should probably go inside, fast!\n", {NO_ROOM, SD1, NO_ROOM, NO_ROOM}},
	{"SD Card Slot", "You are standing in the SD card slot.\n", {NO_ROOM, NO_ROOM, CORRIDOR_1, OUTSIDE}},
	{"A Corridor", "You find yourself in a dimly lit corridor. The walls are lined with pulsing green lights.\n", {NO_ROOM, NO_ROOM, CORRIDOR_2, SD1}},
	{"A Corridor", "The corridor seems darker now. The lights are all off.\n", {NO_ROOM, NO_ROOM, IC1, CORRIDOR_1}},
	{"Integrated Circuit Unit, North Side", "You are standing in a large, spacious room. This seems to be the main unit of the board.\n", {NO_ROOM, IC2, RESET, CORRIDOR_2}},
	{"Integrated Circuit Unit, North-Central Side", "You are standing in a large, spacious room. This seems to be the main unit of the board.\n", {IC1, IC3, AVCC, LED_ROOM}},
	{"Integrated Circuit Unit, South-Central Side", "You are standing in a large, spacious room. This seems to be the main unit of the board.\n", {IC2, IC4, USB, SCROLL_CORR}},
	{"Integrated Circuit Unit, South Side", "You are standing in a large, spacious room. This seems to be the main unit of the board.\n", {IC3, BATT, EXT_PIN_1, CORRIDOR_3}},
	{"Rotary Encoder Control Room", "You are standing in a large, spacious room. This seems to be the main unit of the board.\n", {CORRIDOR_3, NO_ROOM, NO_ROOM, NO_ROOM}},
};

void print_player_pos()
{
	display_string(rooms[player_pos].name);
	display_string("\n");
	display_string(rooms[player_pos].desc);
	uint8_t i = 0;
	for(;i < 4; ++i)
	{
		if(rooms[player_pos].connections[i] == NO_ROOM)
			continue;
		display_string("To the ");
		display_string(i == DIR_NORTH ? "north" : i == DIR_SOUTH ? "south" : i == DIR_WEST ? "west" : "east");
		display_string(" you can go to the ");
		display_string(rooms[rooms[player_pos].connections[i]].name);
		display_string(". ");
	}
}

void init_game()
{
	print_player_pos();
}

void do_command(int selection)
{
	commands[selection].command();
}

void go(uint8_t dir)
{
	if(rooms[player_pos].connections[dir] == NO_ROOM)
	{
		display_string("You can't go that way.");
		return;
	}
	else
	{
		player_pos = rooms[player_pos].connections[dir];
		print_player_pos();
	}
}

void north()
{
	go(DIR_NORTH);
}
void south()
{
	go(DIR_SOUTH);
}
void east()
{
	go(DIR_EAST);
}
void west()
{
	go(DIR_WEST);
} 

void look()
{
	print_player_pos();
}
void examine()
{

}
void use()
{

}
void eat()
{

}
void open()
{

}
void close()
{

}