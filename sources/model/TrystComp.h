//
//  TrystComp.hpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#ifndef TrystComp_hpp
#define TrystComp_hpp

#include "cocos2d.h"
#include "json_lib.h"
#include "extend_define.h"

USING_NS_CC;
using namespace CSJson;

class TrystComp : public CCObject
{
public: // Export
    bool isOngoing();
    bool hasInitAchvTemplate();
    // category: "1","2","3"
    CCArray* fetchAchvTemplate(const char* category);
    int userRatingOfCategory(const char* category);
    // @return: 0-未达成；1-可领；2-已领
    int userAchvStateOfCategory(const char* category, const char* achvId);
    
public:
    ~TrystComp();
    CREATE_FUNC(TrystComp);
    bool init();
    void init_template(Value json);
    void update_user_data(Value json);
    
private:
    CCDictionary*           mysteryAchvTemplate;
    CCDictionary*           mysteryUserdata;
};

#endif /* TrystComp_hpp */
