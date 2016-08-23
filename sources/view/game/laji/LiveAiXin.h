//
//  LiveAiXin.h
//  mm3c
//
//  Created by lakkey on 16-1-12.
//
//

#ifndef __mm3c__LiveAiXin__
#define __mm3c__LiveAiXin__

#include "cocos2d.h"

USING_NS_CC;

class Garbage : public CCSprite {
    
    int result;
public:
    static   Garbage*  create( int type);
    virtual  bool      initWithFile(int type);
    
    int getResult();
    
};


class LiveAiXin : public CCLayer
{
    ~LiveAiXin();
    bool init();
    void funCallback(CCNode * sender);
    void funCallback1(CCNode * sender);
    void setupView();
    
    CCSprite * m_box;
    CCSprite * m_hidden_box;
    
    CCSprite * m_animationSprite;
    CCAnimate * m_bombAction;
    
    int m_number;
    int ciIndex;
    float startFolat;
    float timeFloat;
    
    float m_collDistatance;
    CCArray * garbages;
    CCSprite * prBg;
    
    void crecteGarbage(float dt);
    void addcome();
    void removeGarbage(Garbage * garbage);
    
public:
    
    CREATE_FUNC(LiveAiXin);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
    
    void schedules(float dt);
    virtual void update(float dt);
    void checkForCollision();
    void gameOver();
    void setplayerPositionX(float x);
    
    void callfuncN1(CCNode * sender);
    void runAnimate(CCObject*pSender);
    void openRun();
    void closeRun();
    void updateTime(float dt);
    bool isHaveSaveFile();
    
    void openTouch(float dt);
    
    bool animationBool;
    void starAnimation();
    
protected:
    CCString* strName;
    bool m_Isbomb;
    void resume(float dt);
    bool m_time_going;
    bool overBool;
    bool canRun;
    CCSprite*       luo;
    CCSprite*       timeBg;
    CCSprite*       tishikuang;
    CCLabelTTF*     label_num;
    int             m_score;
};

#endif /* defined(__mm3c__LiveAiXin__) */
