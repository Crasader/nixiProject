//
//  TaskSettlementLayer2.h
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#ifndef __tiegao__TaskSettlementLayer2__
#define __tiegao__TaskSettlementLayer2__

#include <iostream>
#include "cocos2d.h"
#include "FlashNumberLabel.h"
#include "FlashNumberLabel2.h"
#include "FlashNumberLabel3.h"
#include "FlashNumberLabel4.h"


USING_NS_CC;

class TaskSettlementLayer2
: public CCLayer
, public FlashNumberLabelDelegate
, public FlashNumberLabel2Delegate
, public FlashNumberLabel3Delegate
, public FlashNumberLabel4Delegate
{
public:
    ~TaskSettlementLayer2();
    static TaskSettlementLayer2* create(int rating, int coin, int energy, bool isPhaseUP);
    virtual bool init(int rating, int coin, int energy, bool isPhaseUP);
    virtual void onEnter();
    virtual void onExit();
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){}
    
    virtual void will_number_scroll();
    virtual void did_number_stoped();
    
    virtual void will_number_scroll2();
    virtual void did_number_stoped2();
    
    virtual void will_number_scroll3();
    virtual void did_number_stoped3();
    
    virtual void will_number_scroll4();
    virtual void did_number_stoped4();
    
public:
    void creat_view();
    void lingquCallBack(CCObject* pSender);
    void shareCallBack(CCObject* pSender);
    void shareStatus(float dt);
    void _321CallBack(CCObject* pSender);
    
    void creat_Man();
    void initClothes();
    
    void initAniamtion();
    void startAnimation(float dt);
    
    void updataNumber();
    void creatXingXing();
    
    void openLogic();
    
    std::string content;
    int wordCount;
    int contentLength;
    void logic(float dt);
    const char* getContent(){return content.c_str();}
    void init(std::string str);
    std::string getContentByLength(int length);
    int getContentLength();
    
    void exit();
    void nextAnimation1();
    void nextAnimation2();
public:
    
    bool logic_open_bool;
    bool logic_end_Bool;
    bool animationBool;
    bool lingquBool;
    int randIndex1;
    int randIndex2;
    int randIndex3;
    int randIndex4;
    
    int num_child;
    
    
    CCSprite* kuangSpr1;
    CCSprite* coinSpr;
    CCSprite* renSpr;
    CCLabelTTF* coinLabel;
    
    CCSprite* xingSpr1;
    CCSprite* xingSpr2;
    CCSprite* xingSpr3;
    CCSprite* xingSpr4;
    CCSprite* xingSpr5;
    
    FlashNumberLabel* taskLabel1;
    FlashNumberLabel2* taskLabel2;
    FlashNumberLabel3* taskLabel3;
    FlashNumberLabel4* taskLabel4;
    
    CCMenuItem* lingquItem,* shareItem;
    CCString* tishiStr;
    CCLabelTTF* tishiLabel;
    
    CCDictionary* allClothesDic;
    
    CCSprite* tiaoSpr;
    
    CCSprite* roomSpr;
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
    
private:
    int             _rating;
    int             _coin;
    int             _energy;
    bool            _isPhaseUP;
};
#endif /* defined(__tiegao__TaskSettlementLayer2__) */
