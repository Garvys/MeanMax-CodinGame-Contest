#pragma once

#include "Unit.h"

Unit::Unit(int id, int type, double x, double y, double vx, double vy, double mass, double radius) : Point(x, y) {

    this->type = type;
    this->id = id;

    this->vx = vx;
    this->vy = vy;

    this->mass = mass;
    this->radius = radius;

    this->dirty_for_collision = true;
}

void Unit::move(double t) {
    // cerr << "T : " << t << endl;
    this->x += this->vx * t;
    this->y += this->vy * t;
}

double Unit::speed() {
    return sqrt(this->vx * this->vx + this->vy * this->vy);
}

bool Unit::is_in_doof_skill(vector<SkillEffect*> skillEffects) {
    for(SkillEffect* skilleffect : skillEffects) {
        if ((skilleffect->type == TYPE_DOOF_SKILL_EFFECT) && (this->is_in_range(skilleffect, skilleffect->radius + radius))) {
            return true;
        }
    }
    return false;
}

void Unit::thrust(Point* p, int power) {
    double distance = this->distance(p);

    // Avoid a division by zero
    if (fabs(distance) <= EPSILON) {
        return;
    }

    double coef = (((double) power) / this->mass) / distance;
    this->vx += (p->x - this->x) * coef;
    this->vy += (p->y - this->y) * coef;
}

void Unit::adjust(vector<SkillEffect*> skillEffects) {
    this->x = round(this->x);
    this->y = round(this->y);

    if (is_in_doof_skill(skillEffects)) {
        // No friction if we are in a doof skill effect
        this->vx = round(this->vx);
        this->vy = round(this->vy);
    } else {
        this->vx = round(this->vx * (1.0 - this->friction));
        this->vy = round(this->vy * (1.0 - this->friction));
    }
}

Collision Unit::get_collision() {
    // Check instant collision
    if (this->distance(WATERTOWN) + radius >= MAP_RADIUS) {
        return Collision(0.0, this);
    }

    // We are not moving, we can't reach the map border
    if ((this->vx == 0.0) && (this->vy == 0.0)) {
        return NULL_COLLISION;
    }

    // Search collision with map border
    // Resolving: sqrt((x + t*vx)^2 + (y + t*vy)^2) = MAP_RADIUS - radius <=> t^2*(vx^2 + vy^2) + t*2*(x*vx + y*vy) + x^2 + y^2 - (MAP_RADIUS - radius)^2 = 0
    // at^2 + bt + c = 0;
    // a = vx^2 + vy^2
    // b = 2*(x*vx + y*vy)
    // c = x^2 + y^2 - (MAP_RADIUS - radius)^2

    double a = this->vx * this->vx + this->vy * this->vy;

    if (a <= 0.0) {
        return NULL_COLLISION;
    }

    double b = 2.0 * (this->x * this->vx + this->y * this->vy);
    double c = this->x * this->x + this->y * this->y - (MAP_RADIUS - this->radius) * (MAP_RADIUS - this->radius);
    double delta = b * b - 4.0 * a * c;

    if (delta <= 0.0) {
        return NULL_COLLISION;
    }

    double t = (-b + sqrt(delta)) / (2.0 * a);

    if (t <= 0.0) {
        return NULL_COLLISION;
    }

    return Collision(t, this);
}

