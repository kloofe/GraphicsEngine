#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "Display.hpp"
#include "Matrix.hpp"

namespace
{
	int FPS = 60;
	int width = 640;
	int height = 480;

	std::vector<double> initialEyeVector = {0, 0, 200};
	std::vector<double> initialGazeVector = {0, 0, -1};
	
	enum MYKEYS {
	KEY_ESC, KEY_W, KEY_A, KEY_S, KEY_D
	};

	bool key[5] = {false, false, false, false, false};

}


Display::Display()
	: eventQueue{NULL}, timer{NULL}, display{NULL}, eyeVector{initialEyeVector}, gazeVector{initialGazeVector}
{
	al_init();
	al_init_primitives_addon();
	eventQueue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(width, height);
	
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_install_keyboard();
	al_install_mouse();

	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
	al_register_event_source(eventQueue, al_get_mouse_event_source());
}

Display::~Display()
{
	al_destroy_event_queue(eventQueue);
	al_destroy_timer(timer);
	al_destroy_display(display);
}

void Display::run()
{
	al_flip_display();
	al_start_timer(timer);

	while (!key[KEY_ESC])
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventQueue, &ev);

		bool redraw = false;

		if (ev.type == ALLEGRO_EVENT_TIMER)
			redraw = true;
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				key[KEY_ESC] = true;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_W)
				key[KEY_W] = true;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_A)
				key[KEY_A] = true;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_S)
				key[KEY_S] = true;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_D)
				key[KEY_D] = true;
			
			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				key[KEY_ESC] = false;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_W)
				key[KEY_W] = false;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_A)
				key[KEY_A] = false;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_S)
				key[KEY_S] = false;
			if (ev.keyboard.keycode ==  ALLEGRO_KEY_D)
				key[KEY_D] = false;

			redraw = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
		}

		if (redraw)
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));
			if(key[KEY_W])
			{
				eyeVector[0] += (gazeVector[0] * 0.1);
				eyeVector[1] += (gazeVector[1] * 0.1);
				eyeVector[2] += (gazeVector[2] * 0.1);
			}
			if(key[KEY_A])
			{				
				eyeVector[0] -= (1 * 0.1);
				eyeVector[1] -= (0 * 0.1);
				eyeVector[2] -= (0 * 0.1);
			}
			if(key[KEY_S])
			{
				eyeVector[0] -= (gazeVector[0] * 0.1);
				eyeVector[1] -= (gazeVector[1] * 0.1);
				eyeVector[2] -= (gazeVector[2] * 0.1);
			}
			if(key[KEY_D])
			{
				eyeVector[0] += (1 * 0.1);
				eyeVector[1] += (0 * 0.1);
				eyeVector[2] += (0 * 0.1);
			}
			al_flip_display();
		}
		std::cout << "eyeVectorX: " << eyeVector[0] << std::endl;
		std::cout << "eyeVectorY: " << eyeVector[1] << std::endl;
		std::cout << "eyeVectorZ: " << eyeVector[2] << std::endl;
		std::cout << "gazeVectorX: " << gazeVector[0] << std::endl;
		std::cout << "gazeVectorY: " << gazeVector[1] << std::endl;
		std::cout << "gazeVectorZ: " << gazeVector[2] << std::endl;
	}
}