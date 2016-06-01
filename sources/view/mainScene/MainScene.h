//
//  MainScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__MainScene__
#define __tiegao__MainScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class MainScene : public BaseScene
{
public:
    MainScene();
    ~MainScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(MainScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    
    void didAccelerate( CCAcceleration* pAccelerationValue);
    void gengxin(float dt);
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    
public:
    
    void creat_view();
    
    
    void huodongCallBack(CCObject* pSender);
    void qiandaoCallBack(CCObject* pSender);
    void youjianCallBack(CCObject* pSender);
    void renwuCallBack(CCObject* pSender);
    void huanzhuangCallBack(CCObject* pSender);
    void paihangCallBack(CCObject* pSender);
    void juqingCallBack(CCObject* pSender);
    void richangCallBack(CCObject* pSender);
    void shezhiCallBack(CCObject* pSender);
    
public:
    
    CCSprite* roomSpr;
    
    CCPoint playerVelocity;
    
};

#endif /* defined(__tiegao__MainScene__) */
