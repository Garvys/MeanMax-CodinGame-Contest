#pragma once

class Player;
class Collision;

using namespace std;

class Tanker : public Unit {
public:

    Tanker(int id, int water, int size, double x, double y, double vx, double vy, double mass, double radius);

    Wreck* die();

    bool is_full();

    void play();

    Collision get_collision();

    int water;
    int size;
};
