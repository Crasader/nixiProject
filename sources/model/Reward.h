//
//  Reward.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef Reward_hpp
#define Reward_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class Reward : public CCObject
{
public:
    ~Reward();
    CREATE_FUNC(Reward);
    bool init();
    
    void init_with_json(Value json);
    
public:
    void print_self();
    
    int         coin;
    int         diam;
    int         energy;
    
private:
};

#endif /* Reward_hpp */
