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


ClothesScene::ClothesScene(){
    
}
ClothesScene::~ClothesScene(){
    
}

bool ClothesScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    buttonTag = 0;
    
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
    CCSprite* startSpr1 = CCSprite::create("res/pic/clothesScene/button/gj_start.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/clothesScene/button/gj_start.png");
    startSpr2->setScale(1.02f);
    CCMenuItem* startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(ClothesScene::startCallBack));
    startItem->setAnchorPoint(ccp(1.f, .5f));
    startItem->setPosition(ccp(DISPLAY->ScreenWidth() - backItem->getContentSize().width - startItem->getContentSize().width* .95f, DISPLAY->ScreenHeight()* .032f));
    
    CCMenu* menu = CCMenu::create(backItem, startItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 15);
    
    this->creat_View();
    this->creat_Man();
    this->initClothes();
    
    
    return true;
}

void ClothesScene::onEnter(){
    
    
    BaseScene::onEnter();
}

void ClothesScene::onExit(){
    BaseScene::onExit();
    
}

void ClothesScene::keyBackStatus(float dt){
    
}
void ClothesScene::keyBackClicked(){
    
}

bool ClothesScene::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    
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
    
    CCMenu* menu = CCMenu::create(tfItem, wtItem, syItem, spItem, kzItem, wzItem, xzItem, baoItem, NULL);
    menu->alignItemsVerticallyWithPadding(18.f);
    menu->setPosition(ccp(- tfItem->getContentSize().width* .5f, clothKuangSpr->getContentSize().height* .55f));
    menu->setTag(0x3333);
    clothKuangSpr->addChild(menu, 5);
    
