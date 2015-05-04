#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "game.h"

#define OUTSIDE 0
#define SD1 1
#define CORRIDOR_1 2
#define UNUSED 3
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

#define IC_FLAVOUR "You are standing in a large, spacious room. This seems to be the main unit of the board."

#define SD_HELP_CARD 0
#define REMOVED_ITEM 254
#define NO_ITEM 255

#define DIR_NORTH 0
#define DIR_SOUTH 1
#define DIR_WEST 2
#define DIR_EAST 3

#define EVERY_TURN 0
#define DESCRIPTION 1
#define LED_SWITCH 2

uint8_t player_pos = OUTSIDE;

command commands[NUM_COMMANDS] = {
	{"north", north, 0}, 
	{"south", south, 0},
	{"east", east, 0},
	{"west", west, 0},
	{"inventory", inventory, 0},
	{"look", look, 0},
	{"examine", examine, 1},
	{"use", use, 1},
	{"take", take, 1},
	{"eat", eat, 1},
	{"open", open, 1},
	{"close", close, 1}};
 
uint8_t things[NUM_COMMANDS] = {255};

uint8_t is_submenu = 0;

char* item_names[NUM_ITEMS] = {0};
char* item_descs[NUM_ITEMS] = {0};
uint8_t fixed[NUM_ITEMS] = {0}; 
uint8_t picked_up[NUM_ITEMS] = {0}; 
uint8_t edible[NUM_ITEMS] = {0}; 


void remove_item(room* r, uint8_t item)
{
	uint8_t i = 0;
	for(; i < NUM_COMMANDS; i++)
	{
		if(r->items[i] == 255)
			return;
		else if(r->items[i] == item)
		{
			++i;
			break;
		}
	}

	for(; i < NUM_COMMANDS-1; ++i)
	{
		r->items[i-1] = r->items[i];
	}
}

void add_item(uint8_t id, char* name, char* desc, uint8_t fix, uint8_t ed)
{
	item_names[id] = name;
	item_descs[id] = desc;
	fixed[id] = fix;
	picked_up[id] = 0;
	edible[id] = ed;
}

void init_items()
{
	add_item(SD_HELP_CARD, "Help Manual", 
		"An SD card someone has scribbled 'Help Manual' over. It'd probably tell you stuff if you used it.", 0, 0);
	add_item(1, "Heavy Cheese Table", "A huge, heavy, table..made of cheese. Totally not here for mechanics reasons.", 1, 1);
}

void die()
{
	display_string("You have died. RIP!\nReset the board to play again.");
	while(1){};
}


void outside_room(void* r, uint8_t action)
{
	static uint8_t countdown = 3;

	switch(action)
	{
		case EVERY_TURN:
		if(countdown == 3)
			display_string("It's very hot out here. You should get inside before you hurt yourself from this heat.\n");
		else if(countdown == 2)
			display_string("You start to feel woozy in the heat. You're starting to get sunstroke.\n");
		else if(countdown == 1)
			display_string("You're on the verge of passing out. Get inside, quick!\n");
		else
		{
			display_string("You collapse from the heat. You are eaten by a passing sun-grue.\n");
			die();
		}
		countdown--;
		break;
		default:
		break;
	}
}

void above_scroll_room(void* r, uint8_t action)
{
	switch(action)
	{
		case EVERY_TURN:
		break;
		default:
		break;
	}
}

void led_room(void* r, uint8_t action)
{
	static uint8_t led_is_on = 0;
	switch(action)
	{
		case LED_SWITCH:
		if(led_is_on)
		{
			display_string("You turn the LED off.\n");
			led_is_on = 0;
		}
		else
		{
			display_string("You turn the LED on.\n");
			led_is_on = 1;
		}
		break;
		case EVERY_TURN:
		break;
		case DESCRIPTION:
		if(led_is_on)
			((room*)r)->desc = "You look down. You are standing on top of the LED. It is currently on, and the room is very bright.";
		else
		{
			((room*)r)->desc = 
		"You look down. You are standing on top of the LED. It is currently off, and the room is dark. The floor hums.";
		}
		break;
		default:
		break;
	}
}

void final_room(void* r, uint8_t action)
{
	switch(action)
	{
		case EVERY_TURN:
		break;
		default:
		break;
	}

}

