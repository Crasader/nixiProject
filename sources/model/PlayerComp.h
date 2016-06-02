//
//  PlayerComp.hpp
//  tiegao
//
//  Created by mac on 16/5/21.
//
//

#ifndef PlayerComp_hpp
#define PlayerComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class PlayerComp : public CCObject
{
public:
    ~PlayerComp();
    CREATE_FUNC(PlayerComp);
    bool init();
    void init_with_json(Value json);
    
public:
    long long int   update;
    int             energy;
    
    int             exp;
    int             grade;
    int             popularity;
    
    int             coin;
    int             gold;
    int             diam;
    
    int             next_mission;
    int             cur_mission;
    int             company_phase;
    
protected:
    
private:

};

#endif /* PlayerComp_hpp */
