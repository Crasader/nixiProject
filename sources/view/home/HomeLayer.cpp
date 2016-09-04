//
//  HomeLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-3.
//
//

#include "HomeLayer.h"
#include "ClothesScene.h"
#include "MainScene.h"
#include "TaskScene.h"
#include "HaoyouScene.h"
#include "HomeTableView.h"
#include "PromptLayer.h"
#include "ColorLayer.h"
#include "LiveAiXin.h"
#include "GameJingli.h"

#include "DataManager.h"
#include "DisplayManager.h"
#include "Loading2.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "AudioManager.h"
#include "AppUtil.h"


HomeLayer::HomeLayer(){
    
}
HomeLayer::~HomeLayer(){
    
}

bool HomeLayer::init(){
    if (!BaseScene::init()) {
        return false;
    }
    kuangBool = false;
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(false);
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    std::string nowHouse = DATA->getHome()->getCurHouse().c_str();
    DATA->setHouseIndex(atoi(nowHouse.c_str()));
    CCString* bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", DATA->getHouseIndex());
    bgSpr = CCSprite::create(bgStr->getCString());
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    bgSpr->setTag(0x88888);
    this->addChild(bgSpr);
    
    this->creat_View();
    this->creat_Man();
    this->initClothes();
    
    return true;
}
void HomeLayer::updataBg(){
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
    }
    
    CCString* bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", DATA->getHouseIndex());
    bgSpr = CCSprite::create(bgStr->getCString());
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    bgSpr->setTag(0x88888);
    this->addChild(bgSpr);
}

void HomeLayer::onEnter(){
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::_huanzhuangCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::_705CallBack), "HTTP_FINISHED_705", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::_800CallBack), "HTTP_FINISHED_800", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HomeLayer::updataBg), "HomeUpdataBg", NULL);
}

void HomeLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    BaseScene::onExit();
}

void HomeLayer::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}

void HomeLayer::keyBackStatus(float dt){
    
}
void HomeLayer::keyBackClicked(){
    
}

bool HomeLayer::ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent){
    
    CCPoint location = pTouch->getLocation();
    if (kuangBool) {
        if (!kuangSpr->boundingBox().containsPoint(location)) {
            kuangBool = false;
            
            CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(HomeLayer::manAction2));
            CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(kuangSpr->getContentSize().width* .5f - 200, DISPLAY->ScreenHeight()* .475f));
            CCSequence* seq = CCSequence::create(CCDelayTime::create(.3f), callFuncN, NULL);
            kuangSpr->runAction(CCSpawn::create(seq, moveTo, NULL));
            
            this->scheduleOnce(SEL_SCHEDULE(&HomeLayer::openButton), .7f);
        }
    }
    
    return true;
}
void HomeLayer::openButton(float dt){
    qiehuanSpr->setVisible(true);
}

