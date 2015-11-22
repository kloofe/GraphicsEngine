#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>
#include "Matrix.hpp"
#include <cmath>
#include "Cube.hpp"

#include "Display.hpp"

namespace
{
   	int FPS = 24; 
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

    orthogonal.setValue(0, 0, n);
    orthogonal.setValue(1, 1, n);
    orthogonal.setValue(2, 2, n + f);
    orthogonal.setValue(2, 3, -f * n);
    orthogonal.setValue(3, 2, 1);

    updateScreenPoints();
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

    calculateMCP();
    updateScreenPoints();

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
 	    	al_set_mouse_xy(display, 320, 240);
 	    	if(abs(ev.mouse.dx) < 20 && abs(ev.mouse.dy) < 20) {
 	    	    delta_x = ev.mouse.dx;
 	    	    delta_y = -ev.mouse.dy;
 	    	}

 	    	double theta = atan2(-uVector[2], uVector[0]);
            double phi = acos(uVector[1]);

            std::cout << "THETA: " << theta << std::endl;
            std::cout << "PHI: " << phi << std::endl;

            theta += (0.005 * delta_y);
            phi -= (0.01 * delta_x);

            std::cout << "PHI 2: " << phi << std::endl;

            if (theta > 6.2831853)
                theta -= 6.2831853;
            if (theta <= 0)
                theta += 6.2831853;

            if (phi > 3.1415269)
                phi = 3.1415269;
            if (phi < 0)
                phi = 0;


            std::cout << "THETA 2: " << theta << std::endl;
            delta_x, delta_y = 0;

            uVector[0] = cos(theta) * sin(phi);
            uVector[1] = cos(phi);
            uVector[2] = -sin(theta) * sin(phi);
            std::cout << "VECTORSSSS " << uVector[0] << " " << uVector[1] << " " << uVector[2] << std::endl;
 	    	
     	}

		if(key[KEY_W])
		{
			eVector[0] += (uVector[0] * 0.4);
			eVector[1] += (uVector[1] * 0.4);
			eVector[2] += (uVector[2] * 0.4);
		}
		if(key[KEY_A])
		{				
			eVector[0] -= (vVector[0] * 0.4);
			eVector[1] -= (vVector[1] * 0.4);
			eVector[2] -= (vVector[2] * 0.4);
		}
		if(key[KEY_S])
		{
			eVector[0] -= (uVector[0] * 0.4);
			eVector[1] -= (uVector[1] * 0.4);
			eVector[2] -= (uVector[2] * 0.4);
		}
		if(key[KEY_D])
		{
			eVector[0] += (vVector[0] * 0.4);
			eVector[1] += (vVector[1] * 0.4);
			eVector[2] += (vVector[2] * 0.4);
		}

		

		al_clear_to_color(al_map_rgb(255, 255, 255));
		calculateMCP();
		drawObjects();
		al_flip_display();

    }
}

void Display::drawObjects() {
    //draw cube
    updateScreenPoints();
    screenToCanvas();
    for(int j = 0; j < screenPoints.size(); j++) {
        for(int i = 0; i < 7; i++) {
            al_draw_line(getX(screenPoints[j][i]), getY(screenPoints[j][i]),
                         getX(screenPoints[j][i + 1]), getY(screenPoints[j][i + 1]), al_map_rgb(0, 0, 0), 1);
        }
        al_draw_line(getX(screenPoints[j][7]), getY(screenPoints[j][7]),
                     getX(screenPoints[j][4]), getY(screenPoints[j][4]), al_map_rgb(0, 0, 0), 1);
        al_draw_line(getX(screenPoints[j][1]), getY(screenPoints[j][1]),
                     getX(screenPoints[j][6]), getY(screenPoints[j][6]), al_map_rgb(0, 0, 0), 1);
        al_draw_line(getX(screenPoints[j][0]), getY(screenPoints[j][0]),
                     getX(screenPoints[j][7]), getY(screenPoints[j][7]), al_map_rgb(0, 0, 0), 1);
        al_draw_line(getX(screenPoints[j][0]), getY(screenPoints[j][0]),
                     getX(screenPoints[j][3]), getY(screenPoints[j][3]), al_map_rgb(0, 0, 0), 1);
        al_draw_line(getX(screenPoints[j][2]), getY(screenPoints[j][2]),
                     getX(screenPoints[j][5]), getY(screenPoints[j][5]), al_map_rgb(0, 0, 0), 1);
    }
}

double Display::getX(Matrix m) {
    return m.getValue(0, 0);
}

double Display::getY(Matrix m) {
    return m.getValue(1, 0);
}

void Display::screenToCanvas() {
    for(int i = 0; i < screenPoints.size(); i++) {
        for(int j = 0; j < screenPoints.at(i).size(); j++) {
            screenPoints.at(i).at(j).setValue(1, 0, height - screenPoints.at(i).at(j).getValue(1, 0));
        }
    }
}

void Display::initPoints() {
    std::vector<double> center = {0, 0, 0};
    Cube c{center, 100};
    shapes.push_back(c.getPoints());
}

void Display::updateVectors() {
    Matrix uMatrix{uVector};
    Matrix tMatrix{tVector};
    Matrix vMatrix{vVector};
    Matrix wMatrix{wVector};
    vMatrix = uMatrix.crossProduct(tMatrix).normalize();
    wMatrix = vMatrix.crossProduct(uMatrix);
    vVector = wMatrix.toVector();
    wVector = vMatrix.toVector();
}

void Display::calculateMCP() {
	updateVectors();
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
    screenPoints.clear();
    for(int i = 0; i < shapes.size(); i++) {
        std::vector<Matrix> cubePoints = shapes.at(i);
        std::vector<Matrix> cubeScreenPoints;
        for(int j = 0; j < cubePoints.size(); j++) {
            Matrix temp = worldToScreen(cubePoints.at(j));
            Matrix temp2{2, 1};
            temp2.setValue(0, 0, temp.getValue(0, 0));
            temp2.setValue(1, 0, temp.getValue(1, 0));
            cubeScreenPoints.push_back(temp2);
        }
        screenPoints.push_back(cubeScreenPoints);
    }
}

Matrix Display::worldToScreen(Matrix point) {
    point = mcp * point;
    point = orthogonal * point;
    point = scale * point;
    point = convert2D * point;
    point = point * (1/point.getValue(2, 0));
    return point;
}
