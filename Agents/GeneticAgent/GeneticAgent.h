
class GeneticAgent : public Agent {
public:
    GeneticAgent();
    vector<Action*> compute_best_actions(WorldInitializer world_initializer, double allowed_time);
    vector<Action*> decode_individual(vector<double>);
    double eval_chromosome(WorldInitializer world_initializer, vector<double> &chromosome, vector<Action*> &actions);
    vector<double> mutate_chromosome(vector<double> chromosome, double duration, vector<double> &new_chromosome, double allowed_time);
    int worst_chromosome(vector<double> eval_chromosomes);
    int best_chromosome(vector<double> eval_chromosomes);
    vector<vector<double>> create_initial_population();
    Action* decode_gene(double g1, double g2, double g3, bool skill_useable, int i, World &world);
    vector<Action*> decode_chromosome(vector<double> &chromosome);

    vector<double> gammas;
    vector<double> pred_best_chromosome;
};