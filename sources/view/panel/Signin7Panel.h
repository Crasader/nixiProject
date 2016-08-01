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
    static void show();
    
public:
    ~Signin7Panel();
    CREATE_FUNC(Signin7Panel);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    
private:
    CCLayer*            _content;
    CCSprite*           _panel;
};

#endif /* Signin7Panel_hpp */