//    MMShangChengTableView* tabLayer = MMShangChengTableView::create();
//    tabLayer->setPosition(ccp(0, 0));
//    isClothesType = Tag_GJ_WaiTao;
//    _scDelegate = tabLayer;
//    tabLayer->setTag(0x77777);
//    bgkSpr->addChild(tabLayer, 5);
    
    
//    if (buttonTag == Tag_GJ_WaiTao){
//        
//        isClothesType = Tag_GJ_WaiTao;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_WaiTao);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_WaiTao);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_WaiTao);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }else if (buttonTag == Tag_GJ_ShangYi){
//        
//        isClothesType = Tag_GJ_ShangYi;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_ShangYi);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_ShangYi);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_ShangYi);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }else if (buttonTag == Tag_GJ_KuZi){
//        
//        isClothesType = Tag_GJ_KuZi;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_KuZi);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_KuZi);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_KuZi);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }else if (buttonTag == Tag_GJ_WaZi){
//        
//        isClothesType = Tag_GJ_WaZi;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_WaZi);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_WaZi);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_WaZi);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }else if (buttonTag == Tag_GJ_XieZi){
//        
//        isClothesType = Tag_GJ_XieZi;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_XieZi);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_XieZi);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_XieZi);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }else if (buttonTag == Tag_GJ_ShiPin){
//        
//        isClothesType = Tag_GJ_ShiPin;
//        
//        ClothesComponent* clothesComponent = MMDataManager::get_instance()->getClothescomponent();
//        CCArray* _dataArr = clothesComponent->clothes_template_part(Tag_GJ_ShiPin);
//        CCArray* tempArr = CCArray::create();
//        
//        for (int i = 0; i < _dataArr->count(); i++) {
//            ClothTemplateItem* item = (ClothTemplateItem* )_dataArr->objectAtIndex(i);
//            if (scOrBlBool) {// false商城 true布料
//                if (item->getAccess_type() == 11) {
//                    
//                }else{
//                    continue;
//                }
//            }else{
//                if (item->getAccess_type() != 11) {
//                    
//                }else{
//                    continue;
//                }
//            }
//            if (item->getBelong() == 0 || item->getBelong() == MMDataManager::get_instance()->get_current_manpet_id()) {
//                tempArr->addObject(item);
//            }
//        }
//        MMDataManager::get_instance()->setDataSource(tempArr);
//        if (scOrBlBool) {// false商城 true布料
//            _blDelegate->updateTableView(Tag_GJ_ShiPin);
//        }else{
//            _scDelegate->updateTableView(Tag_GJ_ShiPin);
//        }
//        
//        if (_inClothesLayerBool) {
//            this->clothingNameInformation(NULL);
//        }
//        
//    }
}
void ClothesScene::buttonCallBack(CCObject* pSender){
    
}
void ClothesScene::backCallBack(CCObject* pSender){
    CCScene* scene = TaskScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void ClothesScene::startCallBack(CCObject* pSender){
    
}
void ClothesScene::buyCallBack(CCObject* pSender){
    
}
void ClothesScene::creat_Man(){
    
    CCString* _tfStr1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/10000.png");
    _tfSpr1 = CCSprite::create(_tfStr1->getCString());
    _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(_tfSpr1, 100);
    
    CCString* _tfStr2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/10009.png");
    _tfSpr2 = CCSprite::create(_tfStr2->getCString());
    _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(_tfSpr2, 30);
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(manSpr, 50);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    _ManSpr->addChild(_touSpr, 51);
}
void ClothesScene::initClothes(){//穿衣服
    CCDictionary* dress = DATA->getShow()->ondress(); // 男宠衣着
    CCString* str = NULL;
    
    for (int i = Tag_GJ_TouFa; i <= Tag_GJ_Bao; i++) {
        if (i == Tag_GJ_TouFa) {
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str->getCString());
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr1->setTag(Tag_GJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 100);
                
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str->getCString());
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _tfSpr2->setTag(Tag_GJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 30);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _tfSpr1 = CCSprite::create(str->getCString());
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr1->setTag(Tag_GJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _tfSpr3 = CCSprite::create(str->getCString());
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr3->setTag(Tag_GJ_TouFa2);
                            _ManSpr->addChild(_tfSpr3, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _tfSpr2 = CCSprite::create(str->getCString());
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _tfSpr2->setTag(Tag_GJ_TouFa3);
                            _ManSpr->addChild(_tfSpr2, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaiTao){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", cloth_id->getValue());
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _wtSpr1 = CCSprite::create(str->getCString());
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _wtSpr2 = CCSprite::create(str->getCString());
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr2->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _wtSpr3 = CCSprite::create(str->getCString());
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wtSpr3->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShangYi){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", cloth_id->getValue());
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _sySpr1->setTag(Tag_GJ_ShangYi1);
                _ManSpr->addChild(_sySpr1, def_z_order);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _sySpr1 = CCSprite::create(str->getCString());
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_sySpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _sySpr2 = CCSprite::create(str->getCString());
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr2->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_sySpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _sySpr3 = CCSprite::create(str->getCString());
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _sySpr3->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_sySpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_KuZi){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _kzSpr1->setTag(Tag_GJ_KuZi1);
                _ManSpr->addChild(_kzSpr1, def_z_order);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _kzSpr1 = CCSprite::create(str->getCString());
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr1->setTag(Tag_GJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _kzSpr2 = CCSprite::create(str->getCString());
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr2->setTag(Tag_GJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _kzSpr3 = CCSprite::create(str->getCString());
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _kzSpr3->setTag(Tag_GJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaZi){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _wzSpr1 = CCSprite::create(str->getCString());
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr1->setTag(Tag_GJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _wzSpr2 = CCSprite::create(str->getCString());
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr2->setTag(Tag_GJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _wzSpr3 = CCSprite::create(str->getCString());
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _wzSpr3->setTag(Tag_GJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_XieZi){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 50000);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _xzSpr1 = CCSprite::create(str->getCString());
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr1->setTag(Tag_GJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _xzSpr2 = CCSprite::create(str->getCString());
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr2->setTag(Tag_GJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _xzSpr3 = CCSprite::create(str->getCString());
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _xzSpr3->setTag(Tag_GJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShiPin){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 50000);
                _spSpr1 = CCSprite::create(str->getCString());
                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                _spSpr1->setTag(Tag_GJ_ShiPin1);
                _ManSpr->addChild(_spSpr1, def_z_order);
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
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _spSpr1 = CCSprite::create(str->getCString());
                            _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr1->setTag(Tag_GJ_ShiPin1);
                            _ManSpr->addChild(_spSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _spSpr2 = CCSprite::create(str->getCString());
                            _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr2->setTag(Tag_GJ_ShiPin2);
                            _ManSpr->addChild(_spSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _spSpr3 = CCSprite::create(str->getCString());
                            _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _spSpr3->setTag(Tag_GJ_ShiPin3);
                            _ManSpr->addChild(_spSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_Bao){
            str = NULL;
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 50000);
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
                        if (layer1->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png",((CCInteger* )clothDic->valueForKey("layer1"))->getValue());
                            _bSpr1 = CCSprite::create(str->getCString());
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr1->setTag(Tag_GJ_Bao1);
                            _ManSpr->addChild(_bSpr1, ((CCInteger* )clothDic->valueForKey("z_order1"))->getValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png",((CCInteger* )clothDic->valueForKey("layer2"))->getValue());
                            _bSpr2 = CCSprite::create(str->getCString());
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr2->setTag(Tag_GJ_Bao2);
                            _ManSpr->addChild(_bSpr2, ((CCInteger* )clothDic->valueForKey("z_order2"))->getValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png",((CCInteger* )clothDic->valueForKey("layer3"))->getValue());
                            _bSpr3 = CCSprite::create(str->getCString());
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
                            _bSpr3->setTag(Tag_GJ_Bao3);
                            _ManSpr->addChild(_bSpr3, ((CCInteger* )clothDic->valueForKey("z_order3"))->getValue());
                        }
                        break;
                    }
                }
            }
        }
    }

    this->scheduleOnce(SEL_SCHEDULE(&ClothesScene::openTouch), 1.f);
}































