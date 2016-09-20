//
//  TaskTaskLabelColorLayer.h
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#ifndef __tiegao__TaskLabelColorLayer__
#define __tiegao__TaskLabelColorLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class TaskLabelColorLayer : public CCLayerColor
{
public:
    TaskLabelColorLayer();
    ~TaskLabelColorLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    static TaskLabelColorLayer* create_with_index(const char* text);
    void init_with_index(const char* text);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(TaskLabelColorLayer);
    
    void fhCallBack(CCObject* pSender);
    
    CCRect isDkRect();
    bool isDkContainTouchPoint1(CCTouch* touch);
    
public:
    int num_child;
    
    CCSprite* dkSpr;
    CCLabelTTF* m_pLabel;
    int the_current_number_of_pages;
    const char* labelText;
    CC_SYNTHESIZE_RETAIN(CCArray*, _labelArr, LabelArr);
    
    void creatLabel();
    void on_left_button_clicked(CCObject* pSender);
    void on_right_button_clicked(CCObject* pSender);
    
};



#endif /* defined(__tiegao__TaskLabelColorLayer__) */
