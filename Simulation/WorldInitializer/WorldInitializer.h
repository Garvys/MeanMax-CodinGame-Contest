#pragma once

class UnitInitializer {
public:
    int unit_id;
    int unit_type;
    int player;
    float mass;
    int radius;
    int x;
    int y;
    int vx;
    int vy;
    int extra;
    int extra2;
};

class WorldInitializer {
public:
    WorldInitializer();

    void parse_inputs();

    int my_score;
    int enemy_score_1;
    int enemy_score_2;
    int my_rage;
    int enemy_rage_1;
    int enemy_rage_2;
    vector<UnitInitializer> units_initializer;
};