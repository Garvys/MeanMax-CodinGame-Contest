World::World(WorldInitializer world_initializer) : players(NB_PLAYERS), nb_tankers_destroyed(NB_PLAYERS, 0) {

    this->world_initializer = world_initializer;

    this->players[0] = new Player(0, world_initializer.my_score, world_initializer.my_rage);
    this->players[1] = new Player(1, world_initializer.enemy_score_1, world_initializer.enemy_rage_1);
    this->players[2] = new Player(2, world_initializer.enemy_score_2, world_initializer.enemy_rage_2);

    for (int i = 0; i < world_initializer.units_initializer.size(); i++) {
        UnitInitializer u = world_initializer.units_initializer[i];

        if (u.unit_type == LOOTER_REAPER) {
            Looter* unit = new Reaper(
                u.unit_id, this->players[u.player], u.x, u.y, u.vx, u.vy, u.mass, u.radius);
            this->units.push_back(unit);
            this->looters.push_back(unit);
            this->players[u.player]->looters[LOOTER_REAPER] = unit;
        }
        else if (u.unit_type == LOOTER_DESTROYER) {
            Looter* unit = new Destroyer(
                u.unit_id, this->players[u.player], u.x, u.y, u.vx, u.vy, u.mass, u.radius);
            this->units.push_back(unit);
            this->looters.push_back(unit);
            this->players[u.player]->looters[LOOTER_DESTROYER] = unit;
        }
        else if (u.unit_type == LOOTER_DOOF) {
            Looter* unit = new Doof(
                u.unit_id, this->players[u.player], u.x, u.y, u.vx, u.vy, u.mass, u.radius);
            this->units.push_back(unit);
            this->looters.push_back(unit);
            this->players[u.player]->looters[LOOTER_DOOF] = unit;
        }
        else if (u.unit_type == TYPE_TANKER) {
            //cerr << "[EXPECTED] \tID : " << u.unit_id << " \tX : " << u.x << " \tY : " << u.y << endl;
            Tanker* unit = new Tanker(
                u.unit_id, u.extra, u.extra2, u.x, u.y, u.vx, u.vy, u.mass, u.radius);
            //cerr << "[REALITY] \tID : " << unit->id << " \tX : " << unit->x << " \tY : " << unit->y << endl;
            //cerr << endl;
            this->units.push_back(unit);
            this->tankers.push_back(unit);
        }
        else if (u.unit_type == TYPE_WRECK) {
            Wreck* unit = new Wreck(u.x, u.y, u.extra, u.radius);
            this->wrecks.push_back(unit);
        }
        else if (u.unit_type == TYPE_REAPER_SKILL_EFFECT) {
            ReaperSkillEffect* unit = new ReaperSkillEffect(u.x, u.y, u.radius, u.extra);
            skill_effects.push_back(unit);
        }
        else if (u.unit_type == TYPE_DOOF_SKILL_EFFECT) {
            DoofSkillEffect* unit = new DoofSkillEffect(u.x, u.y, u.radius, u.extra);
            skill_effects.push_back(unit);
        }
        else {
            cerr << "ERROR Parsing, unknown type : " << u.unit_type << endl;
            exit(1);
        }
    }

    memo_collision_border.resize(world_initializer.units_initializer.size(), NULL_COLLISION);
    memo_collision_units.resize(world_initializer.units_initializer.size(), memo_collision_border);

    this->adjust();

}

World::~World() {
    for(Player* player : players)
        delete player;

    for(Unit* unit : units)
        delete unit;

    for(Wreck* wreck : wrecks)
        delete wreck;

    for(SkillEffect* skill_effect : skill_effects)
        delete skill_effect;

}

void World::prepare() {
    for(Looter* looter : looters)
        looter->reset();
}

void World::adjust() {
    for(Unit* unit: this->units) {
        unit->adjust(this->skill_effects);
    }
}

// Play a collision
void World::play_collision(Collision collision) {
    if (collision.b == NULL) {
        // Bounce with border
        collision.a->bounce();
        collision.a->dirty_for_collision = true;
    } else {
        Tanker* dead = collision.dead();

        if (dead != NULL) {
            // A destroyer kill a tanker

            tankers.erase(std::remove(tankers.begin(), tankers.end(), dead), tankers.end());

            units.erase(std::remove(units.begin(), units.end(), dead), units.end());

            Wreck* wreck = dead->die();

            delete dead;

            if (collision.a->type == LOOTER_DESTROYER) {
                Looter* looter = (Looter*) collision.a;
                nb_tankers_destroyed[looter->player->id]++;
            }
            if (collision.b->type == LOOTER_DESTROYER) {
                Looter* looter = (Looter*) collision.b;
                nb_tankers_destroyed[looter->player->id]++;
            }

            if (wreck != NULL) {
                wrecks.push_back(wreck);
            }

            // delete dead;
        } else {
            // Bounce between two units
            collision.a->bounce(collision.b);
            collision.b->dirty_for_collision = true;
            collision.a->dirty_for_collision = true;
        }
    }
}