cocos2d::CCScene* HomeLayer::scene(){
    CCScene* scene = CCScene::create();
    HomeLayer* layer = HomeLayer::create();
    scene->addChild(layer);
    
    return scene;
}
void HomeLayer::creat_View(){
    
    // 返回
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(HomeLayer::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .037f));
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 15);
    
    CCSprite* titleDiSpr = CCSprite::create("res/pic/house/house_titleDi.png");
    titleDiSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .93f));
    this->addChild(titleDiSpr, 15);
    CCString* labelStr = CCString::createWithFormat("云诗彤的房间.布拉格广场");
    CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 28, CCSizeMake(titleDiSpr->getContentSize().width* .8f, 28), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(titleDiSpr->getContentSize().width* .5f, titleDiSpr->getContentSize().height* .48f));
    label->setColor(ccWHITE);
    titleDiSpr->addChild(label);
    
    // 公司
    CCSprite* gongsiSpr1 = CCSprite::create("res/pic/house/house_gongsi.png");
    CCSprite* gongsiSpr2 = CCSprite::create("res/pic/house/house_gongsi.png");
    gongsiSpr2->setScale(1.02f);
    CCMenuItem* gongsiItem = CCMenuItemSprite::create(gongsiSpr1, gongsiSpr2, this, menu_selector(HomeLayer::gongsiCallBack));
    
    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* huanzhuangItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(HomeLayer::huanzhuangCallBack));
    
    // 好友
    CCSprite* hySpr1 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    CCSprite* hySpr2 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
    hySpr2->setScale(1.02f);
    CCMenuItem* haoyouItem = CCMenuItemSprite::create(hySpr1, hySpr2, this, menu_selector(HomeLayer::haoyouCallBack));
    
    CCMenu* buttonMenu = CCMenu::create(gongsiItem, huanzhuangItem, haoyouItem, NULL);
    buttonMenu->alignItemsVerticallyWithPadding(20.f);
    buttonMenu->setAnchorPoint(ccp(.5f, .5f));
    buttonMenu->setPosition(ccp(DISPLAY->ScreenWidth() - gongsiSpr1->getContentSize().width* .5f - 5.f, DISPLAY->ScreenHeight()* .7f));
    this->addChild(buttonMenu, 20);
    
    // 切换
    qiehuanSpr = CCSprite::create("res/pic/house/house_qiehuan2.png");
    qiehuanSpr->setAnchorPoint(ccp(0, .5f));
    qiehuanSpr->setPosition(ccp(20, DISPLAY->ScreenHeight()* .85f));
    this->addChild(qiehuanSpr, 20);
    
    CCSprite* qhSpr1 = CCSprite::create("res/pic/house/house_qiehuan1.png");
    CCSprite* qhSpr2 = CCSprite::create("res/pic/house/house_qiehuan1.png");
    qhSpr2->setScale(1.02f);
    qiehuanItem = CCMenuItemSprite::create(qhSpr1, qhSpr2, this, menu_selector(HomeLayer::qiehuanCallBack));
    qiehuanItem->setAnchorPoint(ccp(.5f, .5f));
    qiehuanItem->setPosition(ccp(qiehuanSpr->getContentSize().width* .5f, qiehuanSpr->getContentSize().height* .5f));
    CCMenu* qiehuanMenu = CCMenu::create(qiehuanItem, NULL);
    qiehuanMenu->setPosition(CCPointZero);
    qiehuanSpr->addChild(qiehuanMenu);
    
    CCDictionary* scores = DATA->getHome()->getScores();
    int score1 = ((CCInteger*)scores->objectForKey("1"))->getValue();
    int score2 = ((CCInteger*)scores->objectForKey("2"))->getValue();
    int score3 = ((CCInteger*)scores->objectForKey("3"))->getValue();
    
    // button
    CCSprite* shopSpr1 = CCSprite::create("res/pic/house/house_button1.png");
    CCSprite* shopSpr2 = CCSprite::create("res/pic/house/house_button1.png");
    shopSpr2->setScale(1.02f);
    CCMenuItem* shopItem = CCMenuItemSprite::create(shopSpr1, shopSpr2, this, menu_selector(HomeLayer::gameCallBack));
