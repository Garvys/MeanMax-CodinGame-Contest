#pragma once

#include "Collision.h"

Collision::Collision(double t) {
    this->t = t;
    this->a = NULL;
    this->b = NULL;
}

Collision::Collision(double t, Unit* a) {
    this->t = t;
    this->a = a;
    this->b = NULL;
}

Collision::Collision(double t, Unit* a, Unit* b) {
    this->t = t;
    this->a = a;
    this->b = b;
}

Tanker* Collision::dead() {
    if ((a->type == LOOTER_DESTROYER) && (b->type == TYPE_TANKER) && (b->mass < REAPER_SKILL_MASS_BONUS)) {
        return (Tanker*) b;
    }

    if ((b->type == LOOTER_DESTROYER) && (a->type == TYPE_TANKER) && (a->mass < REAPER_SKILL_MASS_BONUS)) {
        return (Tanker*) a;
    }

    return NULL;
};