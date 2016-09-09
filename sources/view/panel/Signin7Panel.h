//
//  Signin7Panel.hpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#ifndef Signin7Panel_hpp
#define Signin7Panel_hpp

#include "cocos2d.h"

USING_NS_CC;

class Signin7Panel : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~Signin7Panel();
    CREATE_FUNC(Signin7Panel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void config_siginInfo();
    void signin_callback_303();
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
    int                 _signin_id;
    
    int                 _clothesId;
};

#endif /* Signin7Panel_hpp */
