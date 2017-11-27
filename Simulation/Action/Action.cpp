#define DISTANCE_ACTION 10000
#define PI 3.14159265

#define STRING_HARDCODED " PLOP"

Action::Action(ActionType type) {
    this->type = type;
    this->hardcoded_move = false;
}

WaitAction::WaitAction() : Action(WAIT) {

}

string WaitAction::to_string(double x_cour, double y_cour) {
    string s = "WAIT";
    if (this->hardcoded_move)
        s += STRING_HARDCODED;
    return s;
}

void WaitAction::compute_action(int player_idx, int looter_idx, World* world) {
    // world->players[player_idx]->looters[looter_idx]->attempt = this->type;
}

MoveAction::MoveAction(double alpha, double power) : Action(MOVE) {
    this->alpha = alpha;
    this->power = power;
}

Point MoveAction::compute_target_point(double x_cour, double y_cour) {
    double new_x = x_cour + DISTANCE_ACTION * cos(this->alpha * PI / 180.0);
    double new_y = y_cour + DISTANCE_ACTION * sin(this->alpha * PI / 180.0);
    return Point(new_x, new_y);
}

string MoveAction::to_string(double x_cour, double y_cour) {
    Point pos_action = compute_target_point(x_cour, y_cour);
    string s = std::to_string(int(round(pos_action.x))) + " " + \
           std::to_string(int(round(pos_action.y))) + " " + \
           std::to_string(int(round(this->power)));
    if (this->hardcoded_move)
        s += STRING_HARDCODED;
    return s;
}

void MoveAction::compute_action(int player_idx, int looter_idx, World* world) {
    Looter* looter = world->players[player_idx]->looters[looter_idx];
    // looter->attempt = this->type;
    Point pos_action = compute_target_point(looter->x, looter->y);
    looter->set_wanted_thrust(pos_action.x, pos_action.y, power);
}

TargetAction::TargetAction(double x, double y, double power) : Action(MOVE) {
    this->x = x;
    this->y = y;
    this->power = power;
}

string TargetAction::to_string(double x_cour, double y_cour) {
    string s = std::to_string(int(round(this->x))) + " " + \
           std::to_string(int(round(this->y))) + " " + \
           std::to_string(int(round(this->power)));
    if (this->hardcoded_move)
        s += STRING_HARDCODED;
    return s;
}

void TargetAction::compute_action(int player_idx, int looter_idx, World* world) {
    Looter* looter = world->players[player_idx]->looters[looter_idx];
    looter->set_wanted_thrust(this->x, this->y, this->power);
}

SkillAction::SkillAction(double alpha, double distance) : Action(SKILL) {
    this->alpha = alpha;
    this->distance = distance;
}

Point SkillAction::compute_target_point(double x_cour, double y_cour) {
    double new_x = x_cour + this->distance * cos(this->alpha * PI / 180.0);
    double new_y = y_cour + this->distance * sin(this->alpha * PI / 180.0);
    return Point(new_x, new_y);
}

string SkillAction::to_string(double x_cour, double y_cour) {
    Point pos_action = compute_target_point(x_cour, y_cour);
    string s = "SKILL " + std::to_string(int(round(pos_action.x))) + " " + std::to_string(int(round(pos_action.y))) + " BOOM";
    if (this->hardcoded_move)
        s += STRING_HARDCODED;
    return s;
}

void SkillAction::compute_action(int player_idx, int looter_idx, World* world) {
    Looter* looter = world->players[player_idx]->looters[looter_idx];
    // looter->attempt = this->type;
    Point pos_action = compute_target_point(looter->x, looter->y);
    looter->set_wanted_thrust(pos_action.x, pos_action.y, 0);
    SkillEffect* effect = looter->skill(pos_action.x, pos_action.y);
    world->skill_effects.push_back(effect);
}
