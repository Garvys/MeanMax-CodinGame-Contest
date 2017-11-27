Destroyer::Destroyer(int id, Player* player, double x, double y, double vx, double vy, double mass, double radius) :
    Looter(id, LOOTER_DESTROYER, player, x, y, vx, vy, mass, radius) {
    friction = DESTROYER_FRICTION;
    skillCost = DESTROYER_SKILL_COST;
    skillRange = DESTROYER_SKILL_RANGE;
    skillActive = DESTROYER_SKILL_ACTIVE;
}

SkillEffect* Destroyer::skill_impl(double x, double y) {
    return new DestroyerSkillEffect(x, y, DESTROYER_SKILL_RADIUS, DESTROYER_SKILL_DURATION);
}