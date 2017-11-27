Player::Player(int id, int score, int rage) : looters(LOOTER_COUNT) {
    this->id = id;
    this->score = score;
    this->rage = rage;
    this->dead = false;
}

Player::~Player() {

}

void Player::kill() {
    this->dead = true;
}

Reaper* Player::get_reaper() {
    return (Reaper*) this->looters[LOOTER_REAPER];
}

Destroyer* Player::get_destroyer() {
    return (Destroyer*) this->looters[LOOTER_DESTROYER];
}

Doof* Player::get_doof() {
    return (Doof*) this->looters[LOOTER_DOOF];
}