//
//  SocialComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef SocialComp_hpp
#define SocialComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class SocialComp : public CCObject
{
public:
    ~SocialComp();
    CREATE_FUNC(SocialComp);
    bool init();
    
    void init_with_json(Value json);
    void update_stranger(Value json);
    
public:
    
protected:
    
private:
    CCDictionary*       _strangers;
};

#endif /* SocialComp_hpp */
