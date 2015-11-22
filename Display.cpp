#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Matrix.hpp"

#include "Display.hpp"

namespace
{

   	int FPS = 60;
    enum MYKEYS {
	KEY_ESC, KEY_W, KEY_A, KEY_S, KEY_D
	};

	bool key[5] = {false, false, false, false, false};
}


Display::Display()
	: eventQueue{NULL}, timer{NULL}, display{NULL}
{
    initPoints();
    for(int i = 0; i < points.size(); i++) {
        Matrix temp(2, 1);
        screenPoints.push_back(temp);
    }

    orthogonal.setValue(0, 0, n);
    orthogonal.setValue(1, 1, n);
    orthogonal.setValue(2, 2, n + f);
    orthogonal.setValue(2, 3, -f * n);
    orthogonal.setValue(3, 2, 1);

    scale.setValue(0, 0, (double) 2/(r - l));
    scale.setValue(1, 1, (double) 2/(b - t));
    scale.setValue(2, 2, (double) 2/(n - f));
    scale.setValue(2, 3, (double) -(n + f)/(n - f));
    scale.setValue(3, 3, 1);

    convert2D.setValue(0, 0, (double) width/2);
    convert2D.setValue(0, 2, (double) width/2);
    convert2D.setValue(1, 1, (double) height/2);
    convert2D.setValue(1, 2, (double) height/2);
    convert2D.setValue(2, 3, 1);
    convert2D.setValue(3, 3, 1);

    calculateMCP();

    updateScreenPoints();
	al_init();
	al_init_primitives_addon();
	eventQueue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(width, height);
	
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_install_keyboard();

	al_register_event_source(eventQueue, al_get_display_event_source(display));
	al_register_event_source(eventQueue, al_get_timer_event_source(timer));
	al_register_event_source(eventQueue, al_get_keyboard_event_source());
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
			redraw = false;
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
    

        al_clear_to_color(al_map_rgb(255, 255, 255));
	    al_flip_display();

        drawObjects();

		if (redraw)
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));
			if(key[KEY_W])
				al_draw_line(0, 0, 640, 480, al_map_rgb(0, 0, 0), 1.0);
			if(key[KEY_A])
				al_draw_line(0, 480, 640, 0, al_map_rgb(0, 0, 0), 1.0);
			if(key[KEY_S])
				al_draw_line(0, 240, 640, 240, al_map_rgb(0, 0, 0), 1.0);
			if(key[KEY_D])
				al_draw_line(320, 0, 320, 480, al_map_rgb(0, 0, 0), 1.0);
			al_flip_display();
		}

	}
}

void Display::drawObjects() {
    
}

void Display::initPoints() {
    Matrix p1(-50, -50, 50, 1);
    Matrix p2(-50, 50, 50, 1);
    Matrix p3(50, 50, 50, 1);
    Matrix p4(50, -50, 50, 1);
    Matrix p5(50, -50, -50, 1);
    Matrix p6(50, 50, -50, 1);
    Matrix p7(-50, 50, -50, 1);
    Matrix p8(-50, -50, -50, 1);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points.push_back(p6);
    points.push_back(p7);
    points.push_back(p8);
}

void Display::calculateMCP() {
}

void Display::updateScreenPoints() {
    for(int i = 0; i < screenPoints.size(); i++) {
        Matrix temp = worldToScreen(points.at(i));
        screenPoints.at(i).setValue(0, 0, temp.getValue(0, 0));
        screenPoints.at(i).setValue(1, 0, temp.getValue(1, 0));
    }
}

Matrix Display::worldToScreen(Matrix point) {
    std::cout << "This not working huh" << std::endl;
    return point * mcp * orthogonal * scale * convert2D;
}
