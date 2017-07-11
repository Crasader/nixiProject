//
//  NXClothesShop.cpp
//  tiegao
//
//  Created by mac on 17-7-8.
//
//

#include "NXClothesShop.h"
#include "NXClothesScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "SpecialManager.h"
#include "NXMainScene.h"
#include "MZResourceLoader.h"
#include "ConfigManager.h"
#include "TaskScene.h"
#include "HomeLayer.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"
#include "GuideLayer.h"
#include "StringUtil.h"
#include "JNIController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ShareManager.h"
#endif






NXClothesShop::NXClothesShop(){
    
}
NXClothesShop::~NXClothesShop(){
    
}

NXClothesShop* NXClothesShop::create_with_type(int _type_id, int _task_index, int _task_phase, const char *category, const char *tishi){
    NXClothesShop* rtn = NXClothesShop::create();
    rtn->init_with_type(_type_id, _task_index, _task_phase, category, tishi);
    
    return rtn;
}

void NXClothesShop::init_with_type(int _type_id, int _task_index, int _task_phase, const char *category, const char *tishi){
    
    clothesStatus = _type_id;
    this->category = category;
    this->tishi = tishi;
    task_index = _task_index;
    task_phase = _task_phase;
    
    num_child = 0;
    clothes_type = 0;// 衣服价格类型 0-all 1-coin 2-gold 3-稀有
    clothesType = Tag_NXCL_ShangYi;// 当前衣服类型
    clothes_selected = 0;// 选中的衣服
    _buttonStatus = 0;
}

bool NXClothesShop::init(){
    if (!NXBaseScene::init()) {
        return false;
    }
    
    for (int i = 0; i < ClothesShop_MAXNUM; i++) {
        isSelect[i] = false;
    }
    
    this->setTouchSwallowEnabled(false);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    
    tuziSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_tuzi.png");
    tuziSpr->retain();
    
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    CCArray* clothesArr = (CCArray* )dic->objectForKey(Tag_NXCL_ShangYi);// 获得当前类型所有衣服
    CCArray* tempArr = CCArray::create();
    for (int i = 0; i < clothesArr->count(); i++) {
        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
        int id = clothDic->valueForKey("id")->intValue();
        if (DATA->getClothes()->is_owned(Tag_NXCL_ShangYi, id)) {
        }else{
            tempArr->addObject(clothDic);
        }
        
    }
    DATA->setDataSource(tempArr);
    
    
    this->creat_view();
    this->craet_button();
    
    this->scheduleOnce(SEL_SCHEDULE(&NXClothesShop::show_clothesInfo), .7f);
    
    return true;
}
void NXClothesShop::show_clothesInfo(float dt){
    CCArray* arr = DATA->getDataSource();
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex(clothes_selected);
    int cloth_id = dic->valueForKey("id")->intValue(); // 男宠当前所穿上衣
    CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothes", (CCObject* )cloth_id);
    CCNotificationCenter::sharedNotificationCenter()->postNotification("creat_clothesInfo", (CCObject* )clothes_selected);
}

void NXClothesShop::onEnter(){
    NXBaseScene::onEnter();
    NXBaseScene::openChat(false);
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(NXClothesShop::creat_clothes), "creat_clothes", NULL);
    nc->addObserver(this, menu_selector(NXClothesShop::creat_clothesInfo), "creat_clothesInfo", NULL);
    nc->addObserver(this, menu_selector(NXClothesShop::Http_Finished_401), "HTTP_FINISHED_401", NULL);
    nc->addObserver(this, menu_selector(NXClothesShop::Http_Finished_401), "HTTP_FINISHED_403", NULL);
    nc->addObserver(this, menu_selector(NXClothesShop::Http_Finished_401), "HTTP_FINISHED_405", NULL);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&NXClothesShop::keyBackStatus), .8f);
}
void NXClothesShop::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void NXClothesShop::keyBackClicked(){
    num_child++;
    CCLog("===== NXClothesScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
//    this->backCallBack(NULL);
}
void NXClothesShop::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    NXBaseScene::onExit();
}

cocos2d::CCScene* NXClothesShop::scene(){
    CCScene* scene = CCScene::create();
    NXClothesShop* layer = NXClothesShop::create();
    scene->addChild(layer);
    
    return scene;
}

