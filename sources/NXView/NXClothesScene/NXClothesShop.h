//
//  NXClothesShop.h
//  tiegao
//
//  Created by mac on 17-7-8.
//
//

#ifndef __tiegao__NXClothesShop__
#define __tiegao__NXClothesShop__

#include <iostream>
#include "cocos2d.h"
#include "NXBaseScene.h"
#include "AHMessageBox.h"
#include "NXClothesShopTableview.h"

USING_NS_CC;

#define ClothesShop_MAXNUM 4

class NXClothesShop
: public NXBaseScene
{
public:
    
    NXClothesShop();
    ~NXClothesShop();
    
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    static cocos2d::CCScene* scene();
    
    // type_id = clothesStatus - 1:换装任务, 2:纯换装, 3:神秘事件, 4:约会, 5:比拼
    // 公司任务和换装用
    static NXClothesShop* create_with_type(int _type_id, int _task_index, int _task_phase, const char *category, const char *tishi);
    void init_with_type(int _type_id, int _task_index, int _task_phase, const char *category, const char *tishi);
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(NXClothesShop);
    
public:
    int num_child;
    int clothesType;
    long clothesIndex;
    int clothes_type;
    int clothes_selected;
    int clothesStatus;
    int _buttonStatus;
    std::string buyClothesStr;
    void buyClothesMethods();
    int haveEnoughCoin();
    int haveEnoughGold();
    
    int task_index;
    int task_phase;
    // 神秘任务用
    const char* category;
    const char* tishi;
    
    CCMenuItem* buyItem;
    
    CCSprite* bgSpr;
    CCSprite* tuziSpr;
    
    NXClothesShopTableview* _delegate;
    
    CCMenuItemToggle* toggleItem[4];
    bool isSelect[4];
public:
    
    void creat_view();
    void creat_clothesInfo(CCObject* pSender);
    void craet_button();
    void creat_clothes(CCObject* pSender);
    void show_clothesInfo(float dt);
    
    void backCallBack(CCObject* pSender);
    void buttonCallBack(CCObject* pSender);
    void onSelect(CCObject* pSender);
    void allCallBack(CCObject* pSender);
    void coinCallBack(CCObject* pSender);
    void goldCallBack(CCObject* pSender);
    void debrisCallBack(CCObject* pSender);
    void buyCallback(CCObject* pSender);
    void create_buySuccess();
    bool animationBool;
    void removeAllSpr();
    
    void Http_Finished_401(CCObject* pObj);
};
#endif /* defined(__tiegao__NXClothesShop__) */
