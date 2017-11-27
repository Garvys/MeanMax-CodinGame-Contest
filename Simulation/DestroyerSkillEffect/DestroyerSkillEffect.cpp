DestroyerSkillEffect::DestroyerSkillEffect(double x, double y, double radius, int duration) :
    SkillEffect(TYPE_DESTROYER_SKILL_EFFECT, x, y, radius, duration) {
}

void DestroyerSkillEffect::apply_impl(vector<Unit*> units) {
    // Push units
    for(Unit* unit : units) {
            unit->thrust(this, -DESTROYER_NITRO_GRENADE_POWER);
    }
}
