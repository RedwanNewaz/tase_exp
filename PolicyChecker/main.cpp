#include <iostream>
#include <unordered_map>
#include <vector>
#include <iterator>
#include "include/PolicyReader.h"
#include "include/ValidatePolicy.h"
#include "include/PomdpPolicy.h"

//#define GUI true
#define SORT_POLICY true

#ifdef GUI
#include <QApplication>
#include <QtCore>
#include <QMessageBox>

int popUpMsg(beliefstruct robot, string act)
{
    QMessageBox msgBox;
    int window_w(1920),window_h(1080);
    msgBox.move((window_w-msgBox.width())/2,(window_h-msgBox.height())/2);
    QString x=QString::number(robot.x),y=QString::number(robot.y);
    QString msg = "Loc: ("+x+","+y+")";
    msgBox.setText("Continue Next Motion? [ "+QString(act.c_str())+"]");
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    return msgBox.exec();
}
#endif
using namespace std;

int main(int argc, char *argv[]) {

#ifdef GUI
    QApplication app(argc, argv);
#endif


    std::cout << "Policy, Checker!" << std::endl;
    assert(argc>=2 &&"folder path cannot found");

    string folderPath = argv[1];
    PolicyReader reader(folderPath);
    reader.readFiles();
////    std::cout<<reader<<endl;
    auto b_0 = beliefstruct{0,0,1.0};
    auto Goal = beliefstruct{2,2,1.0};
    auto Gt = reader.GroundTruth;

    ValidatePolicy check(Goal,Gt);
    check.GetValidPolicy(reader.Policy);

    PomdpPolicy *PI;

#ifdef SORT_POLICY
    BinaryTreeSort BTS(b_0);
    BTS.createTree(reader.Policy);
//    BTS.printPolicy();
    PI= BTS.getLinkedList();
#else
    assert(argc>=3 &&"option cannot found");
    int option = atoi(argv[2]);
    for(auto &pi:reader.Policy)
    {
//        cout << *pi.second[0] ;
        /* 1 & 2. allocate new node & put in the data  */
        auto node = new PomdpPolicy(pi.second[option]->action,pi.second[option]->robot);
        node->setObstacleBelief(pi.second[option]->obstacles);
        /* 3. Make next of new node as head */
        node->next = PI;
        /* 4. move the head to point to the new node */
        PI    = node;
    }
#endif
    cout <<"POMPD POLICY \n"<<endl;
    int result=0;
    while(PI)
    {
        cout<<*PI<< "\t\t<<<<!!!!>>>\t OBST: ";
        printVec(PI->getObstaclesBelief());
#ifdef GUI
        result = popUpMsg(PI->getState(), PI->nextActionName());
#endif
        PI = PI->next;
        if(result == 4194304)break;
    }





    return 0;
}