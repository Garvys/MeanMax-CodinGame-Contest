
class Agent {
public:
    virtual vector<Action*> compute_best_actions(WorldInitializer world_initializer, double allowed_time) = 0;
};
