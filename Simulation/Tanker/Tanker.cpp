#pragma once

#include "Tanker.h"

Tanker::Tanker(int id, int water, int size, double x, double y, double vx, double vy, double mass, double radius) :
    Unit(id, TYPE_TANKER, x, y, vx, vy, mass, radius) {
        
    this->size = size;
    this->water = water;

    friction = TANKER_FRICTION;
}

// TODO : Do we need a new ID ??
Wreck* Tanker::die() {
    // Don't spawn a wreck if our center is outside of the map
    if (this->distance(WATERTOWN) >= MAP_RADIUS) {
        return NULL;
    }

    return new Wreck(round(this->x), round(this->y), this->water, this->radius);
    // reset_wreck(erased_wreck, round(this->x), round(this->y), this->water, this->radius);
}

bool Tanker::is_full() {
    return water >= size;
}

void Tanker::play() {
    if (this->is_full()) {
        // Try to leave the map
        this->thrust(WATERTOWN, -TANKER_THRUST);
    } else if (this->distance(WATERTOWN) > WATERTOWN_RADIUS) {
        // Try to reach watertown
        this->thrust(WATERTOWN, TANKER_THRUST);
    }
}

Collision Tanker::get_collision() {
    // Tankers can go outside of the map
    return NULL_COLLISION;
}