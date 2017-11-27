#pragma GCC optimize("-O3")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unroll-loops")

#include <vector>
#include <set>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <random>
#include <float.h>
#include <ratio>
#include <chrono>

using namespace std;

#define SINGLE_RUN 1
#define PRINT_INPUTS 0
#define DEBUG_EVAL 0
#define TIME_LIMIT_FIRST_TURN 970
#define TIME_LIMIT_NORMAL_TURN 44

#define DEPTH 3
#define GAMMA 0.9
#define POPULATION_SIZE 10
#define GENE_SIZE 3
#define CHROMOSOME_SIZE (GENE_SIZE * LOOTER_COUNT * DEPTH)

#define AA 9.21740429043578
#define BB 0.650157344807243
#define CC 1.0000
#define DD 0.5599963496062185
#define EE 0.7720695048204423

#define OPP_FACTOR 2.0

int ROUND_COUR;

#define EVAL_RADIUS 1500

#include "Simulation/constants.cpp"
#include "Simulation/Action/Action.h"
#include "Simulation/Point/Point.h"
#include "Simulation/Collision/Collision.h"
#include "Simulation/Unit/Unit.h"
#include "Simulation/Wreck/Wreck.h"
#include "Simulation/Tanker/Tanker.h"
#include "Simulation/Looter/Looter.h"
#include "Simulation/Reaper/Reaper.h"
#include "Simulation/Destroyer/Destroyer.h"
#include "Simulation/Doof/Doof.h"
#include "Simulation/Player/Player.h"
#include "Simulation/TankerSpawn/TankerSpawn.h"
#include "Simulation/SkillEffect/SkillEffect.h"
#include "Simulation/ReaperSkillEffect/ReaperSkillEffect.h"
#include "Simulation/DestroyerSkillEffect/DestroyerSkillEffect.h"
#include "Simulation/DoofSkillEffect/DoofSkillEffect.h"
#include "Simulation/WorldInitializer/WorldInitializer.h"
#include "Simulation/World/World.h"
#include "Agents/Agent/Agent.h"
#include "Agents/GeneticAgent/GeneticAgent.h"

// Center of the map
Point* WATERTOWN = new Point(0, 0);

// The null collision 
Collision NULL_COLLISION = Collision(1.0 + EPSILON);

Action* WAIT_ACTION = new WaitAction();

#include "Simulation/Point/Point.cpp"
#include "Simulation/Collision/Collision.cpp"
#include "Simulation/Unit/Unit.cpp"
#include "Simulation/Wreck/Wreck.cpp"
#include "Simulation/Tanker/Tanker.cpp"
#include "Simulation/Looter/Looter.cpp"
#include "Simulation/Reaper/Reaper.cpp"
#include "Simulation/Destroyer/Destroyer.cpp"
#include "Simulation/Doof/Doof.cpp"
#include "Simulation/Player/Player.cpp"
#include "Simulation/TankerSpawn/TankerSpawn.cpp"
#include "Simulation/SkillEffect/SkillEffect.cpp"
#include "Simulation/ReaperSkillEffect/ReaperSkillEffect.cpp"
#include "Simulation/DestroyerSkillEffect/DestroyerSkillEffect.cpp"
#include "Simulation/DoofSkillEffect/DoofSkillEffect.cpp"
#include "Simulation/WorldInitializer/WorldInitializer.cpp"
#include "Simulation/World/World.cpp"
#include "Simulation/Action/Action.cpp"
#include "Agents/Agent/Agent.cpp"
#include "Agents/GeneticAgent/GeneticAgent.cpp"


using namespace std;


int main()
{
    srand(2010);
    Agent* agent = new GeneticAgent();
    ROUND_COUR = 1;

    // game loop
    while (1) {
        WorldInitializer world_initializer;
        world_initializer.parse_inputs();

        std::clock_t start;

        double duration;

        start = std::clock();

        vector<Action*> best_actions;

        if (ROUND_COUR == 1)
            best_actions = agent->compute_best_actions(world_initializer, TIME_LIMIT_FIRST_TURN);
        else
            best_actions = agent->compute_best_actions(world_initializer, TIME_LIMIT_NORMAL_TURN);

        duration = (( std::clock() - start ) / (double) CLOCKS_PER_SEC) * 1000.0;
        cerr << "Time : " << duration << " ms" << endl;

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        World world(world_initializer);

        for(int i = 0; i < LOOTER_COUNT; i++) {
            cout << best_actions[i]->to_string(world.players[0]->looters[i]->x, world.players[0]->looters[i]->y) << endl;
        }

        for(int i = 0; i < best_actions.size();i++){
            if (best_actions[i] != NULL)
                delete best_actions[i];
        }

        ROUND_COUR++;

        #if SINGLE_RUN
            break;
        #endif
    }

    // delete WATERTOWN;
    // delete WAIT_ACTION;
    // delete agent;
}