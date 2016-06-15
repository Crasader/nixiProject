//
//  PromptLayer.h
//  mm3c
//
//  Created by mac on 15-7-6.
//
//

#ifndef __mm3c__PromptLayer__
#define __mm3c__PromptLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class PromptLayer
: public CCLayer
{
public:
    PromptLayer();
    ~PromptLayer();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(PromptLayer);
    
    void promptBox(int idx);
    void promptBox(CCString* str);
    void promptBoxSprite();
    
    void show_prompt(CCNode* parent, const char*);

protected:
    void removeself();
};

#endif /* defined(__mm3c__PromptLayer__) */
