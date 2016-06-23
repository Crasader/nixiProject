//
//  ClothesScene.cpp
//  tiegao
//
//  Created by mac on 16-6-15.
//
//

#include "ClothesScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "MZResourceLoader.h"
#include "ConfigManager.h"
#include "TaskScene.h"
#include "Loading2.h"
#include "NetManager.h"

ClothesScene::ClothesScene(){
    
}
ClothesScene::~ClothesScene(){
    
}
ClothesScene* ClothesScene::create_with_type(int _type_id, int _task_index, int _task_phase){
    ClothesScene* rtn = ClothesScene::create();
    rtn->init_with_type(_type_id, _task_index, _task_phase);
    
    return rtn;
}
void ClothesScene::init_with_type(int _type_id, int _task_index, int _task_phase){
    
    clothesStatus = _type_id;
    task_index = _task_index;
    task_phase = _task_phase;
    startTask = false;
    tili_AllIndex = 10;
    
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    bgSpr = CCSprite::create("res/pic/clothesScene/gj_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/pic/clothesScene/gj_back.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/clothesScene/gj_back.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(ClothesScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .037f));
    
    // 任务开始
    CCSprite* startSpr1;
    CCSprite* startSpr2;
    CCMenuItem* startItem;
    if (clothesStatus == 1) {// 任务
        startSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_start.png");
        startSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_start.png");
        startSpr2->setScale(1.02f);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(ClothesScene::startCallBack));
    }else if (clothesStatus == 2){// 换装
        startSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_save.png");
        startSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_save.png");
        startSpr2->setScale(1.02f);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(ClothesScene::saveCallBack));
    }
    startItem->setAnchorPoint(ccp(1.f, .5f));
    startItem->setPosition(ccp(DISPLAY->ScreenWidth() - backItem->getContentSize().width - startItem->getContentSize().width* .95f, DISPLAY->ScreenHeight()* .032f));
    CCMenu* menu = CCMenu::create(backItem, startItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 15);
    
    this->creat_View();
    this->creat_Man();
    this->initClothes();
    this->crate_Tishi();
}
bool ClothesScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    buttonTag = Tag_GJ_TouFa;
    isClothesType = 0;
    
    return true;
}

void ClothesScene::onEnter(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(ClothesScene::ChangeClothes), "ChangeClothes", NULL);
    nc->addObserver(this, menu_selector(ClothesScene::ChangClothesIndex), "ChangClothesIndex", NULL);
    nc->addObserver(this, menu_selector(ClothesScene::buttonStatus), "ButtonStatus", NULL);
    nc->addObserver(this, menu_selector(ClothesScene::creat_money), "Creat_money", NULL);
    
    nc->addObserver(this, menu_selector(ClothesScene::Http_Finished_401), "HTTP_FINISHED_401", NULL);
    nc->addObserver(this, menu_selector(ClothesScene::Http_Finished_601), "HTTP_FINISHED_601", NULL);
    nc->addObserver(this, menu_selector(ClothesScene::Http_Finished_602), "HTTP_FINISHED_602", NULL);
    
    BaseScene::onEnter();
}

void ClothesScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    BaseScene::onExit();
}

void ClothesScene::keyBackStatus(float dt){
    
}
void ClothesScene::keyBackClicked(){
    
}

bool ClothesScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    
    
    return true;
}
void ClothesScene::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
}

cocos2d::CCScene* ClothesScene::scene(){
    CCScene* scene = CCScene::create();
    ClothesScene* layer = ClothesScene::create();
    scene->addChild(layer);
    
    return scene;
}