void NXClothesShop::creat_view(){
    bgSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_bj.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    CCSprite* backSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(NXClothesShop::backCallBack));
    backItem->setAnchorPoint(CCPointZero);
    backItem->setPosition(ccp(-7.f, 10.f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 30);
    
    //
    toggleItem[0] = CCMenuItemToggle::createWithTarget(this, menu_selector(NXClothesShop::onSelect), CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button1.png", "res/nxpic/NXClothesShop/NXClothesShop_button1.png"),CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button2.png", "res/nxpic/NXClothesShop/NXClothesShop_button2.png"),NULL);
    toggleItem[0]->setAnchorPoint(ccp(.5f, .5f));
    toggleItem[0]->setTag(0);
    CCLabelTTF* allLable = CCLabelTTF::create("全部", DISPLAY->fangzhengFont(), 25);
    allLable->setPosition(ccp(toggleItem[0]->getContentSize().width* .5f, toggleItem[0]->getContentSize().height* .5f));
    allLable->setColor(ccc3(80, 63, 68));
    toggleItem[0]->addChild(allLable);
    isSelect[0] = true;
    clothes_type = 0;
    toggleItem[0]->setSelectedIndex(1);
    
    
    toggleItem[1] = CCMenuItemToggle::createWithTarget(this, menu_selector(NXClothesShop::onSelect), CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button1.png", "res/nxpic/NXClothesShop/NXClothesShop_button1.png"),CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button2.png", "res/nxpic/NXClothesShop/NXClothesShop_button2.png"),NULL);
    toggleItem[1]->setAnchorPoint(ccp(.5f, .5f));
    toggleItem[1]->setTag(1);
    CCSprite* coinSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_coin.png");
    coinSpr->setPosition(ccp(toggleItem[1]->getContentSize().width* .5f, toggleItem[1]->getContentSize().height* .5f));
    toggleItem[1]->addChild(coinSpr);
    
    
    toggleItem[2] = CCMenuItemToggle::createWithTarget(this, menu_selector(NXClothesShop::onSelect), CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button1.png", "res/nxpic/NXClothesShop/NXClothesShop_button1.png"),CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button2.png", "res/nxpic/NXClothesShop/NXClothesShop_button2.png"),NULL);
    toggleItem[2]->setAnchorPoint(ccp(.5f, .5f));
    toggleItem[2]->setTag(2);
    CCSprite* goldSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_gold.png");
    goldSpr->setPosition(ccp(toggleItem[2]->getContentSize().width* .5f, toggleItem[2]->getContentSize().height* .5f));
    toggleItem[2]->addChild(goldSpr);
    
    toggleItem[3] = CCMenuItemToggle::createWithTarget(this, menu_selector(NXClothesShop::onSelect), CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button1.png", "res/nxpic/NXClothesShop/NXClothesShop_button1.png"),CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_button2.png", "res/nxpic/NXClothesShop/NXClothesShop_button2.png"),NULL);
    toggleItem[3]->setAnchorPoint(ccp(.5f, .5f));
    toggleItem[3]->setTag(3);
    CCSprite* xiyouSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_xiyou.png");
    xiyouSpr->setPosition(ccp(toggleItem[3]->getContentSize().width* .5f, toggleItem[3]->getContentSize().height* .5f));
    toggleItem[3]->addChild(xiyouSpr);
    
    CCMenu* toggleMenu = CCMenu::create(toggleItem[0], toggleItem[1], toggleItem[2], toggleItem[3], NULL);
    toggleMenu->setAnchorPoint(CCPointZero);
    toggleMenu->alignItemsHorizontallyWithPadding(0.f);
    toggleMenu->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .37f));
    toggleMenu->setTag(0x88888);
    this->addChild(toggleMenu, 30);
    
    
    NXClothesShopTableview* layer = NXClothesShopTableview::create();
    layer->setPosition(CCPointZero);
    _delegate = layer;
    this->addChild(layer, 20);
    
    
    
}
void NXClothesShop::onSelect(CCObject* pSender){
    if (animationBool) {
        animationBool = false;
        
        this->removeAllSpr();
    }
    
    int tag = ((CCMenuItemImage*) pSender)->getTag();
    
    for (int i = 0; i < ClothesShop_MAXNUM; i++) {
        if (i == tag) {
//            if (isSelect[i]) {
//                isSelect[i] = false;
//                toggleItem[i]->setSelectedIndex(0);
//            }else if (isSelect[i] == false){
//                isSelect[i] = true;
//                toggleItem[i]->setSelectedIndex(1);
//            }
            if (isSelect[i]) {
                isSelect[i] = true;
                toggleItem[i]->setSelectedIndex(1);
                clothes_type = i;
            }else if (isSelect[i] == false){
                isSelect[i] = true;
                toggleItem[i]->setSelectedIndex(1);
                clothes_type = i;
                
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
                CCArray* tempArr = CCArray::create();
                
                for (int i = 0; i < clothesArr->count(); i++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
                    int id = clothDic->valueForKey("id")->intValue();
                    if (DATA->getClothes()->is_owned(clothesType, id)) {
                    }else{
                        if (clothes_type == 0) {
                            tempArr->addObject(clothDic);
                        }else if (clothes_type == 1){
                            int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                            if (cloth_type == 1) {
                                tempArr->addObject(clothDic);
                            }
                        }else if (clothes_type == 2){
                            int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                            if (cloth_type == 2){
                                tempArr->addObject(clothDic);
                            }
                        }else if (clothes_type == 3 || clothes_type == 10){
                            int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                            if (cloth_type != 1 && cloth_type != 2){
                                tempArr->addObject(clothDic);
                            }
                        }
                    }
                }
                DATA->setDataSource(tempArr);
                _delegate->updateTableView(clothesType);
                
                CCLog("clothes_type == %d", clothes_type);
            }
        }else{
            isSelect[i] = false;
            toggleItem[i]->setSelectedIndex(0);
        }
    }
    
}
void NXClothesShop::backCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    
    DATA->setClothesBool(true);
    
    if (clothesStatus == 5) {
        DATA->getClothes()->copy_clothesTemp(DATA->getCompetition()->getSelf()->getOndress());
    }
    else {
        DATA->getClothes()->copy_clothesTemp(DATA->getShow()->ondress());
    }
    
    if (animationBool) {
        animationBool = false;
        
        this->removeAllSpr();
    }
    
    if (clothesStatus == 1) {// 任务
        CCLayer* layer = NXClothesScene::create_with_type(1, task_index, task_phase);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else if (clothesStatus == 2){// 换装
        CCLayer* layer = NXClothesScene::create_with_type(2, 0, 0);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else if (clothesStatus == 6){// 换装
        CCLayer* layer = NXClothesScene::create_with_type(6, 0, 0);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else if (clothesStatus == 3){// 神秘事件
        CCLayer* layer = NXClothesScene::create_with_mystery(3,this->category, this->tishi);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else if (clothesStatus == 4){// 约会
        CCLayer* layer = NXClothesScene::create_with_tryst(this->tishi);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else if (clothesStatus == 5){// pk
        CCLayer* layer = NXClothesScene::create_with_type(5, 0, 0);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void NXClothesShop::craet_button(){
    
    CCSprite* diSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_di.png");
    diSpr->setAnchorPoint(ccp(.5f, 0.f));
    diSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .37f + toggleItem[0]->getContentSize().height* .5f));
    this->addChild(diSpr, 30);
    
    int index = 0;
    float indexFolat = 0.f;
    CCSprite* sySpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button01.png");
    CCSprite* sySpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button01.png");
    sySpr2->setScale(1.02f);
    CCMenuItem* syItem = CCMenuItemSprite::create(sySpr1, sySpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    syItem->setAnchorPoint(CCPointZero);
    syItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .125f));
    syItem->setTag(Tag_NXCL_ShangYi);
    tuziSpr->setAnchorPoint(ccp(.5f, 0));
    tuziSpr->setPosition(ccp(syItem->getContentSize().width* .5f, syItem->getContentSize().height + 1.f));
    syItem->addChild(tuziSpr);
    index++;
    
    
    CCSprite* hzSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button02.png");
    CCSprite* hzSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button02.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* kzItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    kzItem->setAnchorPoint(CCPointZero);
    kzItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .25f));
    kzItem->setTag(Tag_NXCL_KuZi);
    index++;
    
    
    CCSprite* wtSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button03.png");
    CCSprite* wtSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button03.png");
    wtSpr2->setScale(1.02f);
    CCMenuItem* wtItem = CCMenuItemSprite::create(wtSpr1, wtSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    wtItem->setAnchorPoint(CCPointZero);
    wtItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .375f));
    wtItem->setTag(Tag_NXCL_WaiTao);
    index++;
    
    CCSprite* xzSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button04.png");
    CCSprite* xzSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button04.png");
    xzSpr2->setScale(1.02f);
    CCMenuItem* xzItem = CCMenuItemSprite::create(xzSpr1, xzSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    xzItem->setAnchorPoint(CCPointZero);
    xzItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .5f));
    xzItem->setTag(Tag_NXCL_XieZi);
    index++;
    
    
    CCSprite* txSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button05.png");
    CCSprite* txSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button05.png");
    txSpr2->setScale(1.02f);
    CCMenuItem* txItem = CCMenuItemSprite::create(txSpr1, txSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    txItem->setAnchorPoint(CCPointZero);
    txItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .625f));
    txItem->setTag(Tag_NXCL_TeXiao);
    index++;
    
    
    CCSprite* tfSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button06.png");
    CCSprite* tfSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button06.png");
    tfSpr2->setScale(1.02f);
    CCMenuItem* tfItem = CCMenuItemSprite::create(tfSpr1, tfSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    tfItem->setAnchorPoint(CCPointZero);
    tfItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .75f));
    tfItem->setTag(Tag_NXCL_TouFa);
    index++;
    
    
    CCSprite* spSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button07.png");
    CCSprite* spSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button07.png");
    spSpr2->setScale(1.02f);
    CCMenuItem* spItem = CCMenuItemSprite::create(spSpr1, spSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    spItem->setAnchorPoint(CCPointZero);
    spItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height* .875f));
    spItem->setTag(Tag_NXCL_ShiPin);
    index++;
    
    
    CCSprite* zrSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button08.png");
    CCSprite* zrSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_button08.png");
    zrSpr2->setScale(1.02f);
    CCMenuItem* zrItem = CCMenuItemSprite::create(zrSpr1, zrSpr2, this, menu_selector(NXClothesShop::buttonCallBack));
    zrItem->setAnchorPoint(CCPointZero);
    zrItem->setPosition(ccp(sySpr1->getContentSize().width* index + indexFolat * index, diSpr->getContentSize().height));
    zrItem->setTag(Tag_NXCL_ZhuangRong);
    index++;
    
    
    CCMenu* buttonMenu = CCMenu::create(syItem, kzItem, wtItem, xzItem, txItem, tfItem, spItem, zrItem, NULL);
    buttonMenu->setAnchorPoint(CCPointZero);
    buttonMenu->setPosition(CCPointZero);
    diSpr->addChild(buttonMenu);
    
}
void NXClothesShop::buttonCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    CCLog("item tag == %d", item->getTag());
    if (clothesType != item->getTag()) {
        clothesType = item->getTag();
        clothes_selected = 0;
        
        if (this->getChildByTag(0x200001) != NULL) {
            this->removeChildByTag(0x200001);
        }
        if (this->getChildByTag(0x200002) != NULL) {
            this->removeChildByTag(0x200002);
        }
        
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
        
        if (animationBool) {
            animationBool = false;
            
            this->removeAllSpr();
        }
        
        if (tuziSpr != NULL && tuziSpr->getParent() != NULL) {
            tuziSpr->removeFromParentAndCleanup(true);
            tuziSpr = NULL;
        }
        tuziSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_tuzi.png");
        tuziSpr->setAnchorPoint(ccp(.5f, 0));
        tuziSpr->setPosition(ccp(item->getContentSize().width* .5f, item->getContentSize().height + 1.f));
        item->addChild(tuziSpr);
        
        
        CCDictionary* dic = CONFIG->clothes();// 所有衣服
        CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
        CCArray* tempArr = CCArray::create();
        
        if (clothesArr != NULL) {
            for (int i = 0; i < clothesArr->count(); i++) {
                CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
                int id = clothDic->valueForKey("id")->intValue();
                if (DATA->getClothes()->is_owned(clothesType, id)) {
                }else{
                    if (clothes_type == 0) {
                        tempArr->addObject(clothDic);
                    }else if (clothes_type == 1){
                        int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                        if (cloth_type == 1) {
                            tempArr->addObject(clothDic);
                        }
                    }else if (clothes_type == 2){
                        int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                        if (cloth_type == 2){
                            tempArr->addObject(clothDic);
                        }
                    }else if (clothes_type == 3 || clothes_type == 10){
                        int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                        if (cloth_type != 1 && cloth_type != 2){
                            tempArr->addObject(clothDic);
                        }
                    }
                }
            }
            
        }
        DATA->setDataSource(tempArr);
        
        if (this->getChildByTag(0x100001) != NULL) {
            this->removeChildByTag(0x100001);
        }
        
        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
        _delegate->updateTableView(clothesType);
    }
}

