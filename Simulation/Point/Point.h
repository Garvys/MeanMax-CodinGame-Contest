#pragma once

#include <cmath>
#include <stdint.h>

using namespace std;

class Point {

public:

    Point(double x, double y);

    double distance(Point* p);

    // Move the point to x and y
    void move(double x, double y);

    // Move the point to an other point for a given distance
    void move_to(Point* p, double distance);

    bool is_in_range(Point* p, double range);

    double x;
    double y;
};