void ClothesScene::openTouch(float dt){
    
}
void ClothesScene::creat_money(){
    if (clothKuangSpr->getChildByTag(0x11111) != NULL) {
        clothKuangSpr->removeChildByTag(0x11111);
    }
    if (clothKuangSpr->getChildByTag(0x22222) != NULL) {
        clothKuangSpr->removeChildByTag(0x22222);
    }
    if (clothKuangSpr->getChildByTag(0x33333) != NULL) {
        clothKuangSpr->removeChildByTag(0x33333);
    }
    if (clothKuangSpr->getChildByTag(0x44444) != NULL) {
        clothKuangSpr->removeChildByTag(0x44444);
    }
    
    CCSprite* goldSpr = CCSprite::create("pic/clothesScene/gj_gold.png");
    goldSpr->setScale(.83f);
    goldSpr->setPosition(ccp(clothKuangSpr->getContentSize().width* .32f, clothKuangSpr->getContentSize().height* .115f));
    goldSpr->setTag(0x11111);
    clothKuangSpr->addChild(goldSpr);
    CCString* goldStr = CCString::createWithFormat("%d", haveEnoughGold());
//    CCString* goldStr = CCString::createWithFormat("%d", 9999999);
    CCLabelTTF* goldLabel = CCLabelTTF::create(goldStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(clothKuangSpr->getContentSize().width* .6f, 25), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    goldLabel->setPosition(ccp(clothKuangSpr->getContentSize().width* .67f, clothKuangSpr->getContentSize().height* .113f));
    goldLabel->setColor(ccWHITE);
    goldLabel->setTag(0x33333);
    clothKuangSpr->addChild(goldLabel);
    
    CCSprite* coinSpr = CCSprite::create("pic/clothesScene/gj_coin.png");
    coinSpr->setScale(.85f);
    coinSpr->setPosition(ccp(clothKuangSpr->getContentSize().width* .32f, clothKuangSpr->getContentSize().height* .09f));
    coinSpr->setTag(0x22222);
    clothKuangSpr->addChild(coinSpr);
    CCString* coinStr = CCString::createWithFormat("%d", haveEnoughCoin());
//    CCString* coinStr = CCString::createWithFormat("%d", 9999999);
    CCLabelTTF* coinLabel = CCLabelTTF::create(coinStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(clothKuangSpr->getContentSize().width* .6f, 25), kCCTextAlignmentCenter,kCCVerticalTextAlignmentCenter);
    coinLabel->setPosition(ccp(clothKuangSpr->getContentSize().width* .67f, clothKuangSpr->getContentSize().height* .088f));
    coinLabel->setColor(ccWHITE);
    coinLabel->setTag(0x44444);
    clothKuangSpr->addChild(coinLabel);
}
void ClothesScene::creat_View(){
    if (this->getChildByTag(0x2222) != NULL) {
        this->removeChildByTag(0x2222);
    }
    
    // 试衣间底框
    clothKuangSpr = CCSprite::create("res/pic/clothesScene/gj_bgkuang.png");
    clothKuangSpr->setAnchorPoint(ccp(1, 0.f));
    clothKuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()+7, +2));
    clothKuangSpr->setTag(0x2222);
    this->addChild(clothKuangSpr, 5);
    
    // 购买
    CCSprite* buySpr1 = CCSprite::create("res/pic/clothesScene/gj_buy.png");
    CCSprite* buySpr2 = CCSprite::create("res/pic/clothesScene/gj_buy.png");
    buySpr2->setScale(1.02f);
    CCMenuItem* buyItem = CCMenuItemSprite::create(buySpr1, buySpr2, this, menu_selector(ClothesScene::buyCallBack));
    buyItem->setPosition(ccp(clothKuangSpr->getContentSize().width* .49f, clothKuangSpr->getContentSize().height* .037f));
    CCMenu* buyMenu = CCMenu::create(buyItem, NULL);
    buyMenu->setPosition(CCPointZero);
    clothKuangSpr->addChild(buyMenu);
    
    CCMenuItem* tfItem;
    CCSprite* tfSpr1;
    CCSprite* tfSpr2;
    if (buttonTag == Tag_GJ_TouFa) {
        tfSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        tfSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        tfItem = CCMenuItemSprite::create(tfSpr1, tfSpr2, this, NULL);
    }else{
        tfSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        tfSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        tfSpr2->setScale(1.01f);
        tfItem = CCMenuItemSprite::create(tfSpr1, tfSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    tfItem->setTag(Tag_GJ_TouFa);
    CCSprite* tfSpr = CCSprite::create("res/pic/clothesScene/button/gj_toufa.png");
    tfSpr->setAnchorPoint(CCPointZero);
    tfSpr->setPosition(CCPointZero);
    tfItem->addChild(tfSpr);
    ycSpr1 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr1->setPosition(ccp(tfSpr->getContentSize().width* .75f, tfSpr->getContentSize().height* .3f));
    ycSpr1->setVisible(false);
    tfSpr->addChild(ycSpr1);
    
    
    CCMenuItem* wtItem;
    CCSprite* wtSpr1;
    CCSprite* wtSpr2;
    if (buttonTag == Tag_GJ_WaiTao) {
        wtSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        wtSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        wtItem = CCMenuItemSprite::create(wtSpr1, wtSpr2, this, NULL);
    }else{
        wtSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        wtSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        wtSpr2->setScale(1.01f);
        wtItem = CCMenuItemSprite::create(wtSpr1, wtSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    wtItem->setTag(Tag_GJ_WaiTao);
    CCSprite* wtSpr = CCSprite::create("res/pic/clothesScene/button/gj_waitao.png");
    wtSpr->setAnchorPoint(CCPointZero);
    wtSpr->setPosition(CCPointZero);
    wtItem->addChild(wtSpr);
    ycSpr2 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr2->setPosition(ccp(wtSpr->getContentSize().width* .75f, wtSpr->getContentSize().height* .3f));
    ycSpr2->setVisible(false);
    wtSpr->addChild(ycSpr2);
    
    CCMenuItem* syItem;
    CCSprite* sySpr1;
    CCSprite* sySpr2;
    if (buttonTag == Tag_GJ_ShangYi) {
        sySpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        sySpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        syItem = CCMenuItemSprite::create(sySpr1, sySpr2, this, NULL);
    }else{
        sySpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        sySpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        sySpr2->setScale(1.01f);
        syItem = CCMenuItemSprite::create(sySpr1, sySpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    syItem->setTag(Tag_GJ_ShangYi);
    CCSprite* sySpr = CCSprite::create("res/pic/clothesScene/button/gj_shangyi.png");
    sySpr->setAnchorPoint(CCPointZero);
    sySpr->setPosition(CCPointZero);
    syItem->addChild(sySpr);
    ycSpr3 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr3->setPosition(ccp(sySpr->getContentSize().width* .75f, sySpr->getContentSize().height* .3f));
    ycSpr3->setVisible(false);
    sySpr->addChild(ycSpr3);
    
    CCMenuItem* kzItem;
    CCSprite* kzSpr1;
    CCSprite* kzSpr2;
    if (buttonTag == Tag_GJ_KuZi) {
        kzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        kzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        kzItem = CCMenuItemSprite::create(kzSpr1, kzSpr2, this, NULL);
    }else{
        kzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        kzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        kzSpr2->setScale(1.01f);
        kzItem = CCMenuItemSprite::create(kzSpr1, kzSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    kzItem->setTag(Tag_GJ_KuZi);
    CCSprite* kzSpr = CCSprite::create("res/pic/clothesScene/button/gj_kuzi.png");
    kzSpr->setAnchorPoint(CCPointZero);
    kzSpr->setPosition(CCPointZero);
    kzItem->addChild(kzSpr);
    ycSpr4 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr4->setPosition(ccp(kzSpr->getContentSize().width* .75f, kzSpr->getContentSize().height* .3f));
    ycSpr4->setVisible(false);
    kzSpr->addChild(ycSpr4);
    
    CCMenuItem* wzItem;
    CCSprite* wzSpr1;
    CCSprite* wzSpr2;
    if (buttonTag == Tag_GJ_WaZi) {
        wzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        wzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        wzItem = CCMenuItemSprite::create(wzSpr1, wzSpr2, this, NULL);
    }else{
        wzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        wzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        wzSpr2->setScale(1.01f);
        wzItem = CCMenuItemSprite::create(wzSpr1, wzSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    wzItem->setTag(Tag_GJ_WaZi);
    CCSprite* wzSpr = CCSprite::create("res/pic/clothesScene/button/gj_wazi.png");
    wzSpr->setAnchorPoint(CCPointZero);
    wzSpr->setPosition(CCPointZero);
    wzItem->addChild(wzSpr);
    ycSpr5 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr5->setPosition(ccp(wzSpr->getContentSize().width* .75f, wzSpr->getContentSize().height* .3f));
    ycSpr5->setVisible(false);
    wzSpr->addChild(ycSpr5);
    
    CCMenuItem* xzItem;
    CCSprite* xzSpr1;
    CCSprite* xzSpr2;
    if (buttonTag == Tag_GJ_XieZi) {
        xzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        xzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        xzItem = CCMenuItemSprite::create(xzSpr1, xzSpr2, this, NULL);
    }else{
        xzSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        xzSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        xzSpr2->setScale(1.01f);
        xzItem = CCMenuItemSprite::create(xzSpr1, xzSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    xzItem->setTag(Tag_GJ_XieZi);
    CCSprite* xzSpr = CCSprite::create("res/pic/clothesScene/button/gj_xiezi.png");
    xzSpr->setAnchorPoint(CCPointZero);
    xzSpr->setPosition(CCPointZero);
    xzItem->addChild(xzSpr);
    ycSpr6 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr6->setPosition(ccp(xzSpr->getContentSize().width* .75f, xzSpr->getContentSize().height* .3f));
    ycSpr6->setVisible(false);
    xzSpr->addChild(ycSpr6);
    
    CCMenuItem* spItem;
    CCSprite* spSpr1;
    CCSprite* spSpr2;
    if (buttonTag == Tag_GJ_ShiPin) {
        spSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        spSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        spItem = CCMenuItemSprite::create(spSpr1, spSpr2, this, NULL);
    }else{
        spSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        spSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        spSpr2->setScale(1.01f);
        spItem = CCMenuItemSprite::create(spSpr1, spSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    spItem->setTag(Tag_GJ_ShiPin);
    CCSprite* spSpr = CCSprite::create("res/pic/clothesScene/button/gj_shipin.png");
    spSpr->setAnchorPoint(CCPointZero);
    spSpr->setPosition(CCPointZero);
    spItem->addChild(spSpr);
    ycSpr7 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr7->setPosition(ccp(spSpr->getContentSize().width* .75f, spSpr->getContentSize().height* .3f));
    ycSpr7->setVisible(false);
    spSpr->addChild(ycSpr7);
    
    CCMenuItem* baoItem;
    CCSprite* baoSpr1;
    CCSprite* baoSpr2;
    if (buttonTag == Tag_GJ_Bao) {
        baoSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        baoSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button2.png");
        baoItem = CCMenuItemSprite::create(baoSpr1, baoSpr2, this, NULL);
    }else{
        baoSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        baoSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_button1.png");
        baoSpr2->setScale(1.01f);
        baoItem = CCMenuItemSprite::create(baoSpr1, baoSpr2, this, menu_selector(ClothesScene::buttonCallBack));
    }
    baoItem->setTag(Tag_GJ_Bao);
    CCSprite* baoSpr = CCSprite::create("res/pic/clothesScene/button/gj_baobao.png");
    baoSpr->setAnchorPoint(CCPointZero);
    baoSpr->setPosition(CCPointZero);
    baoItem->addChild(baoSpr);
    ycSpr8 = CCSprite::create("res/pic/clothesScene/gj_yichuan2.png");
    ycSpr8->setPosition(ccp(baoSpr->getContentSize().width* .75f, baoSpr->getContentSize().height* .3f));
    ycSpr8->setVisible(false);
    baoSpr->addChild(ycSpr8);
    
    CCMenu* menu = CCMenu::create(tfItem, wtItem, syItem, kzItem, wzItem, xzItem, spItem, baoItem, NULL);
    menu->alignItemsVerticallyWithPadding(18.f);
    menu->setPosition(ccp(- tfItem->getContentSize().width* .5f, clothKuangSpr->getContentSize().height* .55f));
    menu->setTag(0x3333);
    clothKuangSpr->addChild(menu, 5);
    
    ClothesTableView* tabLayer = ClothesTableView::create();
    tabLayer->setPosition(ccp(0, clothKuangSpr->getContentSize().height* .128f));
    isClothesType = Tag_GJ_TouFa;
    _delegate = tabLayer;
    tabLayer->setTag(0x77777);
    clothKuangSpr->addChild(tabLayer, 5);
    
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    if (buttonTag == Tag_GJ_TouFa){
        
        isClothesType = Tag_GJ_TouFa;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_TouFa);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_TouFa);
    }else if (buttonTag == Tag_GJ_WaiTao){
        
        isClothesType = Tag_GJ_WaiTao;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_WaiTao);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_WaiTao);
    }else if (buttonTag == Tag_GJ_ShangYi){
        
        isClothesType = Tag_GJ_ShangYi;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_ShangYi);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_ShangYi);
        
    }else if (buttonTag == Tag_GJ_KuZi){
        
        isClothesType = Tag_GJ_KuZi;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_KuZi);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_KuZi);
        
    }else if (buttonTag == Tag_GJ_WaZi){
        
        isClothesType = Tag_GJ_WaZi;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_WaZi);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_WaZi);
        
    }else if (buttonTag == Tag_GJ_XieZi){
        
        isClothesType = Tag_GJ_XieZi;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_XieZi);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_XieZi);
        
    }else if (buttonTag == Tag_GJ_ShiPin){
        
        isClothesType = Tag_GJ_ShiPin;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_ShiPin);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_ShiPin);
        
    }else if (buttonTag == Tag_GJ_Bao){
        
        isClothesType = Tag_GJ_Bao;
        
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_Bao);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        DATA->setDataSource(tempArr);
        _delegate->updateTableView(Tag_GJ_Bao);
        
    }
    
    this->buttonStatus();
    this->creat_money();
}
void ClothesScene::crate_Tishi(){
    if (clothesStatus == 1) {// 任务
        CCSprite* renwukuangSpr = CCSprite::create("res/pic/clothesScene/gj_renwukuang.png");
        renwukuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* .9f));
        this->addChild(renwukuangSpr, 10);
        
        CCArray* taskArr = CONFIG->mission();
        CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(task_index);
        int tag1 = dic->valueForKey("tag1")->intValue();
        int tag2 = dic->valueForKey("tag2")->intValue();
        int tag3 = dic->valueForKey("tag3")->intValue();
        if (tag1 > 0) {
            CCString* tagStr1 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag1);
            CCSprite* tagSpr1 = CCSprite::create(tagStr1->getCString());
            tagSpr1->setPosition(ccp(renwukuangSpr->getContentSize().width* .25f, renwukuangSpr->getContentSize().height* .4f));
            renwukuangSpr->addChild(tagSpr1);
        }
        if (tag2 > 0) {
            CCString* tagStr2 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag2);
            CCSprite* tagSpr2 = CCSprite::create(tagStr2->getCString());
            tagSpr2->setPosition(ccp(renwukuangSpr->getContentSize().width* .5f, renwukuangSpr->getContentSize().height* .4f));
            renwukuangSpr->addChild(tagSpr2);
        }
        if (tag3 > 0) {
            CCString* tagStr3 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag3);
            CCSprite* tagSpr3 = CCSprite::create(tagStr3->getCString());
            tagSpr3->setPosition(ccp(renwukuangSpr->getContentSize().width* .75f, renwukuangSpr->getContentSize().height* .4f));
            renwukuangSpr->addChild(tagSpr3);
        }
        
    }else if (clothesStatus == 2){// 换装
        
    }
    
    CCSprite* zhuangrongSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_zhuangrong.png");
    CCSprite* zhuangrongSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_zhuangrong.png");
    zhuangrongSpr2->setScale(1.02f);
    CCSprite* texiaoSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_texiao.png");
    CCSprite* texiaoSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_texiao.png");
    texiaoSpr2->setScale(1.02f);
    
    CCMenuItem* zhuangrongItem = CCMenuItemSprite::create(zhuangrongSpr1, zhuangrongSpr2, this, menu_selector(ClothesScene::zhuangrongCallBack));
    CCMenuItem* texiaoItem = CCMenuItemSprite::create(texiaoSpr1, texiaoSpr2, this, menu_selector(ClothesScene::texiaoCallBack));
    if (clothesStatus == 1) {// 任务
        zhuangrongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .82f));
        texiaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .74f));
    }else if (clothesStatus == 2){// 换装
        zhuangrongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .9f));
        texiaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .07f, DISPLAY->ScreenHeight()* .82f));
    }
    CCMenu* menu = CCMenu::create(zhuangrongItem, texiaoItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 10);
}
void ClothesScene::texiaoCallBack(CCObject* pSender){
    
}
void ClothesScene::zhuangrongCallBack(CCObject* pSender){
    
}
void ClothesScene::buttonCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_Button_Common, false);
//    }
    
    buttonTag = item->getTag();
    if (clothKuangSpr->getChildren() != NULL) {
        clothKuangSpr->removeAllChildren();
    }
    
    this->creat_View();
}
void ClothesScene::backCallBack(CCObject* pSender){
    DATA->getClothes()->copy_clothesTemp();// 还原衣服
    
    if (clothesStatus == 1) {// 任务
        CCScene* scene = TaskScene::scene();
        CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (clothesStatus == 2){// 换装
        CCScene* scene = MainScene::scene();
        CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}
void ClothesScene::startCallBack(CCObject* pSender){
    startTask = true;
    LOADING->show_loading();
    NET->save_dressed_401(DATA->getClothes()->MyClothesTemp());
}
void ClothesScene::buyCallBack(CCObject* pSender){
    if (DATA->getPlayer()->coin >= haveEnoughCoin() && DATA->getPlayer()->diam >= haveEnoughGold()) {
        LOADING->show_loading();
        NET->save_dressed_401(DATA->getClothes()->MyClothesTemp());
    }else if (DATA->getPlayer()->coin < haveEnoughCoin() || DATA->getPlayer()->diam < haveEnoughGold()){
        if (DATA->getPlayer()->diam < haveEnoughGold()) {
            AHMessageBox* mb = AHMessageBox::create_with_message("钻石不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO2, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            return;
        }else if (!haveEnoughCoin()){
            AHMessageBox* mb = AHMessageBox::create_with_message("金币不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO3, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
        }
    }
}
void ClothesScene::saveCallBack(CCObject* pSender){
    if (DATA->getPlayer()->coin >= haveEnoughCoin() && DATA->getPlayer()->diam >= haveEnoughGold()) {
        LOADING->show_loading();
        NET->save_dressed_401(DATA->getClothes()->MyClothesTemp());
    }else if (DATA->getPlayer()->coin < haveEnoughCoin() || DATA->getPlayer()->diam < haveEnoughGold()){
        if (DATA->getPlayer()->diam < haveEnoughGold()) {
            AHMessageBox* mb = AHMessageBox::create_with_message("钻石不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO2, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            return;
        }else if (!haveEnoughCoin()){
            AHMessageBox* mb = AHMessageBox::create_with_message("金币不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO3, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
        }
    }
}
void ClothesScene::creat_Man(){
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(_touSpr, 210);
}
void ClothesScene::initClothes(){//穿衣服
    CCDictionary* dress = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    
    for (int i = Tag_GJ_TouFa; i <= Tag_GJ_Bao; i++) {
        if (i == Tag_GJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr1->setTag(Tag_GJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr2->setTag(Tag_GJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr1->setTag(Tag_GJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr2->setTag(Tag_GJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr3->setTag(Tag_GJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _wtSpr1->setTag(Tag_GJ_WaiTao1);
                _ManSpr->addChild(_wtSpr1, def_z_order);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr2->setTag(Tag_GJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr3->setTag(Tag_GJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _sySpr1->setTag(Tag_GJ_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr1->setTag(Tag_GJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr2->setTag(Tag_GJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr3->setTag(Tag_GJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _kzSpr1->setTag(Tag_GJ_KuZi1);
                _ManSpr->addChild(_kzSpr1, 305);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr1->setTag(Tag_GJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr2->setTag(Tag_GJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr3->setTag(Tag_GJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _wzSpr1->setTag(Tag_GJ_WaZi1);
                _ManSpr->addChild(_wzSpr1, def_z_order);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr1->setTag(Tag_GJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr2->setTag(Tag_GJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr3->setTag(Tag_GJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _xzSpr1->setTag(Tag_GJ_XieZi1);
                _ManSpr->addChild(_xzSpr1, def_z_order);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr1->setTag(Tag_GJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr2->setTag(Tag_GJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr3->setTag(Tag_GJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )dress->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿饰品的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, def_z_order);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_GJ_Bao){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _bSpr1->setTag(Tag_GJ_Bao1);
                _ManSpr->addChild(_bSpr1, def_z_order);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        const CCString* layer4 =  clothDic->valueForKey("layer4");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr1->setTag(Tag_GJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr2->setTag(Tag_GJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr3->setTag(Tag_GJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        
                        if (layer4->compare("") != 0) {
                            CCString* str4 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer4")->intValue());
                            _bSpr4 = CCSprite::create(str4->getCString());
                            _bSpr4->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr4->setTag(Tag_GJ_Bao4);
                            _ManSpr->addChild(_bSpr4, clothDic->valueForKey("z_order4")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }

    this->scheduleOnce(SEL_SCHEDULE(&ClothesScene::openTouch), 1.f);
}
void ClothesScene::ChangClothesIndex(CCObject* pSender){
    changClothesIndex = (long)pSender;
}
void ClothesScene::ChangeClothes(CCObject* pSender){
    
    long index = (long)pSender;
    
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_Button_Clothes, false);
//    }
    
    if (index >= 10000 && index < 20000){
        isClothesType = Tag_GJ_TouFa;
    }else if (index >= 20000 && index < 30000){
        isClothesType = Tag_GJ_WaiTao;
    }else if (index >= 30000 && index < 40000){
        isClothesType = Tag_GJ_ShangYi;
    }else if (index >= 40000 && index < 50000){
        isClothesType = Tag_GJ_KuZi;
    }else if (index >= 50000 && index < 60000){
        isClothesType = Tag_GJ_WaZi;
    }else if (index >= 60000 && index < 70000){
        isClothesType = Tag_GJ_XieZi;
    }else if (index >= 70000 && index < 80000){
        isClothesType = Tag_GJ_ShiPin;
    }else if (index >= 80000 && index < 90000){
        isClothesType = Tag_GJ_Bao;
    }
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    switch (isClothesType) {
        case Tag_GJ_TouFa:{
            if (_ManSpr->getChildByTag(Tag_GJ_TouFa1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_TouFa1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_TouFa2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_TouFa2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_TouFa3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_TouFa3);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_TouFa);// 获得当前类型所有衣服
            
            if (index == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr1->setTag(Tag_GJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr2->setTag(Tag_GJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr1->setTag(Tag_GJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr2->setTag(Tag_GJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr3->setTag(Tag_GJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
        case Tag_GJ_WaiTao:{
            if (_ManSpr->getChildByTag(Tag_GJ_WaiTao1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaiTao1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_WaiTao2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaiTao2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_WaiTao3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaiTao3);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_WaiTao);// 获得当前类型所有衣服
            
            if (index == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%ld.png", index);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _wtSpr1->setTag(Tag_GJ_WaiTao1);
                _ManSpr->addChild(_wtSpr1, def_z_order);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr2->setTag(Tag_GJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr3->setTag(Tag_GJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        case Tag_GJ_ShangYi:{
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi3);
            }
            CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
            _kzSpr1 = CCSprite::create(str->getCString());
            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            _kzSpr1->setTag(Tag_GJ_KuZi1);
            _ManSpr->addChild(_kzSpr1, 305);
            
            if (_ManSpr->getChildByTag(Tag_GJ_ShangYi1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_ShangYi1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_ShangYi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_ShangYi2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_ShangYi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_ShangYi2);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_ShangYi);// 获得当前类型所有衣服
            
            if (index == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%ld.png", index);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _sySpr1->setTag(Tag_GJ_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr1->setTag(Tag_GJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr2->setTag(Tag_GJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr3->setTag(Tag_GJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        case Tag_GJ_KuZi:{
            this->reductionShangyi();
            
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_KuZi3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_KuZi3);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_KuZi);// 获得当前类型所有衣服
            
            if (index == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _kzSpr1->setTag(Tag_GJ_KuZi1);
                _ManSpr->addChild(_kzSpr1, 305);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr1->setTag(Tag_GJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr2->setTag(Tag_GJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr3->setTag(Tag_GJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        case Tag_GJ_WaZi:{
            if (_ManSpr->getChildByTag(Tag_GJ_WaZi1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaZi1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_WaZi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaZi2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_WaZi3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_WaZi3);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_WaZi);// 获得当前类型所有衣服
            
            if (index == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _wzSpr1->setTag(Tag_GJ_WaZi1);
                _ManSpr->addChild(_wzSpr1, def_z_order);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr1->setTag(Tag_GJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr2->setTag(Tag_GJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr3->setTag(Tag_GJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        case Tag_GJ_XieZi:{
            if (_ManSpr->getChildByTag(Tag_GJ_XieZi1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_XieZi1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_XieZi2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_XieZi2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_XieZi3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_XieZi3);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_XieZi);// 获得当前类型所有衣服
            
            if (index == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _xzSpr1->setTag(Tag_GJ_XieZi1);
                _ManSpr->addChild(_xzSpr1, def_z_order);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr1->setTag(Tag_GJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr2->setTag(Tag_GJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr3->setTag(Tag_GJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        case Tag_GJ_ShiPin:{
            
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_ShiPin);// 获得当前类型所有衣服
            int sub_part;
            for (int i = 0; i < clothesArr->count(); i++) {
                CCDictionary* tempDic = (CCDictionary* )clothesArr->objectAtIndex(i);
                int temp_clothes_Id = tempDic->valueForKey("id")->intValue();
                if (changClothesIndex == 0) {
                    if (temp_clothes_Id == index) {
                        sub_part = tempDic->valueForKey("sub_part")->intValue();
                        break;
                    }
                }else{
                    if (temp_clothes_Id == changClothesIndex) {
                        sub_part = tempDic->valueForKey("sub_part")->intValue();
                        break;
                    }
                }
            }
            
            this->reductionShipin(sub_part);
            
            if (_ManSpr->getChildByTag(sub_part + 1000) != NULL) {
                _ManSpr->removeChildByTag(sub_part + 1000);
            }
            if (_ManSpr->getChildByTag(sub_part + 2000) != NULL) {
                _ManSpr->removeChildByTag(sub_part + 2000);
            }
            if (_ManSpr->getChildByTag(sub_part + 3000) != NULL) {
                _ManSpr->removeChildByTag(sub_part + 3000);
            }
            
            
            if (index == 70000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _spSpr1->setTag(sub_part + 1000);
                _ManSpr->addChild(_spSpr1, def_z_order);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                            CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                            _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr1->setTag(sub_part + 1000);
                            _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                            _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr2->setTag(sub_part + 2000);
                            _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                            _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr3->setTag(sub_part + 3000);
                            _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }

            break;
        }
        case Tag_GJ_Bao:{
            if (_ManSpr->getChildByTag(Tag_GJ_Bao1) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_Bao1);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_Bao2) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_Bao2);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_Bao3) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_Bao3);
            }
            if (_ManSpr->getChildByTag(Tag_GJ_Bao4) != NULL) {
                _ManSpr->removeChildByTag(Tag_GJ_Bao4);
            }
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_Bao);// 获得当前类型所有衣服
            
            if (index == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _bSpr1->setTag(Tag_GJ_Bao1);
                _ManSpr->addChild(_bSpr1, def_z_order);
            }else{
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == index) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        const CCString* layer4 =  clothDic->valueForKey("layer4");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr1->setTag(Tag_GJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr2->setTag(Tag_GJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr3->setTag(Tag_GJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        
                        if (layer4->compare("") != 0) {
                            CCString* str4 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer4")->intValue());
                            _bSpr4 = CCSprite::create(str4->getCString());
                            _bSpr4->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr4->setTag(Tag_GJ_Bao4);
                            _ManSpr->addChild(_bSpr4, clothDic->valueForKey("z_order4")->intValue());
                        }
                        break;
                    }
                }
            }
            break;
        }
            
        default:
            break;
    }
}
void ClothesScene::reductionShangyi(){
    if (_ManSpr->getChildByTag(Tag_GJ_ShangYi1) != NULL) {
        _ManSpr->removeChildByTag(Tag_GJ_ShangYi1);
    }
    if (_ManSpr->getChildByTag(Tag_GJ_ShangYi2) != NULL) {
        _ManSpr->removeChildByTag(Tag_GJ_ShangYi2);
    }
    if (_ManSpr->getChildByTag(Tag_GJ_ShangYi2) != NULL) {
        _ManSpr->removeChildByTag(Tag_GJ_ShangYi2);
    }
    CCInteger* cloth_id;
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    cloth_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", Tag_GJ_ShangYi)->getCString()); // 男宠当前所穿上衣
    
    if (cloth_id->getValue() == 30000) {
        CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
        _sySpr1 = CCSprite::create(str->getCString());
        _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        _sySpr1->setTag(Tag_GJ_ShangYi1);
        _ManSpr->addChild(_sySpr1, 350);
    }else{
        CCDictionary* dic = CONFIG->clothes();// 所有衣服
        CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_GJ_ShangYi);// 获得当前类型所有衣服
        for (int j = 0; j < clothesArr->count(); j++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
            int now_clothes_Id = clothDic->valueForKey("id")->intValue();
            if (now_clothes_Id == cloth_id->getValue()) {
                const CCString* layer1 =  clothDic->valueForKey("layer1");
                const CCString* layer2 =  clothDic->valueForKey("layer2");
                const CCString* layer3 =  clothDic->valueForKey("layer3");
                if (layer1->compare("") != 0) {
                    CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                    _sySpr1 = CCSprite::create(str1->getCString());
                    _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                    _sySpr1->setTag(Tag_GJ_ShangYi1);
                    _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                }
                
                if (layer2->compare("") != 0) {
                    CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                    _sySpr2 = CCSprite::create(str2->getCString());
                    _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                    _sySpr2->setTag(Tag_GJ_ShangYi2);
                    _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                }
                
                if (layer3->compare("") != 0) {
                    CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                    _sySpr3 = CCSprite::create(str3->getCString());
                    _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                    _sySpr3->setTag(Tag_GJ_ShangYi3);
                    _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                }
                break;
            }
        }
    }
}
void ClothesScene::reductionShipin(int index){
    if (index == 13 || index == 14) {
        if (_ManSpr->getChildByTag(20 + 1000) != NULL) {
            _ManSpr->removeChildByTag(20 + 1000);
        }
        if (_ManSpr->getChildByTag(20 + 2000) != NULL) {
            _ManSpr->removeChildByTag(20 + 2000);
        }
        if (_ManSpr->getChildByTag(20 + 3000) != NULL) {
            _ManSpr->removeChildByTag(20 + 3000);
        }
        CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
        CCSprite* _spSpr1 = CCSprite::create(str->getCString());
        _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        _spSpr1->setTag(20 + 1000);
        _ManSpr->addChild(_spSpr1, def_z_order);
    }else if (index == 20){
        for (int i = 13; i <= 14; i++) {
            if (_ManSpr->getChildByTag(i + 1000) != NULL) {
                _ManSpr->removeChildByTag(i + 1000);
            }
            if (_ManSpr->getChildByTag(i + 2000) != NULL) {
                _ManSpr->removeChildByTag(i + 2000);
            }
            if (_ManSpr->getChildByTag(i + 3000) != NULL) {
                _ManSpr->removeChildByTag(i + 3000);
            }
            
            CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
            CCSprite* _spSpr1 = CCSprite::create(str->getCString());
            _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            _spSpr1->setTag(i + 1000);
            _ManSpr->addChild(_spSpr1, def_z_order);
        }
    }
}

void ClothesScene::buttonStatus(){
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp(); // 临时数组
    
    for (int i = Tag_GJ_TouFa; i <= Tag_GJ_Bao; i++) {
        if (i == Tag_GJ_TouFa) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 10000) {
                ycSpr1->setVisible(false);
            }else{
                ycSpr1->setVisible(true);
            }
        }
        if (i == Tag_GJ_WaiTao) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 20000) {
                ycSpr2->setVisible(false);
            }else{
                ycSpr2->setVisible(true);
            }
        }
        if (i == Tag_GJ_ShangYi) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 30000) {
                ycSpr3->setVisible(false);
            }else{
                ycSpr3->setVisible(true);
            }
        }
        if (i == Tag_GJ_KuZi) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 40000) {
                ycSpr4->setVisible(false);
            }else{
                ycSpr4->setVisible(true);
            }
        }
        if (i == Tag_GJ_WaZi) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 50000) {
                ycSpr5->setVisible(false);
            }else{
                ycSpr5->setVisible(true);
            }
        }
        if (i == Tag_GJ_XieZi) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 60000) {
                ycSpr6->setVisible(false);
            }else{
                ycSpr6->setVisible(true);
            }
        }
        if (i == Tag_GJ_ShiPin) {
            bool shipinBool = false;
            CCDictionary* tempDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());
            for (int j = 11; j <= 19; j++) {
                CCInteger* temp_id = ((CCInteger*)tempDic->objectForKey(CCString::createWithFormat("%d", j)->getCString()));
                if (temp_id->getValue() != 70000) {
                    shipinBool = true;
                }
            }
            if (!shipinBool) {
                ycSpr7->setVisible(false);
            }else{
                ycSpr7->setVisible(true);
            }
        }
        if (i == Tag_GJ_Bao) {
            CCInteger* clothesTemp_id = ((CCInteger*)clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()));
            if (clothesTemp_id->getValue() == 80000) {
                ycSpr8->setVisible(false);
            }else{
                ycSpr8->setVisible(true);
            }
        }
    }
}

void ClothesScene::Http_Finished_401(cocos2d::CCObject *pObj) {
    
    if (clothesStatus == 1) {// 任务
        if (startTask) {
            NET->start_mission_601(task_index + 1);
        }else{
            startTask = false;
            LOADING->remove();
        }
    }else if (clothesStatus == 2){// 换装
        LOADING->remove();
    }
}

void ClothesScene::Http_Finished_601(CCObject* pObj){
    int tili = DATA->getPlayer()->energy;
    tili_AllIndex = 9;
    if (tili >= tili_AllIndex) {
        NET->commit_mission_602();
    }else{
        LOADING->remove();
        AHMessageBox* mb = AHMessageBox::create_with_message("体力不够,是否购买体力.", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
        mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
    }
}

void ClothesScene::Http_Finished_602(CCObject* pObj){
    LOADING->remove();
    
    CCScene* scene = TaskScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}


void ClothesScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    box->animation_out();
    
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
#warning 需要添加购买体力协议
            
        }
    }else if (button_type == AH_BUTTON_TYPE_YESNO2){
        if (button_tag == AH_BUTTON_TAG_YES) {
#warning 需要添加购买钻石协议
            
        }
    }else if (button_type == AH_BUTTON_TYPE_YESNO3){
        if (button_tag == AH_BUTTON_TAG_YES) {
#warning 需要添加购买金币协议
            
        }
    }
}

int ClothesScene::haveEnoughCoin(){
    int coin = 0;
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    CCDictionary* myClothesTempDic = DATA->getClothes()->MyClothesTemp();
    
    for (int i = Tag_GJ_TouFa; i < Tag_GJ_Bao; i++) {
        CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        for (int j = 0; j < clothesArr->count(); j++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        for (int k = 0; k < tempArr->count(); k++) {
            CCDictionary* dic = (CCDictionary* )tempArr->objectAtIndex(k);
            CCInteger* clothesTemp_id;
            CCDictionary* shipinDic;
            if (i != Tag_GJ_ShiPin) {
                clothesTemp_id = (CCInteger* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());
                if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 1) {
                        if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                            coin += dic->valueForKey("cost")->intValue();
                        }
                    }
                }
            }else{
                shipinDic = (CCDictionary* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
                CCInteger* clothesTemp_id;
                for (int n = 11; n <= 19; n++) {
                    clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                    if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                        int cloth_type = dic->valueForKey("type")->intValue();
                        if (cloth_type == 1) {
                            if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                                coin += dic->valueForKey("cost")->intValue();
                            }
                        }
                    }
                }
            }
        }
    }
    return coin;
}

int ClothesScene::haveEnoughGold(){
    int gold = 0;
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
        
    CCDictionary* myClothesTempDic = DATA->getClothes()->MyClothesTemp();
    
    for (int i = Tag_GJ_TouFa; i < Tag_GJ_Bao; i++) {
        CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        for (int j = 0; j < clothesArr->count(); j++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
            int sale = clothDic->valueForKey("sale")->intValue();
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
        for (int k = 0; k < tempArr->count(); k++) {
            CCDictionary* dic = (CCDictionary* )tempArr->objectAtIndex(k);
            CCInteger* clothesTemp_id;
            CCDictionary* shipinDic;
            if (i != Tag_GJ_ShiPin) {
                clothesTemp_id = (CCInteger* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());
                if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 2) {
                        if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                            gold += dic->valueForKey("cost")->intValue();
                        }
                    }
                }
            }else{
                shipinDic = (CCDictionary* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
                CCInteger* clothesTemp_id;
                for (int n = 11; n <= 19; n++) {
                    clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                    if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                        int cloth_type = dic->valueForKey("type")->intValue();
                        if (cloth_type == 2) {
                            if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                                gold += dic->valueForKey("cost")->intValue();
                            }
                        }
                    }
                }
            }
        }
    }
    return gold;
}


void ClothesScene::setShipinTag1(int index, CCSprite* spr){
    if (index == 11) {
        spr->setTag(Tag_GJ_ShiPin1_1);
    }else if (index == 12){
        spr->setTag(Tag_GJ_ShiPin1_2);
    }else if (index == 13){
        spr->setTag(Tag_GJ_ShiPin1_3);
    }else if (index == 14){
        spr->setTag(Tag_GJ_ShiPin1_4);
    }else if (index == 15){
        spr->setTag(Tag_GJ_ShiPin1_5);
    }else if (index == 16){
        spr->setTag(Tag_GJ_ShiPin1_6);
    }else if (index == 17){
        spr->setTag(Tag_GJ_ShiPin1_7);
    }else if (index == 18){
        spr->setTag(Tag_GJ_ShiPin1_8);
    }else if (index == 19){
        spr->setTag(Tag_GJ_ShiPin1_9);
    }
}
void ClothesScene::setShipinTag2(int index, CCSprite* spr){
    if (index == 11) {
        spr->setTag(Tag_GJ_ShiPin2_1);
    }else if (index == 12){
        spr->setTag(Tag_GJ_ShiPin2_2);
    }else if (index == 13){
        spr->setTag(Tag_GJ_ShiPin2_3);
    }else if (index == 14){
        spr->setTag(Tag_GJ_ShiPin2_4);
    }else if (index == 15){
        spr->setTag(Tag_GJ_ShiPin2_5);
    }else if (index == 16){
        spr->setTag(Tag_GJ_ShiPin2_6);
    }else if (index == 17){
        spr->setTag(Tag_GJ_ShiPin2_7);
    }else if (index == 18){
        spr->setTag(Tag_GJ_ShiPin2_8);
    }else if (index == 19){
        spr->setTag(Tag_GJ_ShiPin2_9);
    }
}
void ClothesScene::setShipinTag3(int index, CCSprite* spr){
    if (index == 11) {
        spr->setTag(Tag_GJ_ShiPin3_1);
    }else if (index == 12){
        spr->setTag(Tag_GJ_ShiPin3_2);
    }else if (index == 13){
        spr->setTag(Tag_GJ_ShiPin3_3);
    }else if (index == 14){
        spr->setTag(Tag_GJ_ShiPin3_4);
    }else if (index == 15){
        spr->setTag(Tag_GJ_ShiPin3_5);
    }else if (index == 16){
        spr->setTag(Tag_GJ_ShiPin3_6);
    }else if (index == 17){
        spr->setTag(Tag_GJ_ShiPin3_7);
    }else if (index == 18){
        spr->setTag(Tag_GJ_ShiPin3_8);
    }else if (index == 19){
        spr->setTag(Tag_GJ_ShiPin3_9);
    }
}













