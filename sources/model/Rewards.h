//
//  Rewards.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef Rewards_hpp
#define Rewards_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class Rewards : public CCObject
{
public:
    ~Rewards();
    CREATE_FUNC(Rewards);
    bool init();
    
    void init_with_json(Value json);
    
public:
    void print_self();
    
    int         coin;
    int         diam;
    int         energy;
    
private:
};

#endif /* Rewards_hpp */
