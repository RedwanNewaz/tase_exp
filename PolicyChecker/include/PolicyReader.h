//
// Created by Redwan Newaz on 2019-05-31.
//

#ifndef POLICYCHECKER_POLICYREADER_H
#define POLICYCHECKER_POLICYREADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <sstream>
#include <iterator>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cassert>
using namespace std;

class ConditionalPlan;

struct beliefstruct{
    int x,y;
    float prob;
    friend ostream &operator<<(ostream &os, const beliefstruct &beliefstruct1){
        os << "(x: " << beliefstruct1.x << " y: " << beliefstruct1.y << ")| prob: " << beliefstruct1.prob<<"\t";
        return os;
    }

    int getIndex(int x_size){
        return y*x_size+x;
    }
    std::tuple<int,int>getSub(int index, int x_size){
        return make_tuple( index%x_size, index/x_size);
    }

    double operator -(const beliefstruct &rhs) const
    {
        return abs(x-rhs.x)+abs(y-rhs.y);
//        return sqrt(pow(x-rhs.x,2)+pow(y-rhs.y,2));

    }

    bool operator==(const beliefstruct &rhs) const
    {
        return x==rhs.x && y==rhs.y;
    }

    bool operator<(const beliefstruct &rhs) const {

        bool distE = rhs.y>=y && rhs.x>=x;
        auto dist = abs(x-rhs.x)+abs(y-rhs.y);

        if (dist <= 1 && distE )
            return true;
        else
            return false;

    }

    bool operator>(const beliefstruct &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const beliefstruct &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const beliefstruct &rhs) const {
        return !(*this < rhs);
    }

    beliefstruct operator +(int action)
    {
        switch (action)
        {
            case 1:
            {
                //move north
                int xx=x, yy=y+1;
                return beliefstruct{xx,yy,prob};
            }
            case 2:
            {
                //move south
                int xx=x, yy=y-1;
                return beliefstruct{xx,yy,prob};
            }
            case 3:
            {
                //move west
                int xx=x-1, yy=y;
                return beliefstruct{xx,yy,prob};;
            }
            case 4:
            {
                //move east
                int xx=x+1, yy=y;
                return beliefstruct{xx,yy,prob};
            }

            default:
                return beliefstruct{x,y,prob};
        }
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
        cout<<d<<"\t";
    cout<<endl;
}

typedef std::shared_ptr<ConditionalPlan> CPtrs;

class ConditionalPlan: public enable_shared_from_this<ConditionalPlan>{
public:
    ConditionalPlan(vector<string>&b, vector<string>&pi);
    CPtrs getPtr();
    beliefstruct robot;
    vector<beliefstruct>obstacles;
    int b_indx;
    int action;

    friend ostream &operator<<(ostream &os, const ConditionalPlan &plan);

protected:
    beliefstruct m_getRobotBelief(string &b);
    vector<beliefstruct> m_getObstacleBelief(string &b);

};


typedef unordered_map<int, vector<CPtrs>> PolicyTree;
static int x_size =3;
class PolicyReader {
    typedef pair<beliefstruct, vector<int>> nodePair;
    typedef unordered_map<int,nodePair> Gg;
public:
    PolicyReader(const string &__dirname);
    void readFiles();
    PolicyTree Policy;
    vector<beliefstruct>GroundTruth;

    Gg createGraph()
    {
        Gg Nodes;
        for(auto &pi:Policy)
        {
//            cout<<endl<<pi.first<<endl;
            for(auto &belief:pi.second)
            {
                int index =belief->robot.getIndex(x_size);
                int action =belief->action;
                // check node is listed or not
                if(Nodes.find(index) != Nodes.end())
                {
                    nodePair dataPair = Nodes[index];
                    auto data = dataPair.second;
                    // check action is in the list or not
                    if(!binary_search(data.begin(),data.end(),action))
                    {
                        data.push_back(action);
                        Nodes[index]=make_pair(belief->robot,data);
                    }
                }
                else
                {
                    vector<int> data{action};
                    Nodes[index] = make_pair(belief->robot,data);
                }
            }

        }
        return Nodes;
    }



    friend ostream &operator<<(ostream &os, const PolicyReader &reader);


private:
    string __dirname;
    string __policyFile, __beliefFile, __testFile;
    vector<string> BELIEF_STR,POLICY_STR;

protected:
    void m_beliefDecoder(string &s);
    void m_policyDecoder(string &s);

};


#endif //POLICYCHECKER_POLICYREADER_H
