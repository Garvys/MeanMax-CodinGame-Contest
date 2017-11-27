#pragma once

using namespace std;

class Unit;

class DestroyerSkillEffect : public SkillEffect {
public:
    DestroyerSkillEffect(double x, double y, double radius, int duration);

    void apply_impl(vector<Unit*> units);
};
