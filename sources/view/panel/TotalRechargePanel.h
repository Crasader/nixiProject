//
//  TotalRechargePanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/8/5.
//
//

#ifndef TotalRechargePanel_hpp
#define TotalRechargePanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TotalRechargePanel : public CCLayer
{
public: // Export
    static void show();
    
public:
    ~TotalRechargePanel();
    CREATE_FUNC(TotalRechargePanel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void updatePanel();
    void btn_lingqu_callback();
    void btn_chongzhi_callback();
    void reward_callback_305(CCObject *obj);
    
    void btn_left_callback();
    void btn_right_callback();
    
private:
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    CCMenuItemSprite*   _item;
    int                 _curIndex;
    CCMenuItemSprite*   _item_left;
    CCMenuItemSprite*   _item_right;
};

#endif /* TotalRechargePanel_hpp */
