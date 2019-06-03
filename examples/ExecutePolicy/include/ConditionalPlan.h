//
// Created by redwan on 6/2/19.
//

#ifndef TASE_EXP_CONDITIONALPLAN_H
#define TASE_EXP_CONDITIONALPLAN_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
using namespace std;

struct beliefstruct{
    int x, y;
    double prob;
    friend ostream &operator<<(ostream &os, const beliefstruct &state) {
        os << "x: " << state.x << " y: " << state.y << " prob: " << state.prob;
        return os;
    }
};

inline vector<string> splitString(string &s, std::string delimiter) {
    size_t pos = 0;
    std::string token;
    vector<string>data;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        data.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    data.push_back(s);
    return data;
}
template <typename T>
inline void printVec(vector<T> data){
    for(auto &d:data )
        cout<<d.robot<<"\n";
    cout<<endl;
}

class ConditionalPlan {
public:
    ConditionalPlan(string & pi, string & b);
    void belief_decoder( string &b);
    void policy_decoder( string &pi);
    static vector<string> read_file(string &filename);
    beliefstruct robot;
    int b_indx;
    int action;

private:

    vector<beliefstruct>obstacles;

protected:
    string getFloatNum(string &data, int start, char * delimiter);

};


#endif //TASE_EXP_CONDITIONALPLAN_H
