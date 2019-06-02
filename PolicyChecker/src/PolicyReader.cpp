//
// Created by Redwan Newaz on 2019-05-31.
//

#include "../include/PolicyReader.h"

PolicyReader::PolicyReader(const string &dirname) : __dirname(dirname) {
    assert(!dirname.empty() && "no directory found" );
    __policyFile = dirname+"/policy.txt";
    __beliefFile = dirname+"/belief.txt";
    __testFile = dirname+"/corridor_test_2";
}

void PolicyReader::m_beliefDecoder(string &s) {

    std::string delimiter = ";";
    BELIEF_STR = splitString(s,delimiter);

}

void PolicyReader::m_policyDecoder(string &s) {
    std::string delimiter = ",";
    POLICY_STR = splitString(s,delimiter);
}

void PolicyReader::readFiles() {
    string line;
    string FILES[]={__beliefFile,__policyFile, __testFile};
    vector<list<string>> DATA;
    for (int i = 0; i < 3; ++i) {
        ifstream myfile (FILES[i]);
        if (myfile.is_open())
        {
            list<string> src;
            while ( getline (myfile,line) )
            {
                src.push_back(line);
            }
            myfile.close();
            DATA.push_back(src);
        }

        else cerr << "Unable to open file";
    }
    DATA[0].pop_back();
    cout << "belief files "<<DATA[0].size() << '\n';
    cout << "policy files "<<DATA[1].size() << '\n';

    auto B = DATA[0].begin();
    auto P = DATA[1].begin();


    for (int j = 0; j < DATA[1].size(); ++j) {
        m_beliefDecoder(*B);
        m_policyDecoder(*P);
//        printVec(BELIEF_STR);
        auto plan = make_shared<ConditionalPlan>(BELIEF_STR,POLICY_STR);
        if(Policy.find(plan->b_indx)== Policy.end())
        {
            // create a new vector
            vector<CPtrs> newPlan{plan->getPtr()};
            Policy[plan->b_indx]=newPlan;
        }
        else
        {
            //update vector
            auto newPlan = Policy[plan->b_indx];

            newPlan.push_back(plan->getPtr());
            Policy[plan->b_indx]=newPlan;
        }
        B++; P++;
    }

//    printVec(Policy);
    int count =0;
    for (auto &test:DATA[2])
       if(++count>3)
       {
           auto d = splitString(test," ");
           GroundTruth.emplace_back(beliefstruct{atoi(d[0].c_str()),atoi(d[1].c_str()),1.0});
//           cout<< d[1]<<endl;
       }
    printVec(GroundTruth);


}

ostream &operator<<(ostream &os, const PolicyReader &reader) {

    for (auto &pi:reader.Policy) {
        os<< pi.first<<"\t";
        for(auto &d:pi.second )
            os<<*d<<"\t";
        os<<endl;
    }
    return os;
}



ConditionalPlan::ConditionalPlan(vector<string> &b, vector<string> &pi) {

    auto init = splitString(b[0],"b");
    b_indx = atoi(init[1].c_str());
    robot = m_getRobotBelief(b[1]);
    obstacles = m_getObstacleBelief(b[2]);
    action = atoi(pi[1].c_str());
    robot = robot + action;

//    printVec(pi);
}

beliefstruct ConditionalPlan::m_getRobotBelief(string &b) {
    auto data = splitString(b,":");
    float prob = atof(data[1].c_str());
    data = splitString(data[0],",");
    beliefstruct belief{atoi(data[0].c_str()),atoi(data[1].c_str()),prob};
//    cout<<belief<<endl;
    return belief;
}

vector<beliefstruct> ConditionalPlan::m_getObstacleBelief(string &b) {
    vector<beliefstruct> obstacles;
    std::istringstream iss(b);
    std::vector<std::string> newbelief((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());
    for (auto &bbelief :newbelief)
        obstacles.push_back(m_getRobotBelief(bbelief));
    return obstacles;
}

ostream &operator<<(ostream &os, const ConditionalPlan &plan) {
    os<< "b_indx: "<<plan.b_indx << " robot: " << plan.robot << " obstacles: " << plan.obstacles.size()<< " action: " << plan.action<< endl;
    return os;
}

CPtrs ConditionalPlan::getPtr() {
    return shared_from_this();
}
