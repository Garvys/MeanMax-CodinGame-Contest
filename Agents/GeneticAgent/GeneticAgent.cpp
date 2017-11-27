#define EVAL_VICTORY 100000.0

double rand_double(double a, double b)
{
    return ((b - a) * ((double)rand() / RAND_MAX)) + a;
}

int rand_int(int a, int b) {
    return (rand() % (b+a+1)) + a;
}

GeneticAgent::GeneticAgent() : gammas(DEPTH) {
    for (int i = 0; i < DEPTH; i++) {
        gammas[i] = pow(GAMMA, i);
    }
}

double linear_interpolation(double x, double xa, double ya, double xb, double yb) {
    return ya + (x - xa) * (yb - ya) / (xb - xa);
}

double eval_score(int id_player, World &world, int depth) {
    double score = world.get_score(id_player);

    int _round = ROUND_COUR + depth + 1;

    if (_round == MAX_ROUND) {
        bool A = true;
        for (int p = 0; p < (NB_PLAYERS-1); p++) {
            int id_opponent = (id_player + p + 1) % NB_PLAYERS;
            A = A && (world.get_score(id_player) >= world.get_score(id_opponent));
        }
        if (A)
            return EVAL_VICTORY;
        else
            return score / WIN_SCORE;
    }
    else {
        if (score >= WIN_SCORE)
            return EVAL_VICTORY;
        else
            return score / WIN_SCORE;
    }
}

double eval_distance_to_water(int id_player, World &world) {
    double eval = 0.0;
    double sum_water_in_zone = 0.0;
    double sum_water_all_wreck = 0.0;
    Looter* reaper = world.players[id_player]->get_reaper();
    for (int i = 0; i < world.wrecks.size(); i++) {
        Wreck* wreck_a = world.wrecks[i];
        sum_water_in_zone = wreck_a->water;
        sum_water_all_wreck += wreck_a->water;
        for (int j = 0; j < world.wrecks.size(); j++) {
            if (i == j)
                continue;
            Wreck* wreck_b = world.wrecks[j];
            if (wreck_a->distance(wreck_b) < EVAL_RADIUS)
                sum_water_in_zone += wreck_b->water;
        }

        eval = max(
            eval, 
            sum_water_in_zone * (1.0 - wreck_a->distance(reaper) / (2.0 * MAP_RADIUS))
        );
    }
    eval = eval / (1.0 + sum_water_all_wreck);
    return eval;
}

double eval_distance_to_tankers(int id_player, World &world) {
    double eval = 0.0;
    double sum_water_in_zone = 0.0;
    double sum_water_all_tank = 0.0;
    Looter* destroyer = world.players[id_player]->get_destroyer();
    for (int i = 0; i < world.tankers.size(); i++) {
        Tanker* tanker_a = world.tankers[i];
        if (WATERTOWN->distance(tanker_a) >= (MAP_RADIUS - 500))
            continue;
        sum_water_all_tank += tanker_a->water;
        sum_water_in_zone = tanker_a->water;
        for (int j = 0; j < world.tankers.size(); j++) {
            if (i == j)
                continue;
            Tanker* tanker_b = world.tankers[j];
            if (WATERTOWN->distance(tanker_b) >= MAP_RADIUS)
                continue;
            if (tanker_a->distance(tanker_b) < EVAL_RADIUS) {
                sum_water_in_zone += tanker_b->water;
            }
        }

        eval = max(
            eval, 
            sum_water_in_zone * (1.0 - (tanker_a->distance(destroyer) / (2.0 * MAP_RADIUS)))
        );
    }
    eval = eval / (1.0 + sum_water_all_tank);
    return eval;
}

double eval_distance_reaper_destroyer(int id_player, World &world) {
    Looter* reaper = world.players[id_player]->get_reaper();
    Looter* destroyer = world.players[id_player]->get_destroyer();
    return 1.0 - ((reaper->distance(destroyer) / (2.0 * MAP_RADIUS)));
}

double eval_rage(int id_player, World &world) {
    return ((double)world.players[id_player]->rage) / MAX_RAGE;
}

