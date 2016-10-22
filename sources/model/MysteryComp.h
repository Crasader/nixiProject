//
//  MysteryComp.hpp
//  tiegao
//
//  Created by mac on 16/10/22.
//
//

#ifndef MysteryComp_hpp
#define MysteryComp_hpp

#include "cocos2d.h"
#include "json_lib.h"
#include "extend_define.h"

USING_NS_CC;
using namespace CSJson;

class MysteryComp : public CCObject
{
public: // Export
    // category: "1","2","3"
    CCArray* fetchTemplate(const char* category);
    
public:
    ~MysteryComp();
    CREATE_FUNC(MysteryComp);
    bool init();
    void init_template(Value json);
    void update_user_data(Value json);
    
private:
    CCDictionary*           mysteryTemplate;
    CCDictionary*           mysteryUserdata;
};

#endif /* MysteryComp_hpp */
