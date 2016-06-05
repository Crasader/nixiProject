//
//  TaskScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__TaskScene__
#define __tiegao__TaskScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class TaskScene : public BaseScene
{
public:
    TaskScene();
    ~TaskScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(TaskScene);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void keyBackClicked();
    
public:
    
    void creat_view();
    void creat_Tishi();
    void EnterTheTishi();
    void ExitTishi();
    
    CCString* getTaskName(int index);
    CCString* getTaskDescription(int index);
    
    void backCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    
public:
    
    CCSprite* roomSpr;
    CCSprite* taskKuang;
    CCSprite* kuangSpr;
    
    CCArray* taskArr;
    
};
#endif /* defined(__tiegao__TaskScene__) */
