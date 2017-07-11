//
//  NXQingjingScene.h
//  tiegao
//
//  Created by mac on 17-7-11.
//
//

#ifndef __tiegao__NXQingjingScene__
#define __tiegao__NXQingjingScene__

#include <iostream>
#include "cocos2d.h"
#include "NXBaseScene.h"
#include "AHMessageBox.h"
#include "NXClothesShopTableview.h"

USING_NS_CC;

#define ClothesShop_MAXNUM 4

class NXQingjingScene
: public NXBaseScene
{
public:
    
    NXQingjingScene();
    ~NXQingjingScene();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    static cocos2d::CCScene* scene();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    CREATE_FUNC(NXQingjingScene);
    
public:
    int num_child;
    
    CCSprite* bgSpr;

public:
    
    void creat_view();
    void creat_tishi();
    
    void backCallBack(CCObject* pSender);
    void qjCallback(CCObject* pSender);
    void qjCallback1(CCObject* pSender);
    void qjCallback2(CCObject* pSender);
    void qjCallback3(CCObject* pSender);
};
#endif /* defined(__tiegao__NXQingjingScene__) */