//    sleepItem->setPosition(ccp(DISPLAY->ScreenWidth()* .88f + 500, DISPLAY->ScreenHeight()* .33f));
    shopItem->setPosition(ccp(DISPLAY->ScreenWidth()* .88f + 500, 0));
    shopItem->setTag(1);
    shopItem->setScale(.3f);
    CCSprite* shopSpr = CCSprite::create("res/pic/house/house_buttonDi.png");
    shopSpr->setPosition(ccp(shopItem->getContentSize().width* .5f, -shopSpr->getContentSize().height* .2f));
    shopItem->addChild(shopSpr);
    CCString* shopStr = CCString::createWithFormat("%d", score1);
    CCLabelTTF* shopLabel = CCLabelTTF::create(shopStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(shopSpr->getContentSize().width* .4f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    shopLabel->setPosition(ccp(shopSpr->getContentSize().width* .82f, shopSpr->getContentSize().height* .46f));
    shopLabel->setColor(ccWHITE);
    shopSpr->addChild(shopLabel);
    
    CCSprite* colorSpr1 = CCSprite::create("res/pic/house/house_button2.png");
    CCSprite* colorSpr2 = CCSprite::create("res/pic/house/house_button2.png");
    colorSpr2->setScale(1.02f);
    CCMenuItem* colorItem = CCMenuItemSprite::create(colorSpr1, colorSpr2, this, menu_selector(HomeLayer::gameCallBack));
//    colorItem->setPosition(ccp(DISPLAY->ScreenWidth()* .85f + 500, DISPLAY->ScreenHeight()* .18f));
    colorItem->setPosition(ccp(DISPLAY->ScreenWidth()* .85f + 500, 0));
    colorItem->setTag(2);
    colorItem->setScale(.3f);
    CCSprite* colorSpr = CCSprite::create("res/pic/house/house_buttonDi.png");
    colorSpr->setPosition(ccp(colorItem->getContentSize().width* .5f, -colorSpr->getContentSize().height* .15f));
    colorItem->addChild(colorSpr);
    CCString* colorStr = CCString::createWithFormat("%d", score2);
    CCLabelTTF* colorLabel = CCLabelTTF::create(colorStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(colorSpr->getContentSize().width* .4f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    colorLabel->setPosition(ccp(colorSpr->getContentSize().width* .82f, colorSpr->getContentSize().height* .46f));
    colorLabel->setColor(ccWHITE);
    colorSpr->addChild(colorLabel);
    
    CCSprite* sleepSpr1 = CCSprite::create("res/pic/house/house_button3.png");
    CCSprite* sleepSpr2 = CCSprite::create("res/pic/house/house_button3.png");
    sleepSpr2->setScale(1.02f);
    CCMenuItem* sleepItem = CCMenuItemSprite::create(sleepSpr1, sleepSpr2, this, menu_selector(HomeLayer::gameCallBack));
//    shopItem->setPosition(ccp(DISPLAY->ScreenWidth()* .63f + 500, DISPLAY->ScreenHeight()* .09f));
    sleepItem->setPosition(ccp(DISPLAY->ScreenWidth()* .63f + 500, 0));
    sleepItem->setTag(3);
    sleepItem->setScale(.3f);
    CCSprite* sleepSpr = CCSprite::create("res/pic/house/house_buttonDi.png");
    sleepSpr->setPosition(ccp(sleepItem->getContentSize().width* .5f, -sleepSpr->getContentSize().height* .15f));
    sleepItem->addChild(sleepSpr);
    CCString* sleepStr = CCString::createWithFormat("%d", score3);
    CCLabelTTF* sleepLabel = CCLabelTTF::create(sleepStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(sleepSpr->getContentSize().width* .4f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    sleepLabel->setPosition(ccp(sleepSpr->getContentSize().width* .82f, sleepSpr->getContentSize().height* .46f));
    sleepLabel->setColor(ccWHITE);
    sleepSpr->addChild(sleepLabel);
    
    CCMenu* gameMenu = CCMenu::create(shopItem, colorItem, sleepItem, NULL);
    gameMenu->setPosition(CCPointZero);
    this->addChild(gameMenu, 20);
    
    CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .88f, DISPLAY->ScreenHeight()* .33f));
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
    shopItem->runAction(CCSequence::create(CCDelayTime::create(.6f), spawn1, NULL));
    
    CCMoveTo* moveTo2 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .85f, DISPLAY->ScreenHeight()* .18f));
    CCScaleTo* scaleTo2 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn2 = CCSpawn::create(moveTo2, scaleTo2, NULL);
    colorItem->runAction(CCSequence::create(CCDelayTime::create(.9f), spawn2, NULL));
    
    CCMoveTo* moveTo3 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .63f, DISPLAY->ScreenHeight()* .09f));
    CCScaleTo* scaleTo3 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn3 = CCSpawn::create(moveTo3, scaleTo3, NULL);
    sleepItem->runAction(CCSequence::create(CCDelayTime::create(1.2f), spawn3, NULL));
    
    
    // 框
    kuangSpr = CCSprite::create("res/pic/house/house_di.png");
    kuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f - 200, DISPLAY->ScreenHeight()* .475f));
    this->addChild(kuangSpr, 20);
    
    // 保存
    CCSprite* saveSpr1 = CCSprite::create("res/pic/common/btn_save.png");
    CCSprite* saveSpr2 = CCSprite::create("res/pic/common/btn_save.png");
    saveSpr2->setScale(1.02f);
    CCMenuItem* saveItem = CCMenuItemSprite::create(saveSpr1, saveSpr2, this, menu_selector(HomeLayer::saveCallBack));
    saveItem->setAnchorPoint(ccp(.5f, .5f));
    saveItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f - 2, kuangSpr->getContentSize().height* .04f));
    CCMenu* saveMenu = CCMenu::create(saveItem, NULL);
    saveMenu->setPosition(CCPointZero);
    kuangSpr->addChild(saveMenu, 15);
    
    HomeTableView* tabLayer = HomeTableView::create();
    tabLayer->setPosition(ccp(-2, 90));
    tabLayer->setTag(0x77777);
    kuangSpr->addChild(tabLayer, 5);
    
    
    
    CSJson::Value missionData = AppUtil::read_json_file("res/mission/mission");
    CCDictionary* missionDic = AppUtil::dictionary_with_json(missionData);
    for (int i = 0; i < missionDic->count(); i++) {
        CCString* keyStr = CCString::createWithFormat("%d", i);
        CCArray* missionArr = (CCArray* )missionDic->objectForKey(keyStr->getCString());
        std::string taskIdStd = ((CCString* )missionArr->objectAtIndex(0))->getCString();
        std::string phaseStd = ((CCString* )missionArr->objectAtIndex(1))->getCString();
        int taskIdIndex = atoi(taskIdStd.c_str());
        int phaseStdIndex = atoi(phaseStd.c_str());
        if (i == 0) {
            if (phaseStdIndex <= DATA->getPlayer()->phase) {
                if (taskIdIndex > DATA->getPlayer()->mission) {
                    shopItem->setEnabled(false);
                    shopItem->setColor(ccGRAY);
                }
            }else{
                shopItem->setEnabled(false);
                shopItem->setColor(ccGRAY);
            }
        }else if (i == 1){
            if (phaseStdIndex <= DATA->getPlayer()->phase) {
                if (taskIdIndex > DATA->getPlayer()->mission) {
                    colorItem->setEnabled(false);
                    colorItem->setColor(ccGRAY);
                }
            }else{
                colorItem->setEnabled(false);
                colorItem->setColor(ccGRAY);
            }
        }else if (i == 2){
            if (phaseStdIndex <= DATA->getPlayer()->phase) {
                if (taskIdIndex > DATA->getPlayer()->mission) {
                    sleepItem->setEnabled(false);
                    sleepItem->setColor(ccGRAY);
                }
            }else{
                sleepItem->setEnabled(false);
                sleepItem->setColor(ccGRAY);
            }
        }
    }    
    
}
void HomeLayer::gameCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    if (item->getTag() == 1) {
        CCLog("点击购物");
        CCScene* scene = CCScene::create();
        LiveAiXin* layer = LiveAiXin::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (item->getTag() == 2) {
        CCLog("点击颜色");
        CCScene* scene = CCScene::create();
        ColorLayer* layer = ColorLayer::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else if (item->getTag() == 3) {
        CCLog("点击睡觉");
        CCScene* scene = CCScene::create();
        GameJingli* layer = GameJingli::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void HomeLayer::qiehuanCallBack(CCObject* pSender){
    qiehuanSpr->setVisible(false);
    
    this->setTouchEnabled(false);
    CCCallFuncN* callFuncN = CCCallFuncN::create(this,callfuncN_selector(HomeLayer::manAction1));
    CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .475f));
    CCSequence* seq = CCSequence::create(CCDelayTime::create(.3f), moveTo, NULL);
    kuangSpr->runAction(CCSpawn::create(seq, callFuncN, NULL));
    
    this->scheduleOnce(SEL_SCHEDULE(&HomeLayer::openTouch), .7f);
}
void HomeLayer::openTouch(float dt){
    this->setTouchEnabled(true);
    kuangBool = true;
}
void HomeLayer::manAction1(){
    CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(130, 0));
    _ManSpr->runAction(moveTo);
}
void HomeLayer::manAction2(){
    CCMoveTo* moveTo = CCMoveTo::create(.4f, ccp(0, 0));
    _ManSpr->runAction(moveTo);
}
void HomeLayer::saveCallBack(CCObject* pSender){
    LOADING->show_loading();
    CCString* str = CCString::createWithFormat("%d", DATA->getHouseIndex());
    NET->change_house_705(str->getCString());
}
void HomeLayer::_705CallBack(CCObject* pSender){
    LOADING->remove();
    PromptLayer* layer = PromptLayer::create();
    layer->show_prompt(this->getScene(), "保存成功.");
}

