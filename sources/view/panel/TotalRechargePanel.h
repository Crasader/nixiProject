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

USING_NS_CC;

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
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
};

#endif /* TotalRechargePanel_hpp */