void NXClothesShop::creat_clothes(CCObject* pSender){
    clothesIndex = (long)pSender;
    if (clothesIndex >= 10000 && clothesIndex < 20000){
        clothesType = Tag_NXCL_TouFa;
    }else if (clothesIndex >= 20000 && clothesIndex < 30000){
        clothesType = Tag_NXCL_WaiTao;
    }else if (clothesIndex >= 30000 && clothesIndex < 40000){
        clothesType = Tag_NXCL_ShangYi;
    }else if (clothesIndex >= 40000 && clothesIndex < 50000){
        clothesType = Tag_NXCL_KuZi;
    }else if (clothesIndex >= 50000 && clothesIndex < 60000){
        clothesType = Tag_NXCL_TeXiao;
    }else if (clothesIndex >= 60000 && clothesIndex < 70000){
        clothesType = Tag_NXCL_XieZi;
    }else if (clothesIndex >= 70000 && clothesIndex < 80000){
        clothesType = Tag_NXCL_ShiPin;
    }else if (clothesIndex >= 80000 && clothesIndex < 90000){
        clothesType = Tag_NXCL_Bao;
    }else if (clothesIndex >= 90000 && clothesIndex < 100000){
        clothesType = Tag_NXCL_ZhuangRong;
    }
    
    if (this->getChildByTag(0x100001) != NULL) {
        this->removeChildByTag(0x100001);
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    CCString* clothesStr;
    if (clothesType == Tag_NXCL_TouFa) {
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/1toufa/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_WaiTao) {
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/2waitao/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_ShangYi){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/3shangyi/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_KuZi){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/4kuzi/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_XieZi){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/6xiezi/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_ShiPin){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/7shipin/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_ZhuangRong){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/9zhuangrong/show%ld.png", clothesIndex);
    }else if (clothesType == Tag_NXCL_TeXiao){
        clothesStr = CCString::createWithFormat("res/nxpic/NXClothes/show/5texiao/show%ld.png", clothesIndex);
    }else{
        return;
    }
    CCSprite* clothesSpr = CCSprite::create(clothesStr->getCString());
    if (clothesType == Tag_NXCL_TouFa) {
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .64f));
    }else if (clothesType == Tag_NXCL_WaiTao) {
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .47f, DISPLAY->ScreenHeight()* .64f));
    }else if (clothesType == Tag_NXCL_ShangYi){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .47f, DISPLAY->ScreenHeight()* .64f));
    }else if (clothesType == Tag_NXCL_KuZi){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .47f, DISPLAY->ScreenHeight()* .64f));
    }else if (clothesType == Tag_NXCL_XieZi){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .68f));
    }else if (clothesType == Tag_NXCL_ShiPin){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .68f));
    }else if (clothesType == Tag_NXCL_TeXiao){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .68f));
    }else if (clothesType == Tag_NXCL_ZhuangRong){
        clothesSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .68f));
    }
    clothesSpr->setTag(0x100001);
    this->addChild(clothesSpr, 5);
}

