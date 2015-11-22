#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <allegro5/allegro.h>
#include "Matrix.hpp"

class Display
{
	public:
		Display();
		~Display();

		void run();

	private:
        double n = 100;
        double f = -200;
        int width = 640;
        int height = 480;
        double r = width/8;
        double l = -width/8;
        double t = height/8;
        double b = -height/8;
        


        Matrix orthogonal{4, 4};
        Matrix scale{4, 4};
        Matrix convert2D{4, 4};
        Matrix mcp{4, 4};
        
        void calculateMCP();
        std::vector<Matrix> points;
        std::vector<Matrix> screenPoints;
        void initPoints();
        void drawObjects();
        void updateScreenPoints();
        Matrix worldToScreen(Matrix point);
		ALLEGRO_EVENT_QUEUE *eventQueue;
		ALLEGRO_TIMER *timer;
		ALLEGRO_DISPLAY *display;
};

#endif // DISPLAY_HPP
