#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <allegro5/allegro.h>

class Display
{
	public:
		Display();
		~Display();

		void run();

	private:
		ALLEGRO_EVENT_QUEUE *eventQueue;
		ALLEGRO_TIMER *timer;
		ALLEGRO_DISPLAY *display;
};

#endif // DISPLAY_HPP