#pragma once

using namespace std;

class Doof : public Looter {
public:
    Doof(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius);

    SkillEffect* skill_impl(double x, double y);

    // With flame effects! Yeah!
    int sing();
};
