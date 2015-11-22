#include "Cube.hpp"


Cube::Cube() 
    : depth{100} {
    center.push_back(0);
    center.push_back(0);
    center.push_back(0);
    createPoints();
}

Cube::Cube(std::vector<double> c, double d)
    : center{c}, depth{d} {
    createPoints();
}

std::vector<Matrix> Cube::getPoints() const {
    return points;
}

void Cube::createPoints() {
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
