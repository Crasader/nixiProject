//
//  PhoneLayer2.h
//  tiegao
//
//  Created by mac on 16-7-18.
//
//

#ifndef __tiegao__PhoneLayer2__
#define __tiegao__PhoneLayer2__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"


USING_NS_CC;


class PhoneLayer2 : public BaseScene
{
public:
    PhoneLayer2();
    ~PhoneLayer2();
    
    void onEnter();
    void onExit();
    
    bool init();
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(PhoneLayer2);
    
public:
    
    std::string content;
    std::string labStr;
    
    int contentLength;
    int wordCount;
    int logIndex;
    
    CCLabelTTF* saidLabel;
    
    bool openStory;
    
    CCProgressTimer* _progress1;
    CCProgressTimer* _progress2;
    
public:
    
    void creat_view();
    void creat_man();
    int qingjingStatus();
    void backCallBack(CCObject* pSender);
    void buquCallBack(CCObject* pSender);
    void quCallBack(CCObject* pSender);
    void _500CallBack(CCObject* pSender);
    
    const char* getContent(){return content.c_str();}
    void init(std::string str);
    std::string getContentByLength(int length);
    int getContentLength();
    void logic(float dt);
    void openTouch(float dt);
};
#endif /* defined(__tiegao__PhoneLayer2__) */
