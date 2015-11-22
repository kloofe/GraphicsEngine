#include <allegro5/allegro.h> 
#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <iostream>
#include "Matrix.hpp"
#include <cmath>

#include "Display.hpp"

namespace
{
/*<<<<<<< HEAD
	int FPS = 1;
	int width = 640;
	int height = 480;
=======
>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced*/

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
/*<<<<<<< HEAD
	: eventQueue{NULL}, timer{NULL}, display{NULL}, uVector{initUVector}, vVector{initVVector}, wVector{initWVector}, eVector{initEVector}
=======*/
	: eventQueue{NULL}, timer{NULL}, display{NULL}, uVector{initUVector}, vVector{initVVector},wVector{initWVector}, eVector{initEVector}, tVector{initTVector}
/*>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced*/
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

//>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced
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

//<<<<<<< HEAD
		if (ev.type == ALLEGRO_EVENT_TIMER){}
/*=======
		bool redraw = false;

		if (ev.type == ALLEGRO_EVENT_TIMER)
			redraw = false;
>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced*/
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
            //updateScreenPoints();
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
 	    	std::cout << "DELTAS AHHHHH ----------------------" << delta_x << " " << delta_y << std::endl;

 	    	double theta = atan2(-uVector[2], uVector[0]);
		double phi = acos(uVector[1]);

		std::cout << "THETA: " << theta << std::endl;
		std::cout << "PHI: " << phi << std::endl;

		theta += (0.005 * delta_y);
		phi -= (0.01 * delta_x);

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

 	    	
     	}

		//al_clear_to_color(al_map_rgb(255, 255, 255));
		if(key[KEY_W])
		{
			eVector[0] += (uVector[0] * 0.1);
			eVector[1] += (uVector[1] * 0.1);
			eVector[2] += (uVector[2] * 0.1);
		}
		if(key[KEY_A])
		{				
			eVector[0] -= (vVector[0] * 0.1);
			eVector[1] -= (vVector[1] * 0.1);
			eVector[2] -= (vVector[2] * 0.1);
		}
//<<<<<<< HEAD
		if(key[KEY_S])
		{
			eVector[0] -= (uVector[0] * 0.1);
			eVector[1] -= (uVector[1] * 0.1);
			eVector[2] -= (uVector[2] * 0.1);
		}
		if(key[KEY_D])
		{
			eVector[0] += (vVector[0] * 0.1);
			eVector[1] += (vVector[1] * 0.1);
			eVector[2] += (vVector[2] * 0.1);
		}

		

		al_clear_to_color(al_map_rgb(255, 255, 255));
		calculateMCP();
		drawObjects();
		al_flip_display();

/*		std::cout << "eyeVectorX: " << eVector[0] << std::endl;
		std::cout << "eyeVectorY: " << eVector[1] << std::endl;
		std::cout << "eyeVectorZ: " << eVector[2] << std::endl;
		std::cout << "gazeVectorX: " << uVector[0] << std::endl;
		std::cout << "gazeVectorY: " << uVector[1] << std::endl;
		std::cout << "gazeVectorZ: " << uVector[2] << std::endl;*/
	}
}

/*void Display::drawObjects() {
=======
    



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
}*/

void Display::drawObjects() {
    //draw cube
    updateScreenPoints();
    screenToCanvas();
    std::cout << "first point" << getX(screenPoints[0]) << " " << getY(screenPoints[0]) << std::endl; 
    for(int i = 0; i < 7; i++) {
        al_draw_line(getX(screenPoints[i]), getY(screenPoints[i]),
                     getX(screenPoints[i + 1]), getY(screenPoints[i + 1]), al_map_rgb(0, 0, 0), 1);
    std::cout << i << " to " << i + 1 << std::endl;
//>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced
    
    }
    al_draw_line(getX(screenPoints[7]), getY(screenPoints[7]),
                 getX(screenPoints[4]), getY(screenPoints[4]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[1]), getY(screenPoints[1]),
                 getX(screenPoints[6]), getY(screenPoints[6]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[0]), getY(screenPoints[0]),
                 getX(screenPoints[7]), getY(screenPoints[7]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[0]), getY(screenPoints[0]),
                 getX(screenPoints[3]), getY(screenPoints[3]), al_map_rgb(0, 0, 0), 1);
    al_draw_line(getX(screenPoints[2]), getY(screenPoints[2]),
                 getX(screenPoints[5]), getY(screenPoints[5]), al_map_rgb(0, 0, 0), 1);




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
    makeCube(center, 100);
}

void Display::makeCube(std::vector<double> center, double depth) {
    Matrix p1(center[0] - depth/2, center[1] - depth/2, center[2] + depth/2, 1);
    Matrix p2(center[0] - depth/2, center[1] + depth/2, center[2] + depth/2, 1);
    Matrix p3(center[0] + depth/2, center[1] + depth/2, center[2] + depth/2, 1);
    Matrix p4(center[0] + depth/2, center[1] - depth/2, center[2] + depth/2, 1);
    Matrix p5(center[0] + depth/2, center[1] - depth/2, center[2] - depth/2, 1);
    Matrix p6(center[0] + depth/2, center[1] + depth/2, center[2] - depth/2, 1);
    Matrix p7(center[0] - depth/2, center[1] + depth/2, center[2] - depth/2, 1);
    Matrix p8(center[0] - depth/2, center[1] - depth/2, center[2] - depth/2, 1);
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    points.push_back(p4);
    points.push_back(p5);
    points.push_back(p6);
    points.push_back(p7);
    points.push_back(p8);
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
    for(int i = 0; i < screenPoints.size(); i++) {
        Matrix temp = worldToScreen(points.at(i));
        screenPoints.at(i).setValue(0, 0, temp.getValue(0, 0));
        screenPoints.at(i).setValue(1, 0, temp.getValue(1, 0));
    }
}

Matrix Display::worldToScreen(Matrix point) {
//<<<<<<< HEAD
/*	calculateMCP();
    return point * mcp * orthogonal * scale * convert2D;
}
/*=======*/
    point = mcp * point;
    std::cout << point.getValue(0, 0) << " " << point.getValue(1, 0) << " " << point.getValue(2, 0) << std::endl;
    point = orthogonal * point;
    point = scale * point;
    point = convert2D * point;
    point = point * (1/point.getValue(2, 0));
    return point;
}
//>>>>>>> 302c617a2fecea8623d8c3e269b8e0aaf8cd3ced*/
