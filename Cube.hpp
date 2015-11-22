#ifndef CUBE_HPP
#define CUBE_HPP

#include "Matrix.hpp"
#include <vector>

class Cube {
public: 
    Cube();
    Cube(std::vector<double> c, double d);

    std::vector<Matrix> getPoints() const;
private:
    std::vector<Matrix> points;
    double depth;
    std::vector<double> center;
    void createPoints();
};


#endif
