//
//  TempSignin.hpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#ifndef TempSignin_hpp
#define TempSignin_hpp

#include "cocos2d.h"

USING_NS_CC;

class TempSignin : public CCLayer
{
public: // Export
    static void show(CCNode* parent);
    
public:
    ~TempSignin();
    CREATE_FUNC(TempSignin);
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
    int num_child;
    
    CCLayer*            _content;
    CCSprite*           _panel;
    int                 _signin_id;
    
    int                 _clothesId;
};

#endif /* TempSignin_hpp */
