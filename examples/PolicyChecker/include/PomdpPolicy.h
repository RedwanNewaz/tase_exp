//
// Created by Redwan Newaz on 2019-06-01.
//

#ifndef POLICYCHECKER_POMDPPOLICY_H
#define POLICYCHECKER_POMDPPOLICY_H

#include <ostream>
#include <map>
#include <vector>
#include <string>
#include "PolicyReader.h"
using namespace std;
class PomdpPolicy;
inline void printPolicy(PomdpPolicy *n){

}

class PomdpPolicy {

public:
    PomdpPolicy(int action, const beliefstruct &belief);
    void setObstacleBelief(vector<beliefstruct>&obs);
    static void append( PomdpPolicy **head_ref, int action, const beliefstruct &belief, vector<beliefstruct>&obs);

    friend ostream &operator<<(ostream &os, const PomdpPolicy &Nodeicy);

    beliefstruct getState();
    vector<beliefstruct> getObstaclesBelief();
    int getAction();
    string nextActionName();
    PomdpPolicy *next;
private:
    int action;
    beliefstruct belief;
    vector<beliefstruct> obstacles;
    std::map<int,string>ACTION_MEANING;

};
// HOW TO USE IT
//BinaryTreeSort BTS(b_0);
//BTS.createTree(reader.Policy);
//BTS.printPolicy();

class BinaryTreeSort{
    struct Node{
        beliefstruct data;
        Node *left, *right;
        vector<beliefstruct> obstacles;
        Node(beliefstruct data, vector<beliefstruct>&obs, Node *left, Node *right):data(data), left(left),right(right){
            std::copy(obs.begin(), obs.end(),back_inserter(obstacles));
        }

    };
    Node *muTree;
    beliefstruct b_0;
    vector<Node*> filterdTree;
    vector<beliefstruct>obstacles;
public:
    BinaryTreeSort(const beliefstruct &b_0);
    void addNode(Node *muTree, beliefstruct b, vector<beliefstruct>&obs);
    void createTree(PolicyTree &policy);
    PomdpPolicy *getLinkedList();
    void printPolicy();
protected:
    void sortTree(Node *mu);

};





#endif //POLICYCHECKER_POMDPPOLICY_H
