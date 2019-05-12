//
// Created by redwan on 5/10/19.
//

#include "../include/PomdpPolicy.h"

PomdpPolicy::PomdpPolicy(string &pi, string &b) {
    // decode pi string
    policy_decoder(pi);
    // decode b string
    belief_decoder(b);
    next = NULL;
}

void PomdpPolicy::belief_decoder(string &b) {

    //add robot belief
    std::string delimiter = ";\t";

    auto data1 = splitString(b,delimiter);
    auto step = splitString(data1[0],"b");
    b_indx_= atoi(step[1].c_str());
    auto data = splitString(data1[1],":");
    auto pos = splitString(data[0],",");
    robot_=State{atoi(pos[0].c_str()),atoi(pos[1].c_str()),atof(data[1].c_str())};


    //add obstacles belief
    auto obst = splitString(data1[2],"\t");
    for(auto &b:obst)
        if(!b.empty())
        {
            auto data = splitString(b,":");
            auto pos = splitString(data[0],",");
            auto state=State{atoi(pos[0].c_str()),atoi(pos[1].c_str()),atof(data[1].c_str())};
            obstacles_.push_back(state);
        }

}

void PomdpPolicy::policy_decoder(string &pi) {
    // assert if belief index does not match

    //add action action index 3
    std::string token = pi.substr(3, pi.find(","));
    add_action(atoi(token.c_str()));
    //add observation

    if(action_==look_north_idx || action_==look_south_idx||action_==look_west_idx||action_==look_east_idx)
    {
        string msg=getFloatNum(pi,6,",");
        add_observation(atof(msg.c_str()));

    }
    else if(action_ ==pick_up_right_hand_idx)
        add_observation(0.2);
    else
        add_observation(1);

}

void PomdpPolicy::add_action(int a) {
    action_ =a;
}

void PomdpPolicy::add_observation(float z) {
    observation_ = z;
}

