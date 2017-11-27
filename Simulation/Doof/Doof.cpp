Doof::Doof(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius) :
    Looter(id, LOOTER_DOOF, player, x, y, vx, vy, mass, radius) {
    friction = DOOF_FRICTION;
    skillCost = DOOF_SKILL_COST;
    skillRange = DOOF_SKILL_RANGE;
    skillActive = DOOF_SKILL_ACTIVE;
}

SkillEffect* Doof::skill_impl(double x, double y) {
    return new DoofSkillEffect(x, y, DOOF_SKILL_RADIUS, DOOF_SKILL_DURATION);
}

// With flame effects! Yeah!
int Doof::sing() {
    return (int) floor(speed() * DOOF_RAGE_COEF);
}