void NXClothesShop::creat_clothesInfo(CCObject* pSender){
    if (this->getChildByTag(0x200001) != NULL) {
        this->removeChildByTag(0x200001);
    }
    if (this->getChildByTag(0x200002) != NULL) {
        this->removeChildByTag(0x200002);
    }
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    long index = (long)pSender;
    CCArray* arr = DATA->getDataSource();
    CCDictionary* dic = (CCDictionary* )arr->objectAtIndex((int)index);
    int cloth_id = dic->valueForKey("id")->intValue();
    const CCString* nameStr1 = dic->valueForKey("name");
    const CCString* nameStr2 = dic->valueForKey("label");
    int phase = dic->valueForKey("phase")->intValue();
    
    // 上框
    CCSprite* dikuangSpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_dikuang2.png");
    dikuangSpr1->setAnchorPoint(ccp(0.f, .5f));
    dikuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .58f, DISPLAY->ScreenHeight()* .84f));
    dikuangSpr1->setTag(0x200001);
    this->addChild(dikuangSpr1, 10);
    CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr1->getCString(), DISPLAY->fangzhengFont(), 24, CCSizeMake(180, 24), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(dikuangSpr1->getContentSize().width* .5f, dikuangSpr1->getContentSize().height* .75f));
    nameLabel->setColor(ccc3(95, 53, 68));
    dikuangSpr1->addChild(nameLabel);
    // 上框星星
    CCMenuItemImage* xingItem1 = CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_xing.png", "res/nxpic/NXClothesShop/NXClothesShop_xing.png");
    xingItem1->setPosition(CCPointZero);
    CCMenuItemImage* xingItem2 = CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_xing.png", "res/nxpic/NXClothesShop/NXClothesShop_xing.png");
    xingItem2->setPosition(CCPointZero);
    CCMenuItemImage* xingItem3 = CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_xing.png", "res/nxpic/NXClothesShop/NXClothesShop_xing.png");
    xingItem3->setPosition(CCPointZero);
    CCMenuItemImage* xingItem4 = CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_xing.png", "res/nxpic/NXClothesShop/NXClothesShop_xing.png");
    xingItem4->setPosition(CCPointZero);
    CCMenuItemImage* xingItem5 = CCMenuItemImage::create("res/nxpic/NXClothesShop/NXClothesShop_xing.png", "res/nxpic/NXClothesShop/NXClothesShop_xing.png");
    xingItem5->setPosition(CCPointZero);
    CCMenu* xingMenu;
    if (phase == 1) {
        xingMenu = CCMenu::create(xingItem1, NULL);
    }else if (phase == 2) {
        xingMenu = CCMenu::create(xingItem1, xingItem2, NULL);
    }else if (phase == 3) {
        xingMenu = CCMenu::create(xingItem1, xingItem2, xingItem3, NULL);
    }else if (phase == 4) {
        xingMenu = CCMenu::create(xingItem1, xingItem2, xingItem3, xingItem4, NULL);
    }else if (phase == 5) {
        xingMenu = CCMenu::create(xingItem1, xingItem2, xingItem3, xingItem4, NULL);
    }else{
        xingMenu = CCMenu::create(xingItem1, NULL);
    }
    xingMenu->alignItemsHorizontallyWithPadding(1.f);
    xingMenu->setPosition(ccp(dikuangSpr1->getContentSize().width* .5f, dikuangSpr1->getContentSize().height* .12f));
    dikuangSpr1->addChild(xingMenu);
    
    
    
    // 下框
    CCSprite* dikuangSpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_dikuang3.png");
    dikuangSpr2->setAnchorPoint(ccp(0.f, .5f));
    dikuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .58f, DISPLAY->ScreenHeight()* .715f));
    dikuangSpr2->setTag(0x200002);
    this->addChild(dikuangSpr2, 10);
    CCLabelTTF* infoLabel = CCLabelTTF::create(nameStr2->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(200, dikuangSpr2->getContentSize().height* .7f), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    infoLabel->setPosition(ccp(dikuangSpr2->getContentSize().width* .5f, dikuangSpr2->getContentSize().height* .35f));
    infoLabel->setColor(ccc3(95, 53, 68));
    dikuangSpr2->addChild(infoLabel);
    
    
    CCSprite* buySpr1 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_buy.png");
    CCSprite* buySpr2 = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_buy.png");
    buySpr2->setScale(1.02f);
    buyItem = CCMenuItemSprite::create(buySpr1, buySpr2, this, menu_selector(NXClothesShop::buyCallback));
    buyItem->setAnchorPoint(ccp(.5f, 1.f));
    buyItem->setPosition(ccp(dikuangSpr2->getContentSize().width* .5f, -70.f));
    buyItem->setTag(cloth_id);
    CCMenu* buyMenu = CCMenu::create(buyItem, NULL);
    buyMenu->setPosition(CCPointZero);
    dikuangSpr2->addChild(buyMenu);
    
    int cloth_type = dic->valueForKey("type")->intValue(); // 男宠当前所穿上衣
    if (cloth_type == 1) {
        const CCString* costStr1 = dic->valueForKey("cost");
        CCString* costStr2 = CCString::createWithFormat("价格:  %s", costStr1->getCString());
        CCLabelTTF* costLabel = CCLabelTTF::create(costStr2->getCString(), DISPLAY->fangzhengFont(), 28);
        costLabel->setAnchorPoint(ccp(0.f, 1.f));
        costLabel->setPosition(ccp(15, -20.f));
        costLabel->setColor(ccc3(98, 53, 68));
        dikuangSpr2->addChild(costLabel);
        
        CCSprite* coinSpr = CCSprite::create("res/nxpic/NXBaseScene/NXBase_jinbi.png");
        coinSpr->setAnchorPoint(ccp(0.f, .5f));
        coinSpr->setPosition(ccp(costLabel->getContentSize().width + 8.f, costLabel->getContentSize().height* .5f));
        costLabel->addChild(coinSpr);
    }else if (cloth_type == 2){
        const CCString* costStr1 = dic->valueForKey("cost");
        CCString* costStr2 = CCString::createWithFormat("价格:  %s", costStr1->getCString());
        CCLabelTTF* costLabel = CCLabelTTF::create(costStr2->getCString(), DISPLAY->fangzhengFont(), 28);
        costLabel->setAnchorPoint(ccp(0.f, 1.f));
        costLabel->setPosition(ccp(15, -20.f));
        costLabel->setColor(ccc3(98, 53, 68));
        dikuangSpr2->addChild(costLabel);
        
        CCSprite* goldSpr = CCSprite::create("res/nxpic/NXBaseScene/NXBase_zuanshi.png");
        goldSpr->setAnchorPoint(ccp(0.f, .5f));
        goldSpr->setPosition(ccp(costLabel->getContentSize().width + 8.f, costLabel->getContentSize().height* .5f));
        costLabel->addChild(goldSpr);
    }else if (cloth_type == 3 || clothes_type == 10){
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_xiyou.png");
        costSpr->setAnchorPoint(ccp(.5f, 1.f));
        costSpr->setPosition(ccp(dikuangSpr2->getContentSize().width* .5f, -20.f));
        dikuangSpr2->addChild(costSpr);
        
        buyItem->setVisible(false);
    }else{
        CCSprite* costSpr = CCSprite::create("res/nxpic/NXClothesShop/NXClothesShop_xiyou.png");
        costSpr->setAnchorPoint(ccp(.5f, 1.f));
        costSpr->setPosition(ccp(dikuangSpr2->getContentSize().width* .5f, -20.f));
        dikuangSpr2->addChild(costSpr);
        
        buyItem->setVisible(false);
    }
    
    
}
void NXClothesShop::buyCallback(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "换装界面", "点击购买");
    
    CCMenuItem* item = (CCMenuItem* )pSender;
    CCLog("准备购买的衣服 %d", item->getTag());
    
    _buttonStatus = 1;
    CCInteger* cloth_integer;
    CCInteger* clothesTemp_id;
    CCDictionary* shipinDic;
    int cloth_id;
    int sub_part;
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    CCDictionary* clothesTemp = DATA->getClothes()->MyClothesTemp();
    
    CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(clothesType);// 获得当前类型所有衣服
    
    // 获取选中类型未购买衣服
    CCArray* tempArr = CCArray::create();
    for (int i = 0; i < clothesArr->count(); i++) {
        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
        int sale = clothDic->valueForKey("sale")->intValue();
        int id = clothDic->valueForKey("id")->intValue();
        if (!DATA->getClothes()->is_owned(clothesType, id)) {
            if (sale != 0) {
                tempArr->addObject(clothDic);
            }
        }
    }
    // 获取要购买衣服的位置，并判断是否可以购买
    for (int i = 0; i < tempArr->count(); i++) {
        CCDictionary* dic = (CCDictionary* )tempArr->objectAtIndex(i);
        CCDictionary* shipinDic;
        if (clothesType != Tag_NXCL_ShiPin) {
            if (dic->valueForKey("id")->intValue() == item->getTag()) {
                cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
                sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
                int phase = dic->valueForKey("phase")->intValue();
                int cloth_type = dic->valueForKey("type")->intValue();
                if (cloth_type == 3 || cloth_type == 10) {
                    PromptLayer* layer = PromptLayer::create();
                    layer->show_prompt(this->getScene(), "不能购买这件衣服!");
                    return;
                }
            }
        }else{
            shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            CCInteger* clothesTemp_id;
            for (int n = 11; n <= 20; n++) {
                clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                if (dic->valueForKey("id")->intValue() == item->getTag()) {
                    cloth_id = dic->valueForKey("id")->intValue(); // 点击的衣服
                    sub_part = dic->valueForKey("sub_part")->intValue(); // 衣服的部位
                    int phase = dic->valueForKey("phase")->intValue();
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 3 || cloth_type == 10) {
                        PromptLayer* layer = PromptLayer::create();
                        layer->show_prompt(this->getScene(), "不能购买这件视频!");
                        return;
                    }
                }
            }
        }
    }
    
    //获取购买类型的临时数组
    if (clothesType != Tag_NXCL_ShiPin) {
        clothesTemp_id = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString()); // 男宠当前所穿上衣
    }else{
        shipinDic = (CCDictionary* )clothesTemp->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
        clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", sub_part)->getCString());
    }
    cloth_integer = CCInteger::create(cloth_id);
    CCString* keyStr;
    //将要购买衣服保存进临时数组
    if (clothesType != Tag_NXCL_ShiPin) {
        if (clothesType == Tag_NXCL_ShangYi) {
            if (sub_part == 1) {
                CCInteger* kuziInteger = CCInteger::create(40000);
                CCString* kuziStr = CCString::createWithFormat("%d", Tag_NXCL_KuZi);
                clothesTemp->setObject(kuziInteger, kuziStr->getCString());
                
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else if (clothesType == Tag_NXCL_KuZi) {
            CCInteger* shangyiInteger = (CCInteger* )clothesTemp->objectForKey(CCString::createWithFormat("%d", Tag_NXCL_ShangYi)->getCString());
            int shangyi_sub_part = 0;
            CCDictionary* shangyiDic = CONFIG->clothes();// 所有衣服
            CCArray* shangyiArr = (CCArray* )shangyiDic->objectForKey(Tag_NXCL_ShangYi);// 获得当前类型所有衣服
            for (int i = 0; i < shangyiArr->count(); i++) {
                CCDictionary* syDic = (CCDictionary* )shangyiArr->objectAtIndex(i);
                int shangyiId = syDic->valueForKey("id")->intValue();
                if (shangyiId == shangyiInteger->getValue()) {
                    shangyi_sub_part = syDic->valueForKey("sub_part")->intValue();
                }
            }
            
            if (shangyi_sub_part == 1) {
                CCInteger* shangyiInteger = CCInteger::create(30000);
                CCString* shangyiStr = CCString::createWithFormat("%d", Tag_NXCL_ShangYi);
                clothesTemp->setObject(shangyiInteger, shangyiStr->getCString());
                
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }else{
                keyStr = CCString::createWithFormat("%d", clothesType);
                clothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
            
        }else{
            keyStr = CCString::createWithFormat("%d", clothesType);
            clothesTemp->setObject(cloth_integer, keyStr->getCString());
        }
    }else{
        if (sub_part == 13 || sub_part == 14) {
            keyStr = CCString::createWithFormat("%d", clothesType);
            CCString* _20_keyStr = CCString::createWithFormat("%d", 20);
            CCInteger* _20Integer = CCInteger::create(70000);
            shipinDic->setObject(_20Integer, _20_keyStr->getCString());
            
            CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
            shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
            clothesTemp->setObject(shipinDic, keyStr->getCString());
        }else if (sub_part == 20){
            keyStr = CCString::createWithFormat("%d", clothesType);
            CCString* _13_keyStr = CCString::createWithFormat("%d", 13);
            CCInteger* _13Integer = CCInteger::create(70000);
            shipinDic->setObject(_13Integer, _13_keyStr->getCString());
            
            CCString* _14_keyStr = CCString::createWithFormat("%d", 14);
            CCInteger* _14Integer = CCInteger::create(70000);
            shipinDic->setObject(_14Integer, _14_keyStr->getCString());
            
            CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
            shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
            clothesTemp->setObject(shipinDic, keyStr->getCString());
        }else{
            keyStr = CCString::createWithFormat("%d", clothesType);
            CCString* sub_part_keyStr = CCString::createWithFormat("%d", sub_part);
            shipinDic->setObject(cloth_integer, sub_part_keyStr->getCString());
            clothesTemp->setObject(shipinDic, keyStr->getCString());
        }
    }
    std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
    std::string saveString;
    if (stdStr.empty()) {
        CCString* saveStr = CCString::createWithFormat("%d;", item->getTag());
        saveString.append(saveStr->getCString());
        
        CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", saveString.c_str());
        CCUserDefault::sharedUserDefault()->flush();
    }else{
        CCArray* strList = StringUtil::sharedStrUtil()->split(stdStr.c_str(), ";");
        for (int i = 0; i < strList->count(); i++) {
            CCString* listStr = (CCString* )strList->objectAtIndex(i);
            int saveClothes = atoi(listStr->getCString());
            CCString* saveStr = CCString::createWithFormat("%d;", saveClothes);
            saveString.append(saveStr->getCString());
        }
        CCString* saveStr = CCString::createWithFormat("%d;", item->getTag());
        saveString.append(saveStr->getCString());
        
        CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", saveString.c_str());
        CCUserDefault::sharedUserDefault()->flush();
    }
    
    
    if (DATA->getPlayer()->coin >= haveEnoughCoin() && DATA->getPlayer()->diam >= haveEnoughGold()) {
        if (haveEnoughCoin() == 0 && haveEnoughGold() == 0) {
            _buttonStatus = 2;
        }
        
        this->buyClothesMethods();
        
        LOADING->show_loading();
        //            NET->save_dressed_401(DATA->getClothes()->MyClothesTemp());
        if (clothesStatus == 5) {
            NET->save_competition_dress_405(DATA->getClothes()->MyClothesTemp());
        }
        else {
            NET->new_save_dressed_403(DATA->getClothes()->MyClothesTemp());
        }
        
    }else if (DATA->getPlayer()->coin < haveEnoughCoin() || DATA->getPlayer()->diam < haveEnoughGold()){
        if (DATA->getPlayer()->diam < haveEnoughGold()) {
            AHMessageBox* mb = AHMessageBox::create_with_message("钻石不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO2, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            return;
        }else if (DATA->getPlayer()->coin < haveEnoughCoin()){
            AHMessageBox* mb = AHMessageBox::create_with_message("金币不够,是否充值,亲?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO3, false);
            mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
            CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
            return;
        }
    }
}
void NXClothesShop::Http_Finished_401(CCObject* pObj){
    LOADING->remove();
    
    if (!buyClothesStr.empty()) {
        CCArray* strList1 = StringUtil::sharedStrUtil()->split(buyClothesStr.c_str(), ";");
        for (int i = 0; i < strList1->count(); i++) {
            CCString* listStr1 = (CCString* )strList1->objectAtIndex(i);
            CCArray* strList2 = StringUtil::sharedStrUtil()->split(listStr1->getCString(), "_");
            CCString* idStr = (CCString* )strList2->objectAtIndex(0);
            CCString* costStr = (CCString* )strList2->objectAtIndex(1);
            DATA->onPurchase(idStr->getCString(), 1, atoi(costStr->getCString()));
        }
    }
    
    
    AUDIO->buy_effect();
    _buttonStatus = 0;
    this->create_buySuccess();
    
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
    CCArray* tempArr = CCArray::create();
    
    for (int i = 0; i < clothesArr->count(); i++) {
        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
        int id = clothDic->valueForKey("id")->intValue();
        if (DATA->getClothes()->is_owned(clothesType, id)) {
        }else{
            if (clothes_type == 0) {
                tempArr->addObject(clothDic);
            }else if (clothes_type == 1){
                int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                if (cloth_type == 1) {
                    tempArr->addObject(clothDic);
                }
            }else if (clothes_type == 2){
                int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                if (cloth_type == 2){
                    tempArr->addObject(clothDic);
                }
            }else if (clothes_type == 3 || clothes_type == 10){
                int cloth_type = clothDic->valueForKey("type")->intValue();// 衣服价格类型
                if (cloth_type != 1 && cloth_type != 2){
                    tempArr->addObject(clothDic);
                }
            }
        }
    }
    DATA->setDataSource(tempArr);
    _delegate->updateTableView(clothesType);
    
}
void NXClothesShop::create_buySuccess(){
    animationBool = true;
    
    CCSprite* box = CCSprite::create("res/pic/clothesScene/gj_tiao.png");
    CCSize box_size = box->boundingBox().size;
    box->setScale(DISPLAY->ScreenWidth() / box_size.width) ;
    box->setPosition(DISPLAY->center());
    box->setTag(9000);
    this->addChild(box, 1000);
    
    CCSprite* buySuccessSpr = CCSprite::create("res/pic/clothesScene/gj_buySuccess.png");
    buySuccessSpr->setPosition(ccp(box->getContentSize().width* .5f, box->getContentSize().height* .5f));
    buySuccessSpr->setTag(9001);
    box->addChild(buySuccessSpr);
    
    CCSequence* boxSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), NULL);
    box->runAction(boxSeq);
    
    CCSprite* guangSpr = CCSprite::create("res/pic/clothesScene/gj_guang.png");
    guangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f + 44.f));
    guangSpr->setTag(9002);
    CCRotateBy* action = CCRotateBy::create(4.f, 360);
    guangSpr->runAction(CCRepeatForever::create(CCRepeatForever::create(action)));
    this->addChild(guangSpr, 998);
}
void NXClothesShop::removeAllSpr(){
    for (int i = 9000; i <= 9003; i++) {
        if (this->getChildByTag(i) != NULL) {
            this->removeChildByTag(i);
        }
    }
}


