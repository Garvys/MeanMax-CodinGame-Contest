#pragma once

#include "Reaper.h"

Reaper::Reaper(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius) :
    Looter(id, LOOTER_REAPER, player, x, y, vx, vy, mass, radius) {
    friction = REAPER_FRICTION;
    skillCost = REAPER_SKILL_COST;
    skillRange = REAPER_SKILL_RANGE;
    skillActive = REAPER_SKILL_ACTIVE;
}

SkillEffect* Reaper::skill_impl(double x, double y) {
    return new ReaperSkillEffect(x, y, REAPER_SKILL_RADIUS, REAPER_SKILL_DURATION);
}
