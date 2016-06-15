//
//  Dialogs.h
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#ifndef __manman_client__Dialogs__
#define __manman_client__Dialogs__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Dialogs : public CCObject
{
public:
    ~Dialogs();
    CREATE_FUNC(Dialogs);
    bool init();
    
    void config_with_file(const char* file);
    
    void clues_config_with_file(const char* file);
        
    void story_figure_config_with_file(const char* file, int PetId, int index);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _dialogs, Dialogs);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _cluesDialogs, CluesDialogs);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, _storyDialogs, StoryDialogs);
    
    CC_SYNTHESIZE(int, _zishi, ZiShi);
protected:
};

#endif /* defined(__manman_client__Dialogs__) */
