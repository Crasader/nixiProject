//
//  BarrageView.hpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#ifndef BarrageView_hpp
#define BarrageView_hpp

#include "cocos2d.h"

USING_NS_CC;

class BarrageView : public CCLayer
{
public:
    static void show();
    void addNewChat(CCObject* pObj);
    
public:
    ~BarrageView();
    CREATE_FUNC(BarrageView);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    
private:
};

#endif /* BarrageView_hpp */