// Get the next collision for the current round
// All units are tested
Collision World::get_next_collision(bool recompute_all) {
    Collision result = NULL_COLLISION;
    Collision collision = NULL_COLLISION;

    for (int i = 0; i < units.size(); i++) {
        Unit* unit = this->units[i];

        // Test collision with map border first
        if (recompute_all || unit->dirty_for_collision) {
            collision = unit->get_collision();
            memo_collision_border[i] = collision;
        }
        else {
            collision = memo_collision_border[i];
        }

        if (collision.t < result.t) {
            result = collision;
        }

        for (int j = i + 1; j < this->units.size(); ++j) {
            if (recompute_all || unit->dirty_for_collision || this->units[j]->dirty_for_collision) {
                collision = unit->get_collision(units[j]);
                memo_collision_units[i][j] = collision;
            }
            else {
                collision = memo_collision_units[i][j];
            }

            if (collision.t < result.t) {
                result = collision;
            }
        }

        unit->dirty_for_collision = false;
    }

    return result;
}


void World::update_game(int round) {
    // Apply skill effects
    for (SkillEffect* effect : skill_effects) {
            effect->apply(units);
    }

    // Apply thrust for tankers
    for (Tanker* t : tankers) {
        t->play();
    }

    // Apply wanted thrust for looters
    for (Player* player : players) {
        for (Looter* looter : player->looters) {
            if (looter->wantedThrustTargetSet) {
                looter->thrust(&(looter->wantedThrustTarget), looter->wantedThrustPower);
            }
        }
    }

    double t = 0.0;

    // Play the round. Stop at each collisions and play it. Reapeat until t > 1.0

    Collision collision = this->get_next_collision(true);
    int count = 0;
    while ((collision.t + t) <= 1.0) {
        double delta = collision.t;

        for(Unit* unit : this->units) {
            unit->move(delta);
        }

        t += collision.t;

        play_collision(collision);
        count++;

        collision = this->get_next_collision(true);
        if (count > 100)
            break;
    }

    // No more collision. Move units until the end of the round
    double delta = 1.0 - t;

    for(Unit* unit : units){
        unit->move(delta);
    }

    vector<Tanker*> tankersToRemove;
    for(Tanker* tanker : tankers) {
            double distance = tanker->distance(WATERTOWN);
            bool full = tanker->is_full();

            if ((distance <= WATERTOWN_RADIUS) && (!full)) {
                // A non full tanker in watertown collect some water
                tanker->water += 1;
                tanker->mass += TANKER_MASS_BY_WATER;
            } else if ((distance >= TANKER_SPAWN_RADIUS + tanker->radius) && full) {
                // Remove too far away and not full tankers from the game
                tankersToRemove.push_back(tanker);
            }
    }

    for(Tanker* tanker : tankersToRemove) {
        units.erase(std::remove(units.begin(), units.end(), tanker), units.end());
        tankers.erase(std::remove(tankers.begin(), tankers.end(), tanker), tankers.end());
        delete tanker;
    } 


    // Water collection for reapers
    vector<Wreck*> alive_wrecks;
    for(Wreck* wreck : this->wrecks) {
        bool alive = wreck->harvest(players, skill_effects);
        if (alive){
            alive_wrecks.push_back(wreck);
        }
        else {
            delete wreck;
        }
    }
    this->wrecks = alive_wrecks;

    // Round values and apply friction
    this->adjust();

    // Generate rage
    if (LOOTER_COUNT >= 3) {
        for(Player* player : players) {
            player->rage = min(MAX_RAGE, player->rage + player->get_doof()->sing());
        }
    }

    // Restore masses
    for(Unit* unit : units) {
        while (unit->mass >= REAPER_SKILL_MASS_BONUS) {
            unit->mass -= REAPER_SKILL_MASS_BONUS;
        }
    }

    // Remove dead skill effects
    vector<SkillEffect*> effects_to_remove;
    for (SkillEffect* effect : skill_effects) {
        if (effect->duration <= 0) {
            effects_to_remove.push_back(effect);
        }
    }

    for(SkillEffect* skill_effect : effects_to_remove){
        skill_effects.erase(std::remove(skill_effects.begin(), skill_effects.end(), skill_effect), skill_effects.end());
        delete skill_effect;
    }
}


bool World::is_player_dead(int playerIdx) {
    return players[playerIdx]->dead;
}


int World::get_score(int playerIdx) {
    return players[playerIdx]->score;
}


bool World::is_game_over() {

    for (Player* player : players) {
        if (player->score >= WIN_SCORE) {
            return true;
        }
    }

    vector<Player*> alive;
    for (Player* player : players) {
        if (!player->dead) {
            alive.push_back(player);
        }
    }

    if (alive.size() == 1) {
        Player* survivor = alive[0];

        // If only one player is alive and he got the highest score, end the game now.
        bool end = true;
        for (Player* player : players) {
            if (player->score > survivor->score){
                end = false;
            }
        }
        return end;
    }

    // Everyone is dead. End of the game.
    return alive.empty();
}

void World::compute_single_action(int player_idx, int looter_idx, Action* action) {
    action->compute_action(player_idx, looter_idx, this);
}
