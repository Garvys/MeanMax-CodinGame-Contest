#pragma once

Wreck::Wreck(double x, double y, int water, double radius) : Point(x, y) {
    this->radius = radius;
    this->water = water;
}

bool Wreck::harvest(vector<Player*> players, vector<SkillEffect*> skillEffects) {
    for (Player* player : players) {
        if ((this->is_in_range(player->get_reaper(), this->radius - 10)) && (!player->get_reaper()->is_in_doof_skill(skillEffects))) {
            (player->score)++;
            this->water--;
        }
    }

    return this->water > 0;
}