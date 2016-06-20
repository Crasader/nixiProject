//
//  StoryComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef StoryComp_hpp
#define StoryComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class StoryComp : public CCObject
{
public:
    ~StoryComp();
    CREATE_FUNC(StoryComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    CCDictionary* cmpleted_story();
    
private:
    CCDictionary*           _cmpleted_story;
};

#endif /* StoryComp_hpp */
