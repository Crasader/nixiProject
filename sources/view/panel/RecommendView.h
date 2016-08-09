//
//  RecommendView.hpp
//  tiegao
//
//  Created by mac on 16/6/30.
//
//

#ifndef RecommendView_hpp
#define RecommendView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RecommendView : public CCLayer
{
public:
    static void show(CCNode* parent);
    
public:
    ~RecommendView();
    CREATE_FUNC(RecommendView);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void remove();
    void buy();
    
    void buy_energy_callback_101(CCObject* pObj);
    
private:
    CCSprite*           _panel;
    CCEditBox*          _input;
};

#endif /* RecommendView_hpp */
