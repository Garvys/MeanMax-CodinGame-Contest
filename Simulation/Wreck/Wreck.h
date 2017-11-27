#pragma once

using namespace std;

class Player;
class SkillEffect;

class Wreck: public Point {

public:

    Wreck(double x, double y, int water, double radius);

    // Reaper harvesting
    bool harvest(vector<Player*> players, vector<SkillEffect*> skillEffects);

    int id;
    double radius;
    int water;
};
