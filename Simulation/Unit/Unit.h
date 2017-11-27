#pragma once

#include <set>

class Collision;
class SkillEffect;

class Unit : public Point {

public:

    Unit(int id, int type, double x, double y, double vx, double vy, double mass, double radius);

    void move(double t);

    double speed();

    void thrust(Point* p, int power);

    bool is_in_doof_skill(vector<SkillEffect*> skillEffects);

    void adjust(vector<SkillEffect*> skillEffects);

    // Search the next collision with the map border
    virtual Collision get_collision();

    // Search the next collision with an other unit
    Collision get_collision(Unit* u);

    // Bounce between 2 units
    void bounce(Unit* u);

    // Bounce with the map border
    void bounce();

    virtual ~Unit();

    int type;
    int id;
    double vx;
    double vy;
    double radius;
    double mass;
    double friction;
    
    bool dirty_for_collision;
};

