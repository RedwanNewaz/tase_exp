//
// Created by redwan on 5/10/19.
//

#ifndef TASE_EXP_POMDPPOLICY_H
#define TASE_EXP_POMDPPOLICY_H
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <sstream>
#include <thread>
#include "TrajPlanner.h"




using namespace std;
class PomdpPolicy {

public:
    PomdpPolicy(string & pi, string & b);
    struct State{
        int x, y;
        double prob;
        friend ostream &operator<<(ostream &os, const State &state) {
            os << "x: " << state.x << " y: " << state.y << " prob: " << state.prob;
            return os;
        }
    };
    void belief_decoder( string &b);
    void policy_decoder( string &pi);
    bool isTerminal();
    static vector<string> read_file(string &filename);
    static void append( PomdpPolicy** head_ref, string & pi, string &b);
    static void printPolicy(PomdpPolicy *n);
    void printRobotState();
    QPoint getState();
    PomdpPolicy *next;
    vector<pair<QPoint,float>> getObstacle();
    int action_, b_indx_;
private:
    State robot_;
    vector<State>obstacles_;

    float observation_;
    enum {
        move_north_idx = 1,
        move_south_idx = 2,
        move_west_idx = 3,
        move_east_idx = 4,
        look_north_idx = 5,
        look_south_idx = 6,
        look_west_idx = 7,
        look_east_idx = 8,
        pick_up_left_hand_idx = 9,
        pick_up_right_hand_idx = 10,
    };

protected:
    void add_action(int a);
    void add_observation(float z);
    string getFloatNum(string &data, int start, char * delimiter);
    vector<string>splitString(string &s, std::string delimiter);

};

class PolicyExecution:public TrajPlanner{
public:
    PolicyExecution(MotionPrimitives *move);
    bool generate() override;
    void obstacleVisualization(PomdpPolicy *pi);
    void run() override;

private:
    ros::NodeHandle nh;
    string policy_file, belief_file, test_file;
    vector<QPoint> ground_truth;
    PomdpPolicy *pi;
    enum ACT_TYPE{
        MOVE,
        LOOK,
        PICK
    };

protected:
    void read_test_file();
    ACT_TYPE action_type(int a);

};


#endif //TASE_EXP_POMDPPOLICY_H
