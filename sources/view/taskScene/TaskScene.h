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
    void enterTheKuang(float dt);
    void creat_Tishi();
    void EnterTheTishi();
    void ExitTishi();
    
    void exitView();
    void openTaskStoryScene();
    
    void creat_Man();
    void initClothes();
    
    CCString* getTaskName(int index);
    CCString* getTaskDescription(int index);
    
    void backCallBack(CCObject* pSender);
    void historyCallBack(CCObject* pSender);
    void startCallBack(CCObject* pSender);
    void _startCallBack(CCObject* pSender);
    
public:
    
    CCDictionary* allClothesDic;
    
    CCMenuItem* historyItem1, * historyItem2, * historyItem3, * historyItem4, * historyItem5, * backItem;
    
//    CCSprite* roomSpr;
    CCSprite* taskKuang;
    CCSprite* kuangSpr;
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* _tfSpr1; // 头发
    CCSprite* _tfSpr2;
    CCSprite* _tfSpr3;
    CCSprite* _wtSpr1; // 外套
    CCSprite* _wtSpr2;
    CCSprite* _wtSpr3;
    CCSprite* _sySpr1; // 上衣
    CCSprite* _sySpr2;
    CCSprite* _sySpr3;
    CCSprite* _kzSpr1; // 裤子
    CCSprite* _kzSpr2;
    CCSprite* _kzSpr3;
    CCSprite* _wzSpr1; // 袜子
    CCSprite* _wzSpr2;
    CCSprite* _wzSpr3;
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _spSpr1; // 饰品
    CCSprite* _spSpr2;
    CCSprite* _spSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _zrSpr1; // 妆容
    
    CCArray* taskArr;
    
    int OpenToWhichOne;
    int taskPhase;
    int taskIndex;    
};
#endif /* defined(__tiegao__TaskScene__) */