Collision Unit::get_collision(Unit* u) {
    // Check instant collision
    if (this->distance(u) <= (this->radius + u->radius)) {
        return Collision(0.0, this, u);
    }

    // Both units are motionless
    if ((this->vx == 0.0) && (this->vy == 0.0) && (u->vx == 0.0) && (u->vy == 0.0)) {
        return NULL_COLLISION;
    }

    // Change referencial
    // Unit u is not at point (0, 0) with a speed vector of (0, 0)
    double x2 = this->x - u->x;
    double y2 = this->y - u->y;
    double r2 = this->radius + u->radius;
    double vx2 = this->vx - u->vx;
    double vy2 = this->vy - u->vy;

    // Resolving: sqrt((x + t*vx)^2 + (y + t*vy)^2) = radius <=> t^2*(vx^2 + vy^2) + t*2*(x*vx + y*vy) + x^2 + y^2 - radius^2 = 0
    // at^2 + bt + c = 0;
    // a = vx^2 + vy^2
    // b = 2*(x*vx + y*vy)
    // c = x^2 + y^2 - radius^2 

    double a = vx2 * vx2 + vy2 * vy2;

    if (a <= 0.0) {
        return NULL_COLLISION;
    }

    double b = 2.0 * (x2 * vx2 + y2 * vy2);
    double c = x2 * x2 + y2 * y2 - r2 * r2;
    double delta = b * b - 4.0 * a * c;

    if (delta < 0.0) {
        return NULL_COLLISION;
    }

    double t = (-b - sqrt(delta)) / (2.0 * a);

    if (t <= 0.0) {
        return NULL_COLLISION;
    }

    return Collision(t, this, u);
}

void Unit::bounce(Unit* u) {
    //cerr << "BOUNCE " << this->id << endl;
    double mcoeff = (mass + u->mass) / (mass * u->mass);
    double nx = this->x - u->x;
    double ny = this->y - u->y;
    double nxnysquare = nx * nx + ny * ny;
    double dvx = this->vx - u->vx;
    double dvy = this->vy - u->vy;
    double product = (nx * dvx + ny * dvy) / (nxnysquare * mcoeff);
    double fx = nx * product;
    double fy = ny * product;
    double m1c = 1.0 / this->mass;
    double m2c = 1.0 / u->mass;

    this->vx -= fx * m1c;
    this->vy -= fy * m1c;
    u->vx += fx * m2c;
    u->vy += fy * m2c;

    fx = fx * IMPULSE_COEFF;
    fy = fy * IMPULSE_COEFF;

    // Normalize vector at min or max impulse
    double impulse = sqrt(fx * fx + fy * fy);
    double coeff = 1.0;
    if (impulse > EPSILON && impulse < MIN_IMPULSE) {
        coeff = MIN_IMPULSE / impulse;
    }

    fx = fx * coeff;
    fy = fy * coeff;

    this->vx -= fx * m1c;
    this->vy -= fy * m1c;
    u->vx += fx * m2c;
    u->vy += fy * m2c;

    double diff = (this->distance(u) - radius - u->radius) / 2.0;
    if (diff <= 0.0) {
        // Unit overlapping. Fix positions.
        this->move_to(u, diff - EPSILON);
        u->move_to(this, diff - EPSILON);
    }
}

void Unit::bounce() {
    double mcoeff = 1.0 / this->mass;
    double nxnysquare = this->x * this->x + this->y * this->y;
    double product = (this->x * this->vx + this->y * this->vy) / (nxnysquare * mcoeff);
    double fx = this->x * product;
    double fy = this->y * product;

    this->vx -= fx * mcoeff;
    this->vy -= fy * mcoeff;

    fx = fx * IMPULSE_COEFF;
    fy = fy * IMPULSE_COEFF;

    // Normalize vector at min or max impulse
    double impulse = sqrt(fx * fx + fy * fy);
    double coeff = 1.0;
    if ((impulse > EPSILON) && (impulse < MIN_IMPULSE)) {
        coeff = MIN_IMPULSE / impulse;
    }

    fx = fx * coeff;
    fy = fy * coeff;
    this->vx -= fx * mcoeff;
    this->vy -= fy * mcoeff;

    double diff = this->distance(WATERTOWN) + this->radius - MAP_RADIUS;
    if (diff >= 0.0) {
        // Unit still outside of the map, reposition it
        this->move_to(WATERTOWN, diff + EPSILON);
    }
}

Unit::~Unit() {

}