void ded_room(room* r, uint8_t action)
{
	switch(action)
	{
		case EVERY_TURN:
		die();
		break;
		default:
		break;
	}
}

room rooms[NUM_ROOMS] = {
	{"Outside", 
	"You are outside the LaFortuna. It is really, really hot out here.", {NO_ROOM, NO_ROOM, SD1, NO_ROOM}, outside_room, {NO_ITEM}},
	{"SD Card Slot", "You are standing in the SD card slot.", {NO_ROOM, NO_ROOM, CORRIDOR_1, OUTSIDE}, 0, {0, 1, NO_ITEM}},
	{"corridor", "You find yourself in a dimly lit corridor. The walls are lined with pulsing green lights.", 
	{NO_ROOM, NO_ROOM, IC1, SD1}, 0, {NO_ITEM}},
	{"", "", {NO_ROOM, NO_ROOM, NO_ROOM, NO_ROOM}, 0, {NO_ITEM}},
	{"Integrated Circuit Unit, North Side", IC_FLAVOUR, {NO_ROOM, IC2, RESET, CORRIDOR_1}, 0, {NO_ITEM}},
	{"Integrated Circuit Unit, North-Central Side", IC_FLAVOUR, {IC1, IC3, AVCC, LED_ROOM}, 0, {NO_ITEM}},
	{"Integrated Circuit Unit, South-Central Side", IC_FLAVOUR, {IC2, IC4, USB, SCROLL_CORR}, 0, {NO_ITEM}},
	{"Integrated Circuit Unit, South Side", IC_FLAVOUR, {IC3, BATT, EXT_PIN_1, CORRIDOR_3}, 0, {NO_ITEM}},
	{"Reset Button", "Above you, you can see the reset switch. You could probably just about reach it from here.",
	 {NO_ROOM, NO_ROOM, NO_ROOM, IC1}, 0, {NO_ITEM}},
	{"AVCC Switch Room", "The walls pulse around you. Everything is oscillating wildly in here. It's giving you a headache.", 
	{NO_ROOM, NO_ROOM, NO_ROOM, IC2}, 0, {NO_ITEM}},
	{"LED Room", "", 
	{NO_ROOM, NO_ROOM, IC2, SD1}, led_room, {NO_ITEM}},
	{"USB Port", 
	"As you walk to the edge of the room, you look out the USB socket. It's a long, long way down. You'd probably die if you fell out.", 
	{NO_ROOM, NO_ROOM, OUTSIDE_2, IC3}, 0, {NO_ITEM}},
	{"Above Scroll Wheel", "You find yourself on a silicon catwalk. Below you is a circular room, split into 5 sections.", 
	{NO_ROOM, NO_ROOM, IC3, NO_ROOM}, above_scroll_room, {NO_ITEM}},
	{"Scroll Wheel", 
		"You climb down the ladder and step on the floor of the circular room. The floor seems to depress when you stand on it.", 
		{NO_ROOM, NO_ROOM, NO_ROOM, NO_ROOM}, 0, {NO_ITEM}},
	{"GPIO Pins", 
	"There are a number of holes above you, light streaming in.", {NO_ROOM, NO_ROOM, NO_ROOM, IC4}, 0, {NO_ITEM}},
	{"A Corridor", "You find yourself in a dimly lit corridor. The walls are lined with pulsing green lights.", 
		{EXT_PIN_2, ROTARY, IC4, CORRIDOR_FINAL}, 0, {NO_ITEM}},
	{"Rotary Encoder Control Room", "The room is spinning round and round and round and round...", {CORRIDOR_3, NO_ROOM, NO_ROOM, NO_ROOM}, 
		0, {NO_ITEM}},
	{"Battery Room Place", "Battery stuff.", {IC4, NO_ROOM, NO_ROOM, CAP_BANK}, 0, {NO_ITEM}},
	{"Capacitor Bank", "CAPACITORS ERRYWHERE.\n", {NO_ROOM, NO_ROOM, BATT, CAP_BANK}, 0, {NO_ITEM}},
	{"GPIO Pins", "There are a number of holes above you, light streaming in.\n", 
	{NO_ROOM, CORRIDOR_3, NO_ROOM, NO_ROOM}, 0, {NO_ITEM}},
	{"An Ominous Corridor", 
	"As you walk down the corridor, you have a very ominous feeling. Almost like something very bad is at the end of the corridor.", 
	{NO_ROOM, NO_ROOM, CORRIDOR_3, NO_ROOM}, 0, {NO_ITEM}},
	{"The LCD Screen", 
	"As you walk into the LCD screen, a cold breeze brushes over you. A giant dragon flies down from the ceiling and lands in front of you.", 
	{NO_ROOM, NO_ROOM, CORRIDOR_FINAL, NO_ROOM}, final_room, {NO_ITEM}},
	{"Outside, Falling", 
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA-splat. I told you it was a long way down.", 
	{NO_ROOM, NO_ROOM, NO_ROOM, NO_ROOM}, ded_room, {NO_ITEM}}
};

