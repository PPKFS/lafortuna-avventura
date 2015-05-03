#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "game.h"

#define SD1 0
#define OUTSIDE 1
#define NO_ROOM 255

#define DIR_NORTH 0
#define DIR_SOUTH 1
#define DIR_EAST 2
#define DIR_WEST 3

int player_pos = SD1;

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
	{"SD Card Slot", "You are standing in the SD card slot.\n", {255, 1, 255, 255}},
	{"Outside", "You are outside the LaFortuna. The sunlight burns you; you should probably go inside, fast!\n", {0, 255, 255, 255}}
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