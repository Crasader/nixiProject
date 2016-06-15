//
//  QingjingScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__QingjingScene__
#define __tiegao__QingjingScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class QingjingScene : public BaseScene
{
public:
    QingjingScene();
    ~QingjingScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(QingjingScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    void creat_Tishi();
    void EnterTheTishi();
    void ExitTishi();
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    
public:
    
    CCSprite* roomSpr;
    CCSprite* qingjingKuang;
    CCSprite* kuangSpr;
    
};
#endif /* defined(__tiegao__QingjingScene__) */
