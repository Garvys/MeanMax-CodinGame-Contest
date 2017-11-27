#pragma once

#include <vector>

using namespace std;

class Unit;
class Looter;

class SkillEffect : public Point {
public:

    SkillEffect(int type, double x, double y, double radius, int duration);

    virtual ~SkillEffect(){}

    void apply(vector<Unit*> units);

    virtual void apply_impl(vector<Unit*> units) = 0;

    int type;
    double radius;
    int duration;
};
