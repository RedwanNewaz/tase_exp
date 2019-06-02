//
// Created by Redwan Newaz on 2019-05-31.
//

#ifndef POLICYCHECKER_VALIDATEPOLICY_H
#define POLICYCHECKER_VALIDATEPOLICY_H

#include <iostream>
#include <z3++.h>
#include "PolicyReader.h"
#include <cassert>
#include <iterator>

class ValidatePolicy {
public:
    ValidatePolicy(){};
    ValidatePolicy(const beliefstruct &m_goal, const vector<beliefstruct> &m_obstacles);
    void GetValidPolicy(PolicyTree &policy);

    std::tuple<bool,PolicyTree> GoalConstraint(PolicyTree& policy, beliefstruct Goal,  bool refinement = false);
private:
    z3::context ctx;

    const z3::expr x =ctx.int_const("x");
    const z3::expr y =ctx.int_const("y");
    const z3::expr Gx =ctx.int_const("Gx");
    const z3::expr Gy =ctx.int_const("Gy");
    beliefstruct m_goal;
    vector<beliefstruct> m_obstacles;

protected:
    void PolicyRefinement(z3::model &m, PolicyTree &pi);

};


#endif //POLICYCHECKER_VALIDATEPOLICY_H
