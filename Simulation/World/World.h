#pragma once

class WorldInitializer;

class World {
public:
    World(WorldInitializer world_initializer);

    void adjust();

    void prepare();

    void update_game(int round);

    Collision get_next_collision(bool recompute_all);

    void play_collision(Collision collision);

    bool is_player_dead(int playerIdx);

    int get_score(int playerIdx);

    bool is_game_over();

    void compute_single_action(int player_idx, int looter_idx, Action* action);

    void reset();

    Wreck* get_allocated_wreck();

    ~World();

    vector<Player*> players;
    vector<Unit*> units;
    vector<Looter*> looters;
    vector<Tanker*> tankers;
    vector<Wreck*> wrecks;
    vector<SkillEffect*> skill_effects;

    // Evaluation
    vector<int> nb_tankers_destroyed;
    WorldInitializer world_initializer;
    vector<Collision> memo_collision_border;
    vector<vector<Collision>> memo_collision_units;

};