#pragma once

using namespace std;

class ReaperSkillEffect : public SkillEffect {
public:
    ReaperSkillEffect(double x, double y, double radius, int duration);

    void apply_impl(vector<Unit*> units);
};
