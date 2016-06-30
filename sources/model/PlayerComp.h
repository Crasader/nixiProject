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
    
    int             coin;
    int             gold;
    int             diam;
    
    int             next_mission;       // 下一个任务(最新解锁)
    int             cur_mission;        // 进行中的任务
    int             phase;              // 公司规模
    int             rating;             // 总任务评价(总星数)
    int             energyBuyTimes;     // 体力每日已购次数
    
protected:
    
private:

};

#endif /* PlayerComp_hpp */
