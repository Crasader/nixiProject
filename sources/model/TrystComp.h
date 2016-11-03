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

typedef enum {
    none = 0,
    ongoing = 1,
    takable = 2
} TrystStatus;

class TrystUserdata : public CCObject
{
public:
    ~TrystUserdata();
    static TrystUserdata* create(Value json);
    
    TrystStatus         status;
    string              curTrystId;
    int                 leftTime;
    
private:
};

// -----------------------------------------------------
// -----------------------------------------------------

class TrystComp : public CCObject
{
public: // Export
    CC_SYNTHESIZE_RETAIN(TrystUserdata*, userData, UserData);
    bool isOngoing();
    bool hasInitAchvTemplate();
    // category: "1","2","3"
    CCArray* fetchAchvTemplate(const char* category);
    
public:
    ~TrystComp();
    CREATE_FUNC(TrystComp);
    bool init();
    void init_template(Value json);
    void update_user_data(Value json);
    
private:
    CCDictionary*           trystTemplate;
};

#endif /* TrystComp_hpp */
