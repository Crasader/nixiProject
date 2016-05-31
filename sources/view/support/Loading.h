//
//  Loading.hpp
//  tiegao
//
//  Created by mac on 16/5/31.
//
//

#ifndef Loading_hpp
#define Loading_hpp

#include "cocos2d.h"

USING_NS_CC;

#define LOADING     Loading::Inst()

class Loading : public CCLayer
{
public:
    ~Loading();
    static Loading* Inst();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    void show_loading();
    void show_loading(CCNode* node);
    void stop_loading();
    void remove();
    
private:
    void loading();
    
private:
    CCSprite*           _loading;
};

#endif /* Loading_hpp */
