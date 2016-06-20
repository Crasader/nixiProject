//
//  Loading2.hpp
//  tiegao
//
//  Created by mac on 16/6/20.
//
//

#ifndef Loading2_hpp
#define Loading2_hpp

#include "cocos2d.h"

USING_NS_CC;

#define LOADING     Loading2::Inst()

class Loading2 : public CCLayer
{
public:
    ~Loading2();
    static Loading2* Inst();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_loading();
    void show_loading(CCNode* node);
    void stop_loading();
    void remove();
    
private:
    void init_content();
    CCString* choose_icon();
    void loading(float dt);
    
private:
    CCLayer*            _content;
    CCArray*            _icons;
    CCObject*           _choosed_icon;
    CCSprite*           _icon;
    CCSprite*           _star1;
    CCSprite*           _star2;
    CCSprite*           _star3;
    CCSprite*           _star4;
    CCSprite*           _dot1;
    CCSprite*           _dot2;
    CCSprite*           _dot3;
};

#endif /* Loading2_hpp */
