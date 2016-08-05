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
    SYNTHESIZE_PASS_BY_REF_READONLY(string, _curHouse, CurHouse);
    SYNTHESIZE_READONLY(CCArray*, _houseTemplate, HouseTemplate);
    SYNTHESIZE_READONLY(CCArray*, _houseUser, HouseUser);
    SYNTHESIZE_READONLY(CCDictionary*, _scores, Scores);
    
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