double GeneticAgent::eval_chromosome(WorldInitializer world_initializer, vector<double> &chromosome, vector<Action*> &actions) {
    World world(world_initializer);
    actions.resize(DEPTH*LOOTER_COUNT);

    double pred_scores[NB_PLAYERS], N_water_won[NB_PLAYERS], S_score[NB_PLAYERS], pred_rage[NB_PLAYERS], rage_won[NB_PLAYERS];
    for (int i = 0; i < NB_PLAYERS; i++) {
        pred_scores[i] = world.get_score(i);
        N_water_won[i] = 0.0;
        pred_rage[i] = world.players[i]->rage;
        rage_won[i] = 0.0;
    }

    int idx_chromosome = 0;
    int idx_decoded_chromosome = 0;

    double temp = 0.0;

    double eval = 0.0;
    double eval_1 = 0.0;
    double eval_2 = 0.0;
    double eval_3 = 0.0;
    double eval_4 = 0.0;
    double eval_5 = 0.0;
    double n_temp = 0.0;
    double sum_water_all_wreck = 0.0;
    double sum_water_all_tank = 0.0;
    double eval_current_round;
    int closest_opp;
    Wreck* wreck_temp;
    Tanker* tanker_temp;
    Action* action;
    Point center_map(0, 0);
    double rage;
    double dist;
    double min_dist;
    int nb_skills_used = 0;
    double rage_cost[LOOTER_COUNT] = {REAPER_SKILL_COST, DESTROYER_SKILL_COST, DOOF_SKILL_COST};

    Looter* my_reaper = world.players[0]->looters[LOOTER_REAPER];
    Looter* my_destroyer = world.players[0]->looters[LOOTER_DESTROYER];
    Looter* my_doof = world.players[0]->looters[LOOTER_DOOF];

    for(int depth = 0; depth < DEPTH; depth++) {
        world.prepare();
        rage = world.players[0]->rage;
        nb_skills_used = 0;
        for(int i = 0; i < LOOTER_COUNT; i++) {

            action = decode_gene(
                chromosome[idx_chromosome], chromosome[idx_chromosome+1],
                chromosome[idx_chromosome+2], rage >= rage_cost[i],
                i, world);
            idx_chromosome += GENE_SIZE;

            if (action->type == SKILL) {
                rage -= rage_cost[i];
                nb_skills_used++;
            }

            //cerr << "Action : " << action->to_string(world.players[0]->looters[i]->x, world.players[0]->looters[i]->y) << endl;

            world.compute_single_action(0, i, action);
            actions[idx_decoded_chromosome] = action;
            idx_decoded_chromosome++;
        }

        // Move for other players 
        for (int p = 1; p <= 2; p++) {

            // REAPER
            min_dist = DBL_MAX;
            wreck_temp = NULL;
            for (Wreck* wreck : world.wrecks) {
                dist = wreck->distance(world.players[p]->get_reaper());
                if (dist < min_dist) {
                    min_dist = dist;
                    wreck_temp = wreck;
                }
            }
            if (wreck_temp != NULL) {
                double _power = 300.0;
                if (world.players[p]->get_reaper()->distance(wreck_temp) < wreck_temp->radius)
                    _power = 0.0;
                world.players[p]->get_reaper()->set_wanted_thrust(wreck_temp->x, wreck_temp->y, _power);
            }
            else {
                world.players[p]->get_reaper()->set_wanted_thrust(0, 0, 0);
            }

            // DESTROYER
            min_dist = DBL_MAX;
            tanker_temp = NULL;
            for (Tanker* tanker : world.tankers) {
                dist = tanker->distance(world.players[p]->get_reaper());
                if (dist < min_dist) {
                    min_dist = dist;
                    tanker_temp = tanker;
                }
            }
            if (tanker_temp != NULL) {
                world.players[p]->get_destroyer()->set_wanted_thrust(tanker_temp->x, tanker_temp->y, 300);
            }
            else {
                world.players[p]->get_destroyer()->set_wanted_thrust(0, 0, 0);
            }

            // DOOF
            if (abs(world.get_score(p) - world.get_score(0)) < abs(world.get_score(p) - world.get_score(3 - p)))
                closest_opp = 0;
            else
                closest_opp = 3 - p;
            world.players[p]->get_doof()->set_wanted_thrust(
                world.players[closest_opp]->get_reaper()->x, 
                world.players[closest_opp]->get_reaper()->y, 
                300
            );


        }

        world.update_game(0);

        eval_current_round = 0.0;
        for (int p = 0; p < NB_PLAYERS; p++) {
            temp = 0.0;
            temp += AA * eval_score(p, world, depth) + BB * eval_distance_to_water(p, world);
            temp += CC * eval_distance_to_tankers(p, world) + DD * eval_distance_reaper_destroyer(p, world);
            temp += EE * eval_rage(p, world);

            if (p == 0) {
                eval_current_round += temp;
            }
            else {
                eval_current_round -= temp / OPP_FACTOR;
            }
        }

        eval += eval_current_round * gammas[depth];

        if ((ROUND_COUR + depth + 1) == MAX_ROUND) {
            break;
        }
    }
    return eval;
}

