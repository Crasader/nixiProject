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
    int energy();
    int exp();
    int grade();
    int popularity();       // 人气
    int coin();
    int gold();
    int diam();
    int cur_mission();
    int company_phase();    // 公司规模
    
protected:
    
private:
    int _energy;
    
    int _exp;
    int _grade;
    int _popularity;
    
    int _coin;
    int _gold;
    int _diam;
    
    int _cur_mission;
    int _company_phase;
};

#endif /* PlayerComp_hpp */
