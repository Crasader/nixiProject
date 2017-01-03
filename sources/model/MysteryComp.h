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
    bool hasInitAchvTemplate();
    CCDictionary* fetchPrecondition(const char* category);
    // category: "1","2","3"
    CCArray* fetchAchvTemplate(const char* category);
    int userRatingOfCategory(const char* category);
    // @return: 0-未达成；1-可领；2-已领
    int userAchvStateOfCategory(const char* category, const char* achvId);
    
public:
    ~MysteryComp();
    CREATE_FUNC(MysteryComp);
    bool init();
    void init_precondition(Value json);
    void init_template(Value json);
    void update_user_data(Value json);
    
private:
    CCDictionary*           mysteryPrecondition;
    CCDictionary*           mysteryAchvTemplate;
    CCDictionary*           mysteryUserdata;
};

#endif /* MysteryComp_hpp */
