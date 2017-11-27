#pragma once

#include "Point.h"

static inline uint64_t doubleToRawBits(double x) {
    uint64_t bits;
    memcpy(&bits, &x, sizeof bits);
    return bits;
}

namespace std {

    template <>
    struct hash<Point>
    {
        size_t operator()(const Point& k) const
        {
            int prime = 31;
            int result = 1;
            long temp;
            temp = doubleToRawBits(k.x);
            result = prime * result + (int) (temp ^ (temp >> 32));
            temp = doubleToRawBits(k.y);
            result = prime * result + (int) (temp ^ (temp >> 32));
            return result;
        }
    };

}

bool operator==(const Point &lhs, const Point & rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator!=(const Point &lhs, const Point & rhs) {
    return !(lhs == rhs);
}

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

double Point::distance(Point* p) {
    return sqrt((this->x - p->x) * (this->x - p->x) + (this->y - p->y) * (this->y - p->y));
}

void Point::move(double x, double y) {
    this->x = x;
    this->y = y;
}

void Point::move_to(Point* p, double distance) {
    double d = this->distance(p);

    if (d < EPSILON) {
        return;
    }

    double dx = p->x - x;
    double dy = p->y - y;
    double coef = distance / d;

    this->x += dx * coef;
    this->y += dy * coef;
}

bool Point::is_in_range(Point* p, double range) {
    return (*p != *this) && (this->distance(p) <= range);
}