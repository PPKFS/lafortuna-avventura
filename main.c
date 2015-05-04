#include <avr/io.h>
#include "os.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include "rios.h"
#include "ruota.h"
#include "ff.h"
#include "game.h"

#define CMD_SIZE 80
#define CMD_TOP 200
#define CMD_X_MAX 3
#define CMD_Y_MAX 5

#define TITLE_X 40
#define TITLE_Y 18

#define NELEM(x) (sizeof(x)/(sizeof(x[0])))
void init_lafortuna(void);
void set_command_pos(int);
int check_switches(int);

int select_x = 0;
int select_y = 0;

int things_size = 0;

void display_welcome()
{
	display_string_xy("Welcome to LaFortuna Avventura!\nThe text adventure game in your microcontroller!\n----------\n", TITLE_X, 0);
	display.x = 0;
}

void display_rect()
{
	rectangle r = {select_x*CMD_SIZE, (select_x*CMD_SIZE)+CMD_SIZE, (select_y*8)+CMD_TOP, (select_y*8)+CMD_TOP+8};
	rectangle r2 = {r.left + 1, r.right-1, r.top+1, r.bottom-1};
	fill_rectangle(r, 0xFFFF);
	fill_rectangle(r2, 0x0000);

}

uint8_t min(uint8_t a, uint8_t b)
{
	return a <= b ? a : b;
}

void get_list_of_things()
{
	uint8_t i = 0;
	for(i = 0; i < NUM_COMMANDS; ++i)
		things[i] = 0;
	things[0] = 255;
	for(i = 1; i <min(NUM_COMMANDS, NELEM(rooms[player_pos].items)); ++i)
	{
		if(rooms[player_pos].items[i-1] == 255)
			break;
		if(rooms[player_pos].items[i-1] == 254)
		{
			continue;
		}
		things[i] = rooms[player_pos].items[i-1];
	}
	things_size = i;
}

void update_select()
{
	rectangle clr = {0, display.width, CMD_TOP, display.height};
	fill_rectangle(clr, 0x0000);
	if(select_x < 0)
		select_x = 0;
	if(select_y < 0)
		select_y = 0;
	if(select_y >= CMD_Y_MAX)
		select_y = CMD_Y_MAX;
	if(select_x >= CMD_X_MAX)
		select_x = CMD_X_MAX;

	display_rect();

	uint8_t i = 0;
	if(!is_submenu)
	{
		for(i = 0; i < NELEM(commands); ++i)
		{
			uint8_t x = (i / CMD_Y_MAX) * 80;
			uint8_t y = (i % (CMD_Y_MAX)) * 8 + CMD_TOP;
			display_string_xy(commands[i].name, x, y);
		}
	}
	else
	{
		get_list_of_things();
		for(i = 0; i < things_size; ++i)
		{
			uint8_t x = (i / CMD_Y_MAX) * 80;
			uint8_t y = (i % (CMD_Y_MAX)) * 8 + CMD_TOP;
			display_string_xy(things[i] == 255 ? "yourself" : item_names[things[i]], x, y);	
		}
	}

}

void clear_main()
{
	rectangle r = {0, display.width, 0, CMD_TOP};
	fill_rectangle(r, 0x0000);
	display_welcome();
}

void do_select()
{
	update_select();
	int selection = (select_x*CMD_Y_MAX) + select_y;
	clear_main();
	if(!is_submenu)
	{
		if(selection >= NELEM(commands))
			return;
		if(!commands[selection].actions)
		{		
			display_string(">");
			display_string(commands[selection].name);
			display_string("\n");
			do_command(selection);
		}
		else
		{
			display_string("What do you want to ");
			display_string(commands[selection].name);
			display_string("?\n");
			is_submenu = selection;
			update_select();
		}
	}
	else
	{
		if(selection >= things_size)
			return;
		display_string(">");
		display_string(item_names[things[selection]]);
		display_string("\n");

		do_command(things[selection]);

		is_submenu = 0;
		update_select();
	}
}


void main(void)
{
    os_init();
    os_add_task(check_switches,  100, 1);
    sei();

    display_welcome();
    init_game();
    update_select();
    while(1)
    {
    }
}

int check_switches(int state) {
		
	if (get_switch_press(_BV(SWE))) 
	{	
		select_x++;
		update_select();
	}	
	else if (get_switch_press(_BV(SWW))) 
	{
		select_x--;
		update_select();
	}
	else if (get_switch_press(_BV(SWN))) 
	{	
		select_y--;
		update_select();
	}	
	else if (get_switch_press(_BV(SWS))) 
	{
		select_y++;
		update_select();
	}

	if (get_switch_press(_BV(SWC))) 
	{
		do_select();
	}

	return state;	
}