#pragma once

using namespace std;

enum ActionType;
class SkillEffect;
class Player;

class Looter : public Unit {
public:
    Looter(int id, int type, Player* player, double x, double y, double vx, double vy, double mass, double radius);

    SkillEffect* skill(double x, double y);

    virtual SkillEffect* skill_impl(double x, double y) = 0;

    void set_wanted_thrust(double x, double y, double power);

    void reset();

    ~Looter();


    int skillCost;
    double skillRange;
    bool skillActive;

    Player* player;

    Point wantedThrustTarget;
    double wantedThrustPower;
    bool wantedThrustTargetSet;
};
