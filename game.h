#ifndef GAME_H
#define GAME_H

#define NUM_COMMANDS 20
#define NUM_ROOMS 23
#define NUM_ITEMS 20


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
	void (*update)(void*, uint8_t);
	uint8_t items[NUM_COMMANDS-1];
} room;

typedef struct
{
	char* name;
	char* desc;
	uint8_t flags;
} item;

extern command commands[NUM_COMMANDS];
extern char* things[NUM_COMMANDS];
extern item items[NUM_ITEMS];

extern room rooms[NUM_ROOMS];
extern uint8_t player_pos;

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