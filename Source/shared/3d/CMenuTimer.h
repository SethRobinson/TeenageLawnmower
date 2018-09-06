//tiny class to help with menu timings
#include "all.h"
#include "CTimer.h"

class CPic;


class CMenuTimer
{
public:

	bool b_menu_locked;
	CTimer menu_timer;
	CPic *p_thing_clicked;
	int i_button_delay;

	CMenuTimer()
	{
	  i_button_delay = 300; //default
	  reset();
	}

	bool menu_active()
	{
	  if (b_menu_locked) return false;
	  return true;
	}
	void set_menu_locked(bool b_new)
	{
	  b_menu_locked = b_new;
	}
	
	void reset()
	{
	  b_menu_locked = false;
	  p_thing_clicked = NULL;

	}

    void set_thing_we_clicked(CPic *p_cpic)
	{
	  p_thing_clicked = p_cpic;
	  set_menu_locked(true);
	  //get the timer going
	 menu_timer.reset();
	 menu_timer.set_interval(i_button_delay);
	}

	bool time_to_act()
	{
		
		if (menu_timer.interval_reached())
		{
			//reset some stuff
			b_menu_locked = false;
		    return true;
		}
		return false;
	}
};