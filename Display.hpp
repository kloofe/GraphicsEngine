#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <allegro5/allegro.h>
#include <vector>

#include "Matrix.hpp"

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
		std::vector<double> eyeVector;
		std::vector<double> gazeVector;

};

#endif // DISPLAY_HPP