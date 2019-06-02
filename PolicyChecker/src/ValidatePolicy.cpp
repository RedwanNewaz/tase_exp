//
// Created by Redwan Newaz on 2019-05-31.
//

#include "../include/ValidatePolicy.h"

/** *
 * @param m_goal
 * @param m_obstacles
 * change conditional plan b_indx = -1 if violate the constraint
 */


ValidatePolicy::ValidatePolicy(const beliefstruct &m_goal, const vector<beliefstruct> &m_obstacles) : m_goal(m_goal),
                                                                                                      m_obstacles(
                                                                                                              m_obstacles) {

}

std::tuple<bool, PolicyTree> ValidatePolicy::GoalConstraint(PolicyTree &policy, beliefstruct Goal, bool refinement) {

    z3::expr_vector cond(ctx), goalCond(ctx);
    cond.push_back(x==Gx);
    cond.push_back(y==Gy);
    goalCond.push_back(Goal.x==Gx);
    goalCond.push_back(Goal.y==Gy);


    auto gen_name =[&](string start, int i, int j){
        start+="_"+to_string(i)+"_"+to_string(j);
        return start.c_str();
    };
    z3::expr_vector belief(ctx);

    int i,j;
    PolicyTree::iterator pi;
    for(pi=policy.begin(), i=0;pi!=policy.end(); ++pi)
    {
        for(j = 0; j<pi->second.size();++j)
        {
            auto plan = pi->second.at(j);
            auto robot = plan->robot;
            i = plan->b_indx;
            if(plan->b_indx<0)continue;

            z3::expr bx = ctx.int_const(gen_name("x",i,j));
            z3::expr by = ctx.int_const(gen_name("y",i,j));
            z3::expr_vector states(ctx);
            z3::expr_vector values(ctx);
            z3::expr_vector combines(ctx);
            states.push_back(x == bx);
            states.push_back(y == by);
            values.push_back(robot.x == bx);
            values.push_back(robot.y == by);
            combines.push_back(z3::mk_and(states));
            combines.push_back(z3::mk_and(cond));
            combines.push_back(z3::mk_and(values));
            belief.push_back(z3::mk_and(combines));
        }
    }


    z3::expr variable = z3::mk_or(belief);
    z3::expr GoalCond = z3::mk_and(goalCond);

    auto s = z3::solver(ctx);
    s.add(variable);
    s.add(GoalCond);

    bool result = s.check()==z3::sat;
    cout<<"result "<<s.check()<<endl;
    if (result && refinement)
    {
        z3::model m = s.get_model();
//        cout<<m<<endl;
        auto indexes = getList(m);
        for (auto &i:indexes)
            cout<<i.first <<"\t"<<i.second <<endl;

        pi_size_ = policy.size();
        PolicyRefinement(m, policy);
        GoalConstraint(policy,Goal,true);
    }


    return make_tuple(result, policy);
}

void ValidatePolicy::PolicyRefinement(z3::model &m, PolicyTree &pi) {

    auto unsatIndx = getList(m);
    setInValidPolicy(unsatIndx, pi);

}

void ValidatePolicy::GetValidPolicy(PolicyTree &policy) {
    bool result = false;
    std::tie(result, policy)= GoalConstraint(policy,m_goal);
    assert(result && "Goal does not exist in the policy tree");
    cout<<"REACHABILITY SATISFIED"<<endl;
    for (auto & o:m_obstacles)
    {
        std::tie(result, policy) = GoalConstraint(policy,o,true);
    }
    cout<<"SAFETY SATISFIED"<<endl;


}

ValidatePolicy::unsatVec ValidatePolicy::getList(z3::model &m) {
    unsatVec unsat;
    for (int i = 0; i < m.num_consts(); i++) {
        if(i%2==0)continue;
        auto fd = m.get_const_decl(i);
        z3::expr r = m.get_const_interp(fd);
        std::istringstream iss(fd.to_string());
        std::vector<std::string> newbelief((std::istream_iterator<std::string>(iss)),
                                           std::istream_iterator<std::string>());
        if (newbelief[1].find("_") != std::string::npos)
        {
            auto strs = splitString(newbelief[1],"_");
            int b_indx =  atoi(strs[1].c_str());
            int p_indx = atoi(strs[2].c_str());
            unsat.push_back(make_pair(b_indx,p_indx));
        }
    }
    return unsat;
}

void ValidatePolicy::setInValidPolicy(ValidatePolicy::unsatVec &indx, PolicyTree &pi) {
    /**@brief
     * change conditional plan b_indx = -1 if it violates the constraint
     */
    for (auto &in :indx)
    {
        int i = in.first;
        int j = in.second;
        pi[i][j]->b_indx = -1;
    }

}
