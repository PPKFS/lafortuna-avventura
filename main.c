#include <avr/io.h>
#include "os.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include "rios.h"
#include "ruota.h"
#include "ff.h"
#include "game.h"

#define STEP_DELAY_MS 5
#define MIN_STEP    2    /* > 0 */
#define MAX_STEP  255

void init_lafortuna(void);
void set_command_pos(int);
int check_switches(int);

int position = 0;

void main(void)
{
    os_init();
    os_add_task( check_switches,  100, 1);
    sei();

    display_string("Welcome to LaFortuna Avventura!\n\n");
    init_game();
    set_command_pos(0);
    while(1)
    {
    	/*if(get_switch_press(_BV(SWE)))
    		display_string("Right");
    	if(get_switch_press(_BV(SWW)))
    		display_string("Left");
    	if(get_switch_press(_BV(SWC)))
    		display_string("Centre");
    	_delay_ms(500);*/
    }
}

int check_switches(int state) {
	
	if (get_switch_press(_BV(SWN))) {
		display_string("North audio!\n");
	}
		
	if (get_switch_press(_BV(SWE))) {	
		display_string("East audio!\n");
	}
		
	if (get_switch_press(_BV(SWS))) {
		display_string("South audio!\n");
		  }
		
	if (get_switch_press(_BV(SWW))) {
		display_string("West audio!\n");
	}

	if (get_switch_short(_BV(SWC))) {
			display_string("[S] Centre\n");
	}

	if (get_switch_rpt(_BV(SWN))) {
			display_string("[R] North\n");
	}
		
	if (get_switch_rpt(_BV(SWE))) {
			display_string("[R] East\n");
	}
		
	if (get_switch_rpt(_BV(SWS))) {
			display_string("[R] South\n");
	}
		
	if (get_switch_rpt(_BV(SWW))) {
			display_string("[R] West\n");
	}

	if (get_switch_rpt(SWN)) {
			display_string("[R] North\n");
	}

	return state;	
}

void set_command_pos(int pos)
{

}