void HomeLayer::backCallBack(CCObject* pSender){
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void HomeLayer::gongsiCallBack(CCObject* pSender){
    if (DATA->getStory()->has_init_story()) {
        LOADING->show_loading();
        NET->completed_mission_600();
    }else{
        LOADING->show_loading();
        NET->completed_story_500();
    }
}
void HomeLayer::_500CallBack(CCObject* pSender){
    NET->completed_mission_600();
}
void HomeLayer::_600CallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    LOADING->remove();
    
    DATA->setHomeBool(true);
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void HomeLayer::huanzhuangCallBack(CCObject* pSender){
    if (DATA->getClothes()->has_init_clothes == true) {
        this->_huanzhuangCallBack(pSender);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}
void HomeLayer::_huanzhuangCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    
    DATA->setHomeBool(true);
    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void HomeLayer::haoyouCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    LOADING->show_loading();
    NET->social_info_800();
}
void HomeLayer::_800CallBack(CCObject* pSender){
    LOADING->remove();
    
    DATA->setHomeBool(true);
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void HomeLayer::creat_Man(){
//    float widthFolt = .65f;
    float widthFolt = .5f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    manSpr->setScale(scaleFloat);
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _touSpr->setScale(scaleFloat);
    _ManSpr->addChild(_touSpr, 210);
    
    CCSprite* shadowSpr = CCSprite::create("res/pic/house/house_shadow.png");
    shadowSpr->setPosition(ccp(DISPLAY->ScreenWidth()* (widthFolt - .11f), DISPLAY->ScreenHeight()* (heightFloat - .425f)));
    _ManSpr->addChild(shadowSpr);
}
void HomeLayer::initClothes(){//穿衣服
//    float widthFolt = .65f;
    float widthFolt = .5f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    int sub_part = 0;
    
    CCDictionary* dress = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    
    for (int i = Tag_CL_TouFa; i <= Tag_CL_ZhuangRong; i++) {
        if (i == Tag_CL_TouFa) {
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_CL_TouFa1);
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_CL_TouFa2);
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
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
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_CL_TouFa1);
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_CL_TouFa2);
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_CL_TouFa3);
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_WaiTao){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_CL_WaiTao1);
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
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
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_CL_WaiTao1);
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_CL_WaiTao2);
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_CL_WaiTao3);
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_ShangYi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_CL_ShangYi1);
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        sub_part = clothDic->valueForKey("sub_part")->intValue();
                        
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_CL_ShangYi1);
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_CL_ShangYi2);
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_CL_ShangYi3);
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_KuZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_CL_KuZi1);
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_CL_KuZi1);
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _ManSpr->addChild(_kzSpr1, 290);
                }
                
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
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_CL_KuZi1);
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_CL_KuZi2);
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_CL_KuZi3);
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_WaZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_CL_WaZi1);
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
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
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_CL_WaZi1);
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_CL_WaZi2);
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_CL_WaZi3);
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_XieZi){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_CL_XieZi1);
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
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
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_CL_XieZi1);
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_CL_XieZi2);
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_CL_XieZi3);
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_CL_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )dress->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿饰品的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _spSpr1->setTag(j + 1000);
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
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
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_CL_Bao){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_CL_Bao1);
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
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
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_CL_Bao1);
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_CL_Bao2);
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_CL_Bao3);
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        
                        if (layer4->compare("") != 0) {
                            CCString* str4 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer4")->intValue());
                            _bSpr4 = CCSprite::create(str4->getCString());
                            _bSpr4->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr4->setTag(Tag_CL_Bao4);
                            _bSpr4->setScale(scaleFloat);
                            _bSpr4->setFlipX(flipxBool);
                            _ManSpr->addChild(_bSpr4, clothDic->valueForKey("z_order4")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_CL_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)dress->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_CL_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_CL_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}





