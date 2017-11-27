SkillEffect::SkillEffect(int type, double x, double y, double radius, int duration) : Point(x,y) {

    this->type = type;
    this->radius = radius;
    this->duration = duration;
}

void SkillEffect::apply(vector<Unit*> units) {
    this->duration -= 1;
    vector<Unit*> units_to_apply;
    for (Unit* unit: units) {
        if (this->is_in_range(unit, this->radius + unit->radius)) {
            units_to_apply.push_back(unit);
        }
    }
    this->apply_impl(units_to_apply);
}
