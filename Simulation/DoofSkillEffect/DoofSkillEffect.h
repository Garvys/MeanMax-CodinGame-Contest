#pragma once

using namespace std;

class DoofSkillEffect : public SkillEffect {
public:
    DoofSkillEffect(double x, double y, double radius, int duration);

    void apply_impl(vector<Unit*> units);
};
