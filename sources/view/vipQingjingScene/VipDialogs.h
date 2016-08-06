//
//  VipDialogs.h
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#ifndef __manman_client__VipDialogs__
#define __manman_client__VipDialogs__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class VipDialogs : public CCObject
{
public:
    ~VipDialogs();
    CREATE_FUNC(VipDialogs);
    bool init();
    
    void config_with_file(const char* file);
    
    void clues_config_with_file(const char* file);
        
    void story_figure_config_with_file(const char* file, int PetId, int index);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _VipDialogs, VipDialogs);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _cluesVipDialogs, CluesVipDialogs);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _storyVipDialogs, StoryVipDialogs);
    
    CC_SYNTHESIZE(int, _zishi, ZiShi);
protected:
};

#endif /* defined(__manman_client__VipDialogs__) */
