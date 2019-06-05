#include <iostream>
#include <unordered_map>
#include <vector>
#include <iterator>

#include <utility>
#include <thread>
#include <chrono>
#include "include/PolicyReader.h"
#include "include/ValidatePolicy.h"
#include "include/PomdpPolicyChecker.h"
#include "FetchActionClient.h"

#ifdef GUI
#include <QApplication>
#endif

using namespace std;

int main(int argc, char *argv[]) {

#ifdef GUI
    QApplication app(argc, argv);
#endif
    FetchActionClient actionClient(argc,argv);
    string folderPath = actionClient.getFolderPath();
    PolicyReader reader(folderPath);
    reader.readFiles();
////    std::cout<<reader<<endl;
    auto b_0 = beliefstruct{0,0,1.0};
    auto Goal = beliefstruct{2,2,1.0};
    auto Gt = reader.GroundTruth;

    ValidatePolicy check(Goal,Gt);
    check.GetValidPolicy(reader.Policy);
    vector<CPtrs>data;
    for (int j = 0; j < reader.Policy.size(); ++j) {
        data.push_back(reader.Policy[j][0]);

    }

    actionClient(data,Gt);
//    PomdpPolicyChecker *PI;
//
//#ifdef SORT_POLICY
//    BinaryTreeSort BTS(b_0);
//    BTS.createTree(reader.Policy);
////    BTS.printPolicy();
//    PI= BTS.getLinkedList();
//#else
//    assert(argc>=3 &&"option cannot found");
//    int option = atoi(argv[2]);
//    for(auto &pi:reader.Policy)
//    {
////        cout << *pi.second[0] ;
//        /* 1 & 2. allocate new node & put in the data  */
//        auto node = new PomdpPolicy(pi.second[option]->action,pi.second[option]->robot);
//        node->setObstacleBelief(pi.second[option]->obstacles);
//        /* 3. Make next of new node as head */
//        node->next = PI;
//        /* 4. move the head to point to the new node */
//        PI    = node;
//    }
//#endif
//    cout <<"POMPD POLICY \n"<<endl;
//    int result=0;
//    while(PI)
//    {
//        cout<<*PI<< "\t\t<<<<!!!!>>>\t OBST: ";
//        printVec(PI->getObstaclesBelief());
//#ifdef GUI
//        result = popUpMsg(PI->getState(), PI->nextActionName());
//#endif
//
//        sendAction(PI->getAction());
//        PI = PI->next;
//        if(result == 4194304)break;
//    }

    return 0;
}