vector<double> GeneticAgent::mutate_chromosome(vector<double> chromosome, double duration, vector<double> &new_chromosome, double allowed_time) {
    double norm_x_max = linear_interpolation(duration, 0.0, 1.0, allowed_time, 0.0);
    norm_x_max = max(0.0, norm_x_max);
    norm_x_max = min(1.0, norm_x_max);
    double x_min;
    double x_max;
    bool cyclic;
    for (int i = 0; i < CHROMOSOME_SIZE; i++) {
        cyclic = (i % GENE_SIZE) == 1;
        if (cyclic) {
            new_chromosome[i] = chromosome[i] + rand_double(-norm_x_max, norm_x_max);
            if (new_chromosome[i] < 0.0)
                new_chromosome[i] += 1.0;
            else if (new_chromosome[i] > 1.0)
                new_chromosome[i] -= 1.0;
        }
        else {
            x_max = min(1.0 - chromosome[i], norm_x_max);
            x_min = min(chromosome[i], norm_x_max);
            new_chromosome[i] = chromosome[i] + rand_double(-x_min, x_max);
        }
    }
    return new_chromosome;
}

int GeneticAgent::worst_chromosome(vector<double> eval_chromosomes) {
    int worst_index = 0;
    double worst_eval = DBL_MAX;
    for(int i = 0; i < POPULATION_SIZE; i++) {
        if (eval_chromosomes[i] < worst_eval) {
            worst_eval = eval_chromosomes[i];
            worst_index = i;
        }
    }
    return worst_index;
}

int GeneticAgent::best_chromosome(vector<double> eval_chromosomes) {
    int best_index = 0;
    double best_eval = -DBL_MAX;
    for(int i = 0; i < POPULATION_SIZE; i++) {
        if (eval_chromosomes[i] > best_eval) {
            best_eval = eval_chromosomes[i];
            best_index = i;
        }
    }
    return best_index;
}

vector<Action*> GeneticAgent::compute_best_actions(WorldInitializer world_initializer, double allowed_time) {

    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

    double duration = 0.0;

    vector<vector<double>> population = this->create_initial_population();
    vector<vector<Action*>> actions_chromosomes(POPULATION_SIZE);
    vector<double> eval_chromosomes(POPULATION_SIZE);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        eval_chromosomes[i] = this->eval_chromosome(world_initializer, population[i], actions_chromosomes[i]);
    }

    int nb_simus = POPULATION_SIZE * DEPTH;

    vector<double> new_chromosome(CHROMOSOME_SIZE);
    double eval_new_chromosome;
    int random_int;
    int worst_index;
    vector<Action*> actions;

    while (true) {

        random_int = rand_int(0, POPULATION_SIZE-1);

        mutate_chromosome(population[random_int], duration, new_chromosome, allowed_time);
        eval_new_chromosome = eval_chromosome(world_initializer, new_chromosome, actions);

        worst_index = worst_chromosome(eval_chromosomes);

        if (eval_new_chromosome > eval_chromosomes[worst_index]) {
            population[worst_index] = new_chromosome;
            eval_chromosomes[worst_index] = eval_new_chromosome;
            for(int i = 0; i < actions_chromosomes[worst_index].size();i++){
                delete actions_chromosomes[worst_index][i];
            }
            actions_chromosomes[worst_index] = actions;

        }

        duration = chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - start).count() * 1000.0;

        nb_simus += DEPTH;

        if (duration > allowed_time) {
            break;
        }
    }

    int best_index = best_chromosome(eval_chromosomes);

    cerr << "Best index : " << best_index << endl;

    this->pred_best_chromosome = population[best_index];

    for (int j = 0; j < actions_chromosomes.size(); j++) {
        if (j == best_index)
            continue;
        for(int i = 0; i < actions_chromosomes[j].size();i++){
            if (actions_chromosomes[j][i] != NULL)
                delete actions_chromosomes[j][i];
        }
    }

    cerr << "Best score : " << eval_chromosomes[best_index] << endl;

    cerr << "NB Simus : " << nb_simus << endl;

    return actions_chromosomes[best_index];
}

