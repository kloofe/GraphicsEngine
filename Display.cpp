#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include "Matrix.hpp"
#include <cmath>
#include "Cube.hpp"

#include "Display.hpp"

namespace
{

   	int FPS = 60; 
    std::vector<double> initUVector = {0, 0, -1};
    std::vector<double> initTVector = {0, 1, 0};
	std::vector<double> initVVector = {1, 0, 0};
	std::vector<double> initWVector = {0, 1, 0};
	std::vector<double> initEVector = {0, 0, 200};
	
	enum MYKEYS {
	KEY_ESC, KEY_W, KEY_A, KEY_S, KEY_D
	};

	bool key[5] = {false, false, false, false, false};
}


Display::Display()
	: eventQueue{NULL}, timer{NULL}, display{NULL}, uVector{initUVector}, vVector{initVVector},wVector{initWVector}, eVector{initEVector}, tVector{initTVector}
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

    for(int i = 0; i < 4; i++) {
        std::cout << scale.getValue(i, i) << std::endl;
    }
    std::cout << scale.getValue(2, 3) << std::endl;

    convert2D.setValue(0, 0, (double) width/2);
    convert2D.setValue(0, 2, (double) width/2);
    convert2D.setValue(1, 1, (double) height/2);
    convert2D.setValue(1, 2, (double) height/2);
    convert2D.setValue(2, 2, 1);
    convert2D.setValue(3, 3, 1);


    updateVectors();

    calculateMCP();
    updateScreenPoints();
    for(int i = 0; i < 3; i++) {
        std::cout << vVector[i] << std::endl;
    }
    for(int i = 0; i < 3; i++) {
        std::cout << wVector[i] << std::endl;
    }


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
            updateScreenPoints();
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
    



		if (redraw)
		{
			al_clear_to_color(al_map_rgb(255, 255, 255));

            drawObjects();
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
    //draw cube
    updateScreenPoints();
    screenToCanvas();
    std::cout << "first point" << getX(screenPoints[0]) << " " << getY(screenPoints[0]) << std::endl; 
    for(int i = 0; i < 7; i++) {
        al_draw_line(getX(screenPoints[i]), getY(screenPoints[i]),
                     getX(screenPoints[i + 1]), getY(screenPoints[i + 1]), al_map_rgb(0, 0, 0), 1);
    
    }
    al_draw_line(getX(screenPoints[7]), getY(screenPoints[7]),
                 getX(screenPoints[4]), getY(screenPoints[4]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[1]), getY(screenPoints[1]),
                 getX(screenPoints[6]), getY(screenPoints[6]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[0]), getY(screenPoints[0]),
                 getX(screenPoints[7]), getY(screenPoints[7]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[0]), getY(screenPoints[0]),
                 getX(screenPoints[3]), getY(screenPoints[3]), al_map_rgb(0, 0, 0), 1);
}

double Display::getX(Matrix m) {
    return m.getValue(0, 0);
}

double Display::getY(Matrix m) {
    return m.getValue(1, 0);
}

void Display::screenToCanvas() {
    for(int i = 0; i < screenPoints.size(); i++) {
        screenPoints.at(i).setValue(1, 0, height - screenPoints.at(i).getValue(1, 0));
    }
}

void Display::initPoints() {
    std::vector<double> center = {0, 0, 0};
    Cube c{center, depth};
    shapes.push_back(c.getPoints());
}

void Display::updateVectors() {
    Matrix uMatrix{uVector};
    Matrix tMatrix{tVector};
    Matrix vMatrix{vVector};
    Matrix wMatrix{wVector};
    vMatrix = uMatrix.crossProduct(tMatrix).normalize();
    wMatrix = vMatrix.crossProduct(uMatrix);
    std::cout << "AH:SLDKFJS:LDKFJ " << std::endl;
    std::cout << wMatrix.getValue(0, 0) << " " << wMatrix.getValue(0, 1) << " " << wMatrix.getValue(0, 2) << std::endl;
    vVector = wMatrix.toVector();
    wVector = vMatrix.toVector();
}

void Display::calculateMCP() {
    Matrix temp1{4, 4};
    Matrix temp2{4, 4};
    for(int i = 0; i < vVector.size(); i++) {
        temp1.setValue(0, i, vVector[i]);
    }
    for(int i = 0; i < wVector.size(); i++) {
        temp1.setValue(1, i, wVector[i]);
    }
    for(int i = 0; i < uVector.size(); i++) {
        temp1.setValue(2, i, -uVector[i]);
    }
    temp1.setValue(3, 3, 1);

    for(int i = 0; i < 4; i++) {
        temp2.setValue(i, i, 1);
        if(i != 3) {
            temp2.setValue(i, 3, -eVector[i]);
        }
    }

    mcp = temp1 * temp2;
    
}

void Display::updateScreenPoints() {
    for(int i = 0; i < shapes.size(); i++) {
        std::vector<Matrix> cubePoints = shapes.at(i);
    }
    for(int i = 0; i < cubePoints.size(); i++) {
        Matrix temp = worldToScreen(cubePoints.at(i));
        screenPoints.at(i).setValue(0, 0, temp.getValue(0, 0));
        screenPoints.at(i).setValue(1, 0, temp.getValue(1, 0));
    }
}

Matrix Display::worldToScreen(Matrix point) {
    point = mcp * point;
    std::cout << point.getValue(0, 0) << " " << point.getValue(1, 0) << " " << point.getValue(2, 0) << std::endl;
    point = orthogonal * point;
    point = scale * point;
    point = convert2D * point;
    point = point * (1/point.getValue(2, 0));
    return point;
}
