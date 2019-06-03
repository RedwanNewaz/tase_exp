//
// Created by Redwan Newaz on 2019-06-01.
//

#include "../include/PomdpPolicyChecker.h"

PomdpPolicyChecker::PomdpPolicyChecker(int action, const beliefstruct &belief) : action(action), belief(belief) {
    ACTION_MEANING[1]="Move north";
    ACTION_MEANING[2]="Move south";
    ACTION_MEANING[3]="Move west ";
    ACTION_MEANING[4]="Move east ";
    ACTION_MEANING[5]="Look north";
    ACTION_MEANING[6]="Look south";
    ACTION_MEANING[7]="Look west ";
    ACTION_MEANING[8]="Look east ";
    ACTION_MEANING[9]="Pick up left hand ";
    ACTION_MEANING[10]="Pick up right hand";
}

beliefstruct PomdpPolicyChecker::getState() {
    return belief;
}

int PomdpPolicyChecker::getAction() {
    return action;
}

ostream &operator<<(ostream &os, const PomdpPolicyChecker &policy) {



    os << "action: " <<policy.action << " belief: " << policy.belief;
    return os;
}

void PomdpPolicyChecker::setObstacleBelief(vector<beliefstruct> &obs) {
    std::copy(obs.begin(), obs.end(),back_inserter(obstacles));

}

vector<beliefstruct> PomdpPolicyChecker::getObstaclesBelief() {
    return obstacles;
}

string PomdpPolicyChecker::nextActionName() {
    return ACTION_MEANING[action];
}

void PomdpPolicyChecker::append(PomdpPolicyChecker **head_ref, int action, const beliefstruct &belief, vector<beliefstruct>&obs) {
    /* 1. allocate PolicyTree */
    PomdpPolicyChecker* new_PolicyTree = new PomdpPolicyChecker(action,belief);
    new_PolicyTree->setObstacleBelief(obs);
    PomdpPolicyChecker *last = *head_ref;  /* used in step 5*/


    /* 3. This new PolicyTree is going to be the last PolicyTree, so make next of
          it as NULL*/
    new_PolicyTree->next = NULL;

    /* 4. If the Linked List is empty, then make the new PolicyTree as head */
    if (*head_ref == NULL)
    {
        *head_ref = new_PolicyTree;
        return  ;
    }

    /* 5. Else traverse till the last PolicyTree */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last PolicyTree */
    last->next = new_PolicyTree;
    return  ;

}


BinaryTreeSort::BinaryTreeSort(const beliefstruct &b_0) : b_0(b_0) {

}

void BinaryTreeSort::addNode(BinaryTreeSort::Node *muTree, beliefstruct b, vector<beliefstruct>&obs) {
    if(b< muTree->data)
    {
        if(muTree->left!=NULL)
            addNode(muTree->left, b, obs);
        else
        {
            muTree->left=new Node(b,obs,NULL,NULL);

        }
    }
    else if(b>=muTree->data)
    {
        if(muTree->right!=NULL)
            addNode(muTree->right, b, obs);
        else
        {
            muTree->right=new Node(b,obs,NULL,NULL);
        }
    }

}

void BinaryTreeSort::createTree(PolicyTree &policy)
{

    bool first = true;
    for(int i =0; i<policy.size();++i)
    {
        auto pi = policy[i];
        for(auto &belief:pi)
        {

            if(belief->b_indx<0)continue;
            auto b = belief->robot;
            b.prob = belief->action;
            if(first)
                muTree = new Node(b,belief->obstacles,NULL,NULL);
            else
                addNode(muTree,b, belief->obstacles);

            first = false;

        }

    }
    sortTree(muTree);
}

void BinaryTreeSort::sortTree(Node *mu)
{
    if(!mu)
        return;

    auto distance = mu->data - b_0;
    if(distance<=1)
    {
        b_0 = mu->data;
        filterdTree.emplace_back(mu);

    }
    sortTree(mu->left);
    sortTree(mu->right);
}

void BinaryTreeSort::printPolicy()
{
    for(auto &mu:filterdTree)
        cout << mu->data <<endl;
}

PomdpPolicyChecker* BinaryTreeSort::getLinkedList()
{
    PomdpPolicyChecker *PI=NULL;

    for(auto &pi:filterdTree)
    {
        PomdpPolicyChecker::append(&PI,pi->data.prob,pi->data,pi->obstacles);
//        auto load = new PomdpPolicyChecker(pi->data.prob,pi->data);
//        load->setObstacleBelief(pi->obstacles);
//        load->next = PI;
//        PI    = load;
    }

    return PI;
}