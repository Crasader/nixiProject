//
//  VipStorySettlementOfTheAnimationLayer.h
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#ifndef __tiegao__VipStorySettlementOfTheAnimationLayer__
#define __tiegao__VipStorySettlementOfTheAnimationLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class VipStorySettlementOfTheAnimationLayer : public CCLayerColor
{
public:
    VipStorySettlementOfTheAnimationLayer();
    ~VipStorySettlementOfTheAnimationLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
    
    static VipStorySettlementOfTheAnimationLayer* create_with_index(int index, std::string ending);
    void init_with_index(int index, std::string ending);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(VipStorySettlementOfTheAnimationLayer);
    
public:
    
    void openTouch(float dt);
    void closeCallBack(CCObject* pSender);
    
    void creatStar1(float dt);
    void creatStar2(float dt);
    void creatStar3(float dt);
    void creatStar4(float dt);
    void creatStar5(float dt);
    void creatStar6(float dt);
};



#endif /* defined(__tiegao__VipStorySettlementOfTheAnimationLayer__) */
