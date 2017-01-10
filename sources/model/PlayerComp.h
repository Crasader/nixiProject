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
    void init_custom(Value json);
    
public:
    long long int   update;
    int             energy;
    int             left;               // 倒计时时间
    
    int             coin;
    int             gold;
    int             diam;
    
    int             mission;            // 最新解锁任务
    int             phase;              // 公司规模
    CCDictionary*   rating;             // 总任务评价(总星数)
    
    int             mystery;            // 兼容旧版事件解锁
    
    CC_SYNTHESIZE(int, _guide, Guide);  // 引导
    
    int             missionEnergyCost;
    int             mysteryEnergyCost;
    int             trystEnergyCost;
    int             storyEnergyCost;
    int             vipStoryBuyCost;
    
public:
    bool hasCommitIdentity();    // 是否完善资料
    int total_ratings();         // 所有阶段总星数
    int ratings(int phase);      // 某阶段星数

private:
    int             _identity;          // 是否完善资料

    
private:
    void test();
};

#endif /* PlayerComp_hpp */
