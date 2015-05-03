#ifndef GAME_H
#define GAME_H

#define NUM_COMMANDS 10
#define NUM_ROOMS 10


typedef struct 
{
	char* name;
	void (*command)();
	uint8_t actions;
} command;

typedef struct
{
	char* name;
	char* desc;
	uint8_t connections[4];

} room;

extern command commands[NUM_COMMANDS];
 
extern command* active_commands;

extern room rooms[NUM_ROOMS];

void init_game();
void print_player_pos();
void do_command(int selection);

void north();
void south();
void east();
void west(); 
void look();
void examine();
void use(); 
void eat();
void open(); 
void close(); 

#endif