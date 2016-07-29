//
//  MissionComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef MissionComp_hpp
#define MissionComp_hpp


#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class MissionComp : public CCObject
{
public: // Export
    CCDictionary* cmpleted_mission();
    int mission_rating(const char* mission_index);
    
public:
    ~MissionComp();
    CREATE_FUNC(MissionComp);
    bool init();
    void init_with_json(Value json);
    
private:
    CCDictionary*           _cmpleted_mission;
};

#endif /* MissionComp_hpp */
