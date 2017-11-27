#pragma once

class Unit;
class Tanker;

class Collision {
public:

    Collision(double t);

    Collision(double t, Unit* a);

    Collision(double t, Unit* a, Unit* b);

    Tanker* dead();

    double t;
    Unit* a;
    Unit* b;
};