vector<string> PomdpPolicy::splitString(string &s, std::string delimiter) {
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

string PomdpPolicy::getFloatNum(string &data, int start, char *delimiter) {
    string msg="";

    do{
        auto temp = data.substr(start, data.find(delimiter));
        if(temp.empty())
            break;
        msg += temp;
    }
    while(++start);
    return msg;
}

bool PomdpPolicy::isTerminal() {
    return  action_ ==pick_up_right_hand_idx;
}

void PomdpPolicy::append( PomdpPolicy **head_ref, string &pi, string &b) {
    /* 1. allocate PolicyTree */
    PomdpPolicy* new_PolicyTree = new PomdpPolicy(pi,b);
    PomdpPolicy *last = *head_ref;  /* used in step 5*/


    /* 3. This new PolicyTree is going to be the last PolicyTree, so make next of
          it as NULL*/
    new_PolicyTree->next = NULL;

    /* 4. If the Linked List is empty, then make the new PolicyTree as head */
    if (*head_ref == NULL)
    {
        *head_ref = new_PolicyTree;
        return;
    }

    /* 5. Else traverse till the last PolicyTree */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last PolicyTree */
    last->next = new_PolicyTree;
    return;
}

void PomdpPolicy::printPolicy(PomdpPolicy *n) {
    while (!n->isTerminal())
    {
        n->printRobotState();
        n = n->next;
    }
}

vector<string> PomdpPolicy::read_file(string &filename) {
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

void PomdpPolicy::printRobotState() {
    cout<<robot_<<endl;

}

QPoint PomdpPolicy::getState() {
    return QPoint(robot_.x,robot_.y);
}

vector<pair<QPoint, float>> PomdpPolicy::getObstacle() {
    vector<pair<QPoint, float>> collision;
    for(auto &o:obstacles_)
    {
        auto point = QPoint(o.x,o.y);
        auto b = make_pair(point,o.prob);
        collision.push_back(b);
    }
    return collision;

}


/**@brief Thnigs to do
 *  read policy, belief and test files \par
 *  generate policy tree using policy and belief tree. \par
 *  show ground truth obstacles in rviz \par
 *  show robot belief for obstacles and execute actions \par
 *  for move action compute the robot to set point difference first and don't execute action is diff is zero \par
 *  for look action rotate the head of the fetch only into desire location .
 */

PolicyExecution::PolicyExecution(MotionPrimitives *move) : TrajPlanner(move) {
    nh.getParam("/tase/policy/policy_file", policy_file);
    nh.getParam("/tase/policy/belief_file", belief_file);
    nh.getParam("/tase/policy/test_file", test_file);
    qDebug()<<policy_file.c_str();
    qDebug()<<belief_file.c_str();
    qDebug()<<test_file.c_str();

}

bool PolicyExecution::generate() {

    read_test_file();
    auto policy = PomdpPolicy::read_file(policy_file);
    auto belief = PomdpPolicy::read_file(belief_file);
    pi =NULL;
    for (int i = 0; i < policy.size(); ++i) {
        PomdpPolicy::append(&pi,policy[i],belief[i]);
    }

    return true;
}

void PolicyExecution::read_test_file() {
    ifstream test_reader;
    int num_obs,x_size, y_size;
    float delta;
    test_reader.open(test_file);
    if (test_reader.is_open()) {
        test_reader >> x_size >> y_size;
        test_reader >> delta;
        test_reader >> num_obs;

        for (int i = 0; i < num_obs; ++i) {
            int x, y;
            test_reader >> x >> y;
            ground_truth.push_back(QPoint(x, y));
        }
        test_reader.close();
    }

    qDebug()<<ground_truth;

}

void PolicyExecution::obstacleVisualization(PomdpPolicy *pi) {
    /** @brief obstacle colors \par
     * ground truth obstacles are yellow color. \par
     * robot belief on obstacles are red color. \par
     * if robot belief overlap with ground truth then replace the ground truth obstacle with robot belief \par
     * change color transparency with probability
     */
     // ground truth for visualization
    vector<pair<QPoint, float>> gt;
    auto collision = pi->getObstacle();
    for(auto &g:ground_truth)
    {
        bool isSame = false;
        for(auto &b:collision)
        {
            if(b.first==g)
            {
                isSame = true;
                break;
            }
        }
        if(!isSame)
            gt.push_back(make_pair(g,1.0));
    }
    // call visuzliation function
    int id = 0;
    for( auto &o:gt)
    {
        disp.setObstacles(++id,o.first,"yellow",o.second);
    }

    for( auto &o:collision)
    {
        disp.setObstacles(++id,o.first,"red",o.second);
    }

}

void PolicyExecution::run() {
    bool status = generate();
    if(!status)
    {
        std::cerr<<"no trajectory found"<<std::endl;
        exit(-1);
    }
    //TODO - create visulization class for showing gloabl trajectory

    // TODO - show robot tracking trajectory


    int step_index =-1;
    while (step_index<pi->b_indx_&& ros::ok())
    {
        auto point = pi->getState();
        int action = action_type(pi->action_);
        switch(action)
        {
            case MOVE:{
                qDebug()<< "move to " <<point;
                move_->goTo(point.x(),point.y());
                obstacleVisualization(pi);

                break;
            }
            case LOOK:{
                qDebug()<< "look at " <<point;
                obstacleVisualization(pi);

                break;

            }
            case PICK:{
                qDebug()<< "pick up at " <<point;

                move_->goTo(point.x(),point.y());
                obstacleVisualization(pi);
                break;

            }
        }

        step_index = pi->b_indx_;
        pi = pi->next;
//        std::this_thread::sleep_for(chrono::seconds(2));
//        disp.setTrajectory(trajectory_);

    }

    qDebug()<<"Planner thread terminated";
}

PolicyExecution::ACT_TYPE PolicyExecution::action_type(int a) {
    if(a<5)
        return MOVE;
    else if(a>=5 &&a<9)
        return LOOK;
    else
        return PICK;
}
