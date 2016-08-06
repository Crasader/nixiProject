//
//  HomeComp.hpp
//  tiegao
//
//  Created by mac on 16/8/5.
//
//

#ifndef HomeComp_hpp
#define HomeComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"
#include "extend_define.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class HomeComp : public CCObject
{
    SYNTHESIZE_PASS_BY_REF_READONLY(string, _curHouse, CurHouse);   // 当前用的房子的id
    SYNTHESIZE_READONLY(CCArray*, _houseTemplate, HouseTemplate);   // 所有的房子(每个下标对应一个房间id 类型为 CCString)
    SYNTHESIZE_READONLY(CCArray*, _houseUser, HouseUser);           // 玩家拥有的房子(每个下标对应一个房间id 类型为 CCString)
    SYNTHESIZE_READONLY(CCDictionary*, _scores, Scores);            // 3个游戏最高分的字典
    
public: // Export
    bool has_init_house_template();
    
public: // Inport
    void init_house_template(Value json);
    void replace_home_info(Value json);
    
public:
    ~HomeComp();
    CREATE_FUNC(HomeComp);
    virtual bool init();
};

#endif /* HomeComp_hpp */
