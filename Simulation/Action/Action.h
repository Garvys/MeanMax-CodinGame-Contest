#pragma once

using namespace std;

class Point;

class World;

enum ActionType {
    SKILL, MOVE, WAIT, NONE
};


class Action {
public:
    Action(ActionType type);
    virtual string to_string(double x_cour, double y_cour) = 0;
    virtual void compute_action(int player_idx, int looter_idx, World* world) = 0;

    ActionType type;

    bool hardcoded_move = false;

    virtual ~Action(){}
};


class WaitAction : public Action {
public:
    WaitAction();
    string to_string(double x_cour, double y_cour);
    void compute_action(int player_idx, int looter_idx, World* world);
    ~WaitAction(){}
};

class MoveAction : public Action {
public:
    MoveAction(double alpha, double power);
    Point compute_target_point(double x_cour, double y_cour);
    string to_string(double x_cour, double y_cour);
    void compute_action(int player_idx, int looter_idx, World* world);
    ~MoveAction(){}

    double alpha;
    double power;
};

class TargetAction : public Action {
public:
    TargetAction(double x, double y, double power);
    string to_string(double x_cour, double y_cour);
    void compute_action(int player_idx, int looter_idx, World* world);
    ~TargetAction(){}

    double x;
    double y;
    double power;
};

class SkillAction : public Action {
public:
    SkillAction(double alpha, double distance);
    Point compute_target_point(double x_cour, double y_cour);
    string to_string(double x_cour, double y_cour);
    void compute_action(int player_idx, int looter_idx, World* world);
    ~SkillAction(){}

    double alpha;
    double distance;
};