void NXClothesShop::buyClothesMethods(){
    if (animationBool) {
        animationBool = false;
        
        this->removeAllSpr();
    }
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    CCDictionary* myClothesTempDic = DATA->getClothes()->MyClothesTemp();
    
    for (int i = Tag_NXCL_TouFa; i <= Tag_NXCL_ZhuangRong; i++) {
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
            if (i != Tag_NXCL_ShiPin) {
                clothesTemp_id = (CCInteger* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());
                if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 2) {
                        if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                            
                            CCString* tempBuyStr = CCString::createWithFormat("%d_%d;", dic->valueForKey("id")->intValue(), dic->valueForKey("cost")->intValue());
                            buyClothesStr.append(tempBuyStr->getCString());
                        }
                    }
                }
            }else{
                shipinDic = (CCDictionary* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
                CCInteger* clothesTemp_id;
                for (int n = 11; n <= 20; n++) {
                    clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                    if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                        int cloth_type = dic->valueForKey("type")->intValue();
                        if (cloth_type == 2) {
                            if (!DATA->getClothes()->is_owned(i, dic->valueForKey("id")->intValue())) {
                                
                                CCString* tempBuyStr = CCString::createWithFormat("%d_%d;", dic->valueForKey("id")->intValue(), dic->valueForKey("cost")->intValue());
                                buyClothesStr.append(tempBuyStr->getCString());
                            }
                        }
                    }
                }
            }
        }
    }
}
int NXClothesShop::haveEnoughCoin(){
    int coin = 0;
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    CCDictionary* myClothesTempDic = DATA->getClothes()->MyClothesTemp();
    CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
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
        if (clothesType != Tag_NXCL_ShiPin) {
            clothesTemp_id = (CCInteger* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());
            if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                int cloth_type = dic->valueForKey("type")->intValue();
                if (cloth_type == 1) {
                    if (!DATA->getClothes()->is_owned(clothesType, dic->valueForKey("id")->intValue())) {
                        
                        coin += dic->valueForKey("cost")->intValue();
                    }
                }
            }
        }else{
            shipinDic = (CCDictionary* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
            CCInteger* clothesTemp_id;
            for (int n = 11; n <= 20; n++) {
                clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 1) {
                        if (!DATA->getClothes()->is_owned(clothesType, dic->valueForKey("id")->intValue())) {
                            coin += dic->valueForKey("cost")->intValue();
                        }
                    }
                }
            }
        }
    }
    return coin;
}

