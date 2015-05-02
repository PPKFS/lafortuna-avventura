#include "buttons.h"
#include <avr/io.h>

int down[3] = {0, 0, 0};
int any_down = 0;

int is_pressed(int button)
{
	int pin = (button == 0 ? ~PINE : ~PINC) & (button == 0 ? _BV(PE7) : (button == 1 ? _BV(PC5) : _BV(PC3)));
	if(pin && !down[button] && !any_down)
	{
		down[button] = 1;
		any_down = 1;
		return 1;
	}
	else if(pin && down[button])
	{
		down[button] = 0;
		any_down = 0;
	}
	return 0;
}