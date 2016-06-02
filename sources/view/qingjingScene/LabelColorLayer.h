//
//  LabelColorLayer.h
//  mm3c
//
//  Created by mac on 14-11-10.
//
//

#ifndef __mm3c__LabelColorLayer__
#define __mm3c__LabelColorLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class LabelColorLayer : public CCLayerColor
{
public:
    LabelColorLayer();
    ~LabelColorLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
        
    static LabelColorLayer* create_with_index(const char* text);
    void init_with_index(const char* text);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(LabelColorLayer);
    
    void fhCallBack(CCObject* pSender);
    
    CCRect isDkRect();
    bool isDkContainTouchPoint1(CCTouch* touch);
    
public:
    
    CCSprite* dkSpr;
    CCLabelTTF* m_pLabel;
    int the_current_number_of_pages;
    const char* labelText;
    CC_SYNTHESIZE_RETAIN(CCArray*, _labelArr, LabelArr);
    
    void creatLabel();
    void on_left_button_clicked(CCObject* pSender);
    void on_right_button_clicked(CCObject* pSender);
    
};


#endif /* defined(__mm3c__LabelColorLayer__) */
