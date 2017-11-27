ReaperSkillEffect::ReaperSkillEffect(double x, double y, double radius, int duration) : 
    SkillEffect(TYPE_REAPER_SKILL_EFFECT, x, y, radius, duration) {
}

void ReaperSkillEffect::apply_impl(vector<Unit*> units) {
    // Increase mass
    for(Unit* unit : units) {
            unit->mass += REAPER_SKILL_MASS_BONUS;
    }
}