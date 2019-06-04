//
// Created by redwan on 6/3/19.
//

#include "BeliefState.h"

BeliefState::BeliefState(){}

BeliefState::BeliefState(int x, int y , float prob ):x(x),y(y),prob(prob)
{

}

void BeliefState::set(int x, int y, float prob) {
    this->x =x;
    this->y =y;
    this->prob = prob;
}

int BeliefState::getIndex(int x_size){
    return y*x_size+x;
}
std::tuple<int,int> BeliefState::getSub(int index, int x_size){
    return std::make_tuple( index%x_size, index/x_size);
}

double BeliefState::operator -(const BeliefState &rhs) const
{
    return abs(x-rhs.x)+abs(y-rhs.y);
//        return sqrt(pow(x-rhs.x,2)+pow(y-rhs.y,2));

}

bool BeliefState::operator==(const BeliefState &rhs) const
{
    return x==rhs.x && y==rhs.y;
}

bool BeliefState::operator<(const BeliefState &rhs) const {

    bool distE = rhs.y>=y && rhs.x>=x;
    auto dist = abs(x-rhs.x)+abs(y-rhs.y);

    if (dist <= 1 && distE )
        return true;
    else
        return false;

}

bool BeliefState::operator>(const BeliefState &rhs) const {
    return rhs < *this;
}

bool BeliefState::operator<=(const BeliefState &rhs) const {
    return !(rhs < *this);
}

bool BeliefState::operator>=(const BeliefState &rhs) const {
    return !(*this < rhs);
}

BeliefState BeliefState::operator +(int action)
{
    switch (action)
    {
        case 1:
        {
            //move north
            int xx=x, yy=y+1;
            return BeliefState{xx,yy,prob};
        }
        case 2:
        {
            //move south
            int xx=x, yy=y-1;
            return BeliefState{xx,yy,prob};
        }
        case 3:
        {
            //move west
            int xx=x-1, yy=y;
            return BeliefState{xx,yy,prob};;
        }
        case 4:
        {
            //move east
            int xx=x+1, yy=y;
            return BeliefState{xx,yy,prob};
        }

        default:
            return BeliefState{x,y,prob};
    }
}