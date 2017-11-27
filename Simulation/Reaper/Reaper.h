#pragma once

using namespace std;

class Player;
class SkillEffect;

class Reaper : public Looter {
public:
    Reaper(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius);

    SkillEffect* skill_impl(double x, double y);
};