vector<vector<double>> GeneticAgent::create_initial_population() {
    vector<vector<double>> initial_population(POPULATION_SIZE);
    vector<double> chromosome(CHROMOSOME_SIZE);

    // First chromosme uses the best chromomsome from the round before

    for(int i = 0 ; i < POPULATION_SIZE; i++) {
        if ((i == 0) && (pred_best_chromosome.size() > 0)) {
            for(int j = 0; j < CHROMOSOME_SIZE; j++) {
                if (j < (GENE_SIZE * LOOTER_COUNT * (DEPTH - 1)))
                    chromosome[j] = this->pred_best_chromosome[GENE_SIZE * LOOTER_COUNT + j];
                else
                    chromosome[j] = rand_double(0.0, 1.0);
            }
        }
        else {
            for(int j = 0; j < CHROMOSOME_SIZE; j++) {
                chromosome[j] = rand_double(0.0, 1.0);
            }
        }
        initial_population[i] = chromosome;
    }
    return initial_population;
}

Action* GeneticAgent::decode_gene(double g1, double g2, double g3, bool skill_useable, int idx_looter, World &world) {

    if (idx_looter == LOOTER_REAPER)
        skill_useable = false;

    if (skill_useable && (g1 < 0.2)) {
        SkillAction* action = new SkillAction(-1.0, -1.0);
        action->alpha = linear_interpolation(g2, 0.0, 0.0, 1.0, 360.0);
        action->distance = linear_interpolation(g3, 0.0, 0.0, 1.0, REAPER_SKILL_RANGE);
        return action;
    }
    else if ((skill_useable && (g1 < 0.4)) || (!skill_useable && (g1 < 0.2))) {
        Looter* reaper = world.players[0]->get_reaper();
        Looter* destroyer = world.players[0]->get_destroyer();
        Looter* doof = world.players[0]->get_doof();

        if (idx_looter == LOOTER_REAPER) {
            // REAPER
            double min_dist = DBL_MAX;
            double dist;
            Wreck* wreck_temp = NULL;
            for (Wreck* wreck : world.wrecks) {
                double dist = wreck->distance(reaper);
                if (dist < min_dist) {
                    min_dist = dist;
                    wreck_temp = wreck;
                }
            }
            if (wreck_temp != NULL) {
                Action* action = new TargetAction(wreck_temp->x, wreck_temp->y, 300);
                action->hardcoded_move = true;
                return action;
            }
            else {
                Action* action = new TargetAction(destroyer->x, destroyer->y, 300);
                action->hardcoded_move = true;
                return action;
            }
        }
        else if (idx_looter == LOOTER_DESTROYER) {
            // DESTROYER
            double min_dist = DBL_MAX;
            double dist;
            Tanker* tanker_temp = NULL;
            for (Tanker* tanker : world.tankers) {
                dist = tanker->distance(reaper);
                if (dist < min_dist) {
                    min_dist = dist;
                    tanker_temp = tanker;
                }
            }
            if (tanker_temp != NULL) {
                Action* action = new TargetAction(tanker_temp->x, tanker_temp->y, 300.0);
                action->hardcoded_move = true;
                return action;
            }
            else {
                Action* action = new TargetAction(reaper->x, reaper->y, 300.0);
                action->hardcoded_move = true;
                return action;
            }
        }
        else {
            // DOOF
            int closest_opp;
            if (abs(world.get_score(1) - world.get_score(0)) < abs(world.get_score(2) - world.get_score(0)))
                closest_opp = 1;
            else
                closest_opp = 2;

            Looter* reaper_opp = world.players[closest_opp]->get_reaper();
            Action* action = new TargetAction(reaper_opp->x, reaper_opp->y, 300.0);
            action->hardcoded_move = true;
            return action;
        }
    }
    else {
        MoveAction* action = new MoveAction(-1.0, -1.0);
        action->alpha = linear_interpolation(g2, 0.0, 0.0, 1.0, 360.0);
        if (g3 < 0.2) {
            action->power = 0;
        }
        else if (g3 > 0.8) {
            action->power = 300;
        }
        else {
            action->power = linear_interpolation(g3, 0.2, 0.0, 0.8, 300.0);
        }
        return action;
    }
}
