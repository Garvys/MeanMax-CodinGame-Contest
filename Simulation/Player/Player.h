#pragma once

#include <vector>
#include <queue>

using namespace std;

class Looper;
class Reaper;
class Destroyer;
class Doof;
class TankerSpawn;

class Player {
public:

    Player(int id, int score, int rage);
    ~Player();

    void kill();

    Reaper* get_reaper();

    Destroyer* get_destroyer();

    Doof* get_doof();

    int score;
    int id;
    int rage;
    vector<Looter*> looters;
    bool dead;
    queue<TankerSpawn*> tankers;
};
