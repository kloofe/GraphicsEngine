#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>

#include "Display.hpp"
#include "Matrix.hpp"

namespace
{
	int FPS = 1;
	int width = 640;
	int height = 480;

	std::vector<double> initUVector = {0, 0, -1};
	std::vector<double> initVVector = {-1, 0, 0};
	std::vector<double> initWVector = {0, 1, 0};
	std::vector<double> initEVector = {0, 0, 200};
	
	enum MYKEYS {
	KEY_ESC, KEY_W, KEY_A, KEY_S, KEY_D
	};

	bool key[5] = {false, false, false, false, false};

}


Display::Display()
	: eventQueue{NULL}, timer{NULL}, display{NULL}, uVector{initUVector}, vVector{initVVector}, wVector{initWVector}, eVector{initEVector}
{
    /*initPoints();
    for(int i = 0; i < points.size(); i++) {
        Matrix temp(2, 1);
        screenPoints.push_back(temp);
    }

    orthogonal.setValue(0, 0, n);
    orthogonal.setValue(1, 1, n);
    orthogonal.setValue(2, 2, n + f);
    orthogonal.setValue(2, 3, -f * n);
    orthogonal.setValue(3, 2, 1);

    updateScreenPoints();*/
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
	int delta_x = 0;
	int delta_y = 0;

	while (!key[KEY_ESC])
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(eventQueue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER){}
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
		}
 	    else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
 	    {
 	    	delta_x = ev.mouse.dx;
 	    	delta_y = -ev.mouse.dy;

 	    	al_set_mouse_xy(display, 320, 240);
     	}

		al_clear_to_color(al_map_rgb(255, 255, 255));
		if(key[KEY_W])
		{
			eVector[0] += (uVector[0] * 0.1);
			eVector[1] += (uVector[1] * 0.1);
			eVector[2] += (uVector[2] * 0.1);
		}
		if(key[KEY_A])
		{				
			eVector[0] += (vVector[0] * 0.1);
			eVector[1] += (vVector[1] * 0.1);
			eVector[2] += (vVector[2] * 0.1);
		}
		if(key[KEY_S])
		{
			eVector[0] -= (uVector[0] * 0.1);
			eVector[1] -= (uVector[1] * 0.1);
			eVector[2] -= (uVector[2] * 0.1);
		}
		if(key[KEY_D])
		{
			eVector[0] -= (vVector[0] * 0.1);
			eVector[1] -= (vVector[1] * 0.1);
			eVector[2] -= (vVector[2] * 0.1);
		}

		double theta = atan2(-uVector[2], uVector[0]);
		double phi = acos(uVector[1]);

		std::cout << "THETA: " << theta << std::endl;
		std::cout << "PHI: " << phi << std::endl;

		theta += (0.01 * delta_x);
		phi -= (0.02 * delta_y);

		std::cout << "THETA 2: " << theta << std::endl;
		std::cout << "PHI 2: " << phi << std::endl;

		if (theta > 6.2831853)
			theta -= 6.2831853;
		if (theta < 0)
			theta += 6.2831853;

		if (phi > 3.1415269)
			phi = 3.1415269;
		if (phi < 0)
			phi = 0;

		std::cout << "F THETA: " << theta << std::endl;
		std::cout << "F PHI: " << phi << std::endl;

		delta_x, delta_y = 0;

		uVector[0] = cos(theta) * sin(phi);
		uVector[1] = cos(phi);
		uVector[2] = -sin(theta) * sin(phi);

		al_flip_display();

		std::cout << "eyeVectorX: " << eVector[0] << std::endl;
		std::cout << "eyeVectorY: " << eVector[1] << std::endl;
		std::cout << "eyeVectorZ: " << eVector[2] << std::endl;
		std::cout << "gazeVectorX: " << uVector[0] << std::endl;
		std::cout << "gazeVectorY: " << uVector[1] << std::endl;
		std::cout << "gazeVectorZ: " << uVector[2] << std::endl;
	}
}

/*void Display::drawObjects() {
    
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

void Display::updateScreenPoints() {
    for(int i = 0; i < screenPoints.size(); i++) {
        Matrix temp = worldToScreen(points.at(i));
        screenPoints.at(i).setValue(0, 0, temp.getValue(0, 0));
        screenPoints.at(i).setValue(1, 0, temp.getValue(1, 0));
    }
}

Matrix Display::worldToScreen(Matrix point) {
    return point * mcp * orthogonal * scale * convert2D;
}*/
