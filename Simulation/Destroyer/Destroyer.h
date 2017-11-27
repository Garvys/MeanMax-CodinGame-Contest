#pragma once

using namespace std;

class Destroyer : public Looter {
public:
    Destroyer(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius);

    SkillEffect* skill_impl(double x, double y);
};
