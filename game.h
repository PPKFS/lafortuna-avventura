#ifndef GAME_H
#define GAME_H

#define NUM_COMMANDS 20
#define NUM_ROOMS 23
#define NUM_ITEMS 20


typedef struct 
{
	char* name;
	void (*command)(uint8_t);
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

extern command commands[NUM_COMMANDS];
extern uint8_t things[NUM_COMMANDS];
extern char* item_names[NUM_ITEMS];
extern char* item_descs[NUM_ITEMS];
extern uint8_t fixed[NUM_ITEMS];
extern uint8_t picked_up[NUM_ITEMS];
extern uint8_t edible[NUM_ITEMS];

extern room rooms[NUM_ROOMS];
extern uint8_t player_pos;

extern uint8_t is_submenu;
void init_game();
void print_player_pos();
void do_command(int selection);

void north(uint8_t);
void south(uint8_t);
void east(uint8_t);
void west(uint8_t); 
void look(uint8_t);
void examine(uint8_t);
void use(uint8_t); 
void eat(uint8_t);
void open(uint8_t); 
void close(uint8_t); 
void take(uint8_t);

#endif