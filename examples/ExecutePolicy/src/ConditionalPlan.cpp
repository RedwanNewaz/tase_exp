//
// Created by redwan on 6/2/19.
//

#include "../include/ConditionalPlan.h"

ConditionalPlan::ConditionalPlan(string &pi, string &b) {
    // decode pi string
    policy_decoder(pi);
    // decode b string
    belief_decoder(b);
}

void ConditionalPlan::belief_decoder(string &b) {
    //add robot belief
    std::string delimiter = ";\t";

    auto data1 = splitString(b,delimiter);
    auto step = splitString(data1[0],"b");
    b_indx= atoi(step[1].c_str());
    auto data = splitString(data1[1],":");
    auto pos = splitString(data[0],",");
    robot=beliefstruct{atoi(pos[0].c_str()),atoi(pos[1].c_str()),atof(data[1].c_str())};


    //add obstacles belief
    auto obst = splitString(data1[2],"\t");
    for(auto &b:obst)
        if(!b.empty())
        {
            auto data = splitString(b,":");
            auto pos = splitString(data[0],",");
            auto state=beliefstruct{atoi(pos[0].c_str()),atoi(pos[1].c_str()),atof(data[1].c_str())};
            obstacles.push_back(state);
        }

}

void ConditionalPlan::policy_decoder(string &pi) {
    std::string token = pi.substr(3, pi.find(","));
    action =atoi(token.c_str());
}


vector<string> ConditionalPlan::read_file(string &filename) {
    std::ifstream filereader(filename);
    // read belief.txt file
    std::string line;
    vector<string> text;
    while (std::getline(filereader, line))
    {
        text.push_back(line);
    }
    filereader.close();
    return text;
}