void display_items_in_room()
{
	uint8_t i = 0;
	if(rooms[player_pos].items[i] == 255)
		return;
	while(i < NUM_COMMANDS-1 && rooms[player_pos].items[i] != 255)
	{
		display_string("There is a ");
		display_string(item_names[rooms[player_pos].items[i]]);
		display_string(" here. ");
		++i;
	}
	display_string("\n");
}

void print_player_pos()
{
	display_string(rooms[player_pos].name);
	display_string("\n");
	if(rooms[player_pos].update != 0)
	{
		display_string("\n");
		rooms[player_pos].update((void*)(&rooms[player_pos]), DESCRIPTION);
	}
	display_string(rooms[player_pos].desc);
	display_string("\n");
	display_items_in_room();

	uint8_t i = 0;
	for(;i < 4; ++i)
	{
		if(rooms[player_pos].connections[i] == NO_ROOM)
			continue;
		display_string("To the ");
		display_string(i == DIR_NORTH ? "north" : i == DIR_SOUTH ? "south" : i == DIR_WEST ? "west" : "east");
		display_string(" you can go to the ");
		display_string(rooms[rooms[player_pos].connections[i]].name);
		display_string(".\n");
	}
}

void init_game()
{
	init_items();
	print_player_pos();
}

void do_command(int selection)
{
	if(!is_submenu)
	{
		commands[selection].command(255);
	}
	else
	{
		commands[is_submenu].command(selection);
	}
	if(rooms[player_pos].update != 0)
	{
		display_string("\n");
		rooms[player_pos].update((void*)(&rooms[player_pos]), EVERY_TURN);
	}
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

void north(uint8_t item)
{
	go(DIR_NORTH);
}
void south(uint8_t item)
{
	go(DIR_SOUTH);
}
void east(uint8_t item)
{
	go(DIR_EAST);
}
void west(uint8_t item)
{
	go(DIR_WEST);
} 

void look(uint8_t item)
{
	print_player_pos();
}

void examine(uint8_t item)
{
	if(item == 255)
		display_string("It's you, our intrepid adventurer and COMP2215 student!");
	else
		display_string(item_descs[item]);
}

void use(uint8_t item)
{
	if(player_pos == LED_ROOM)
		rooms[LED_ROOM].update((void*)(&rooms[LED_ROOM]), LED_SWITCH);
}

void inventory(uint8_t item)
{
	display_string("You are carrying: \n");
	uint8_t i = 0;
	for(; i < NUM_ITEMS; ++i)
	{
		if(picked_up[i])
		{
			display_string(item_names[i]);
			display_string(" ");
		}
	}
}

void take(uint8_t it)
{
	if(it == 255)
		display_string("You can't take yourself!\n");
	else
	{
		if(fixed[it])
		{
			display_string("You can't take that!\n");
		}
		else if(picked_up[it])
		{
			display_string("You already have that!\n");
		}
		else
		{
			display_string("You take the ");
			display_string(item_names[it]);
			display_string(" and put it in your bag.");
			picked_up[it] = 1;
			remove_item(&rooms[player_pos], it);
		}
	}
}

void eat(uint8_t it)
{
	if(it == 255)
		display_string("Don't eat yourself. That's...ew.\n");
	else
	{
		if(!edible[it])
		{
			display_string("That doesn't look overly edible.\n");
		}
		else
		{
			display_string("You eat the ");
			display_string(item_names[it]);
			display_string(". Delicious!");
			picked_up[it] = 0;
			remove_item(&rooms[player_pos], it);
		}
	}

}
void open(uint8_t item)
{

}

void close(uint8_t item)
{

}