//
// Created by redwan on 6/3/19.
//

#ifndef TASE_EXP_BELIEFSTATE_H
#define TASE_EXP_BELIEFSTATE_H

#include <iostream>
#include <tuple>
using namespace std;

class BeliefState {
public:

    int x,y;
    float prob;
    friend ostream &operator<<(ostream &os, const BeliefState &beliefstruct1){
        os << "(x: " << beliefstruct1.x << " y: " << beliefstruct1.y << ")| prob: " << beliefstruct1.prob<<"\t";
        return os;
    }
    BeliefState();
    void set(int x, int y , float prob = 1.0);

    BeliefState(int x, int y , float prob = 1.0);

    int getIndex(int x_size);
    std::tuple<int,int>getSub(int index, int x_size);

    double operator -(const BeliefState &rhs) const;

    bool operator==(const BeliefState &rhs) const;

    bool operator<(const BeliefState &rhs) const ;

    bool operator>(const BeliefState &rhs) const ;

    bool operator<=(const BeliefState &rhs) const ;

    bool operator>=(const BeliefState &rhs) const ;

    BeliefState operator +(int action);
    
};


#endif //TASE_EXP_BELIEFSTATE_H
