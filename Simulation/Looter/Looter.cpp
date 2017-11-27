#pragma once

#include "Looter.h"

Looter::Looter(int id, int type, Player* player, double x, double y, double vx, double vy, double mass, double radius) : Unit(id, type, x, y, vx, vy, mass, radius), wantedThrustTarget(0,0) {
    this->player = player;
    this->reset();
}

SkillEffect* Looter::skill(double x, double y) {
    player->rage -= skillCost;
    return this->skill_impl(x, y);
}

void Looter::reset() {
    this->wantedThrustTargetSet = false;
}

void Looter::set_wanted_thrust(double x, double y, double power) {
    if (power < 0) {
        power = 0;
    }

    this->wantedThrustTargetSet = true;

    this->wantedThrustTarget.x = x;
    this->wantedThrustTarget.y = y;

    this->wantedThrustPower = min(power, MAX_THRUST);
}

Looter::~Looter() {
}