int NXClothesShop::haveEnoughGold(){
    int gold = 0;
    
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    
    CCDictionary* myClothesTempDic = DATA->getClothes()->MyClothesTemp();
    CCArray* clothesArr = (CCArray* )dic->objectForKey(clothesType);// 获得当前类型所有衣服
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
        if (clothesType != Tag_NXCL_ShiPin) {
            clothesTemp_id = (CCInteger* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());
            if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                int cloth_type = dic->valueForKey("type")->intValue();
                if (cloth_type == 2) {
                    if (!DATA->getClothes()->is_owned(clothesType, dic->valueForKey("id")->intValue())) {
                        
                        gold += dic->valueForKey("cost")->intValue();
                        
                        int goldIndex = DATA->getTaskGameIndex6();
                        goldIndex++;
                        if (goldIndex >= 5) {
                            goldIndex = 5;
                        }
                        DATA->setTaskGameIndex6(goldIndex);
                    }
                }
            }
        }else{
            shipinDic = (CCDictionary* )myClothesTempDic->objectForKey(CCString::createWithFormat("%d", clothesType)->getCString());// 获取所穿视频的字典
            CCInteger* clothesTemp_id;
            for (int n = 11; n <= 20; n++) {
                clothesTemp_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", n)->getCString());
                if (dic->valueForKey("id")->intValue() == clothesTemp_id->getValue()) {
                    int cloth_type = dic->valueForKey("type")->intValue();
                    if (cloth_type == 2) {
                        if (!DATA->getClothes()->is_owned(clothesType, dic->valueForKey("id")->intValue())) {
                            
                            gold += dic->valueForKey("cost")->intValue();
                            
                            int goldIndex = DATA->getTaskGameIndex6();
                            goldIndex++;
                            if (goldIndex >= 5) {
                                goldIndex = 5;
                            }
                            DATA->setTaskGameIndex6(goldIndex);
                        }
                    }
                }
            }
        }
    }
    return gold;
}

void NXClothesShop::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    if (button_type == AH_BUTTON_TYPE_YESNO2){
        if (button_tag == AH_BUTTON_TAG_YES) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
        }
    }else if (button_type == AH_BUTTON_TYPE_YESNO3){
        if (button_tag == AH_BUTTON_TAG_YES) {
            CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_COIN_EXCHANGE");
        }else if (button_tag == AH_BUTTON_TAG_NO){
            PromptLayer* layer = PromptLayer::create();
            int randIndex = rand()%2 + 1;
            if (randIndex == 1) {
                layer->show_prompt(this->getScene(), "公司的工资亲你领了吗~!可获得金币呦~!");
            }else if (randIndex == 2){
                layer->show_prompt(this->getScene(), "金币还可以去重复做其他日常任务获得呦~!");
            }
        }
    }
}

bool NXClothesShop::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (animationBool) {
        animationBool = false;
        
        this->removeAllSpr();
    }
    
    
    return true;
}









