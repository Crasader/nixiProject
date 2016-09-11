//
//  HaoyouScene.cpp
//  tiegao
//
//  Created by mac on 16-6-18.
//
//

#include "HaoyouScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "HomeLayer.h"
#include "ConfigManager.h"
#include "TaskScene.h"

//#include "HaoyouRankLayer.h"
#include "FriendsScene.h"

#include "StrangerScene.h"
#include "TotalRankScene.h"
#include "Loading2.h"
#include "NetManager.h"
#include "MessageLayer.h"
#include "NoteLayer.h"
#include "AudioManager.h"
#include "WSManager.h"
#include "ChatPanel.h"
#include "AppUtil.h"
#include "JNIController.h"



HaoyouScene::HaoyouScene(){
    
}
HaoyouScene::~HaoyouScene(){
    
}

bool HaoyouScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    hasFriends = false;
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    this->setTouchEnabled(false);
    
    return true;
}
CCScene* HaoyouScene::scene(){
    CCScene* scene = CCScene::create();
    HaoyouScene* layer = HaoyouScene::create();
    scene->addChild(layer);
    
    return scene;
}

void HaoyouScene::onEnter(){
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::strangers_callback_802), "HTTP_FINISHED_802", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::all_message_callback_804), "HTTP_FINISHED_804", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::all_paper_callback_808), "HTTP_FINISHED_808", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::all_friends_callback_806), "HTTP_FINISHED_806", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::rank_list_callback_300), "HTTP_FINISHED_300", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::displayChatItem), "CLOSE_CHATPANEL", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::_605CallBack), "HTTP_FINISHED_605", NULL);
    nc->addObserver(this, SEL_CallFuncO(&HaoyouScene::_321CallBack), "HTTP_FINISHED_321", NULL);
    
    this->update_news_status();
    this->schedule(SEL_SCHEDULE(&HaoyouScene::update_news_status), 3);
    
    this->scheduleOnce(SEL_SCHEDULE(&HaoyouScene::keyBackStatus), .8f);
}

void HaoyouScene::onEnterTransitionDidFinish() {
    CCLayer::onEnterTransitionDidFinish();
    CCLog("onEnterTransitionDidFinish");
    this->openChat();
}

void HaoyouScene::onExitTransitionDidStart() {
    if (this->getChildByTag(0x1008)) {
        this->removeChildByTag(0x1008, true);
    }
    DATA->setChatOut(true);
    DATA->getChat()->setItems(CCArray::create());
    CCLOG("onExitTransitionDidStart");
    CCLayer::onExitTransitionDidStart();
}

void HaoyouScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void HaoyouScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    BaseScene::onExit();
}

void HaoyouScene::creat_view(){
    roomSpr = CCSprite::create("res/pic/qingjingScene/bgimage/33.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(HaoyouScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCSprite* xiaoxiSpr1 = CCSprite::create("res/pic/haoyouScene/hy_xiaoxi.png");
    CCSprite* xiaoxiSpr2 = CCSprite::create("res/pic/haoyouScene/hy_xiaoxi.png");
    xiaoxiSpr2->setScale(1.02f);
    _xiaoxiItem = CCMenuItemSprite::create(xiaoxiSpr1, xiaoxiSpr2, this, menu_selector(HaoyouScene::xiaoxiCallBack));
    _xiaoxiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .42f));
    
    CCSprite* zhitiaoSpr1 = CCSprite::create("res/pic/haoyouScene/hy_zhitiao.png");
    CCSprite* zhitiaoSpr2 = CCSprite::create("res/pic/haoyouScene/hy_zhitiao.png");
    zhitiaoSpr2->setScale(1.02f);
    _zhitiaoItem = CCMenuItemSprite::create(zhitiaoSpr1, zhitiaoSpr2, this, menu_selector(HaoyouScene::zhitiaoCallBack));
    _zhitiaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .30f));
    
    CCSprite* haoyouSpr1 = CCSprite::create("res/pic/haoyouScene/hy_haoyou.png");
    CCSprite* haoyouSpr2 = CCSprite::create("res/pic/haoyouScene/hy_haoyou.png");
    haoyouSpr2->setScale(1.02f);
    CCMenuItem* haoyouItem = CCMenuItemSprite::create(haoyouSpr1, haoyouSpr2, this, menu_selector(HaoyouScene::haoyouCallBack));
    haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .89f, DISPLAY->ScreenHeight()* .33f));
    
    CCSprite* strangerSpr1 = CCSprite::create("res/pic/haoyouScene/hy_stranger.png");
    CCSprite* strangerSpr2 = CCSprite::create("res/pic/haoyouScene/hy_stranger.png");
    strangerSpr2->setScale(1.02f);
    CCMenuItem* strangerItem = CCMenuItemSprite::create(strangerSpr1, strangerSpr2, this, menu_selector(HaoyouScene::strangerCallBack));
    strangerItem->setPosition(ccp(DISPLAY->ScreenWidth()* .89f, DISPLAY->ScreenHeight()* .2f));
    
    CCSprite* paihangSpr1 = CCSprite::create("res/pic/haoyouScene/hy_paihang.png");
    CCSprite* paihangSpr2 = CCSprite::create("res/pic/haoyouScene/hy_paihang.png");
    paihangSpr2->setScale(1.02f);
    CCMenuItem* paihangItem = CCMenuItemSprite::create(paihangSpr1, paihangSpr2, this, menu_selector(HaoyouScene::paihangCallBack));
    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .89f, DISPLAY->ScreenHeight()* .07f));
    
    // 聊天
    CCSprite* qipao = CCSprite::create("res/pic/panel/chat/qipao.png");
    CCSprite* qipao2 = CCSprite::create("res/pic/panel/chat/qipao.png");
    qipao2->setScale(1.02f);
    item_chat = CCMenuItemSprite::create(qipao, qipao2, this, menu_selector(HaoyouScene::openChat));
    item_chat->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
    
    allMenu = CCMenu::create(backItem, _xiaoxiItem, _zhitiaoItem, haoyouItem, strangerItem, paihangItem, item_chat, NULL);
    allMenu->setPosition(CCPointZero);
    this->addChild(allMenu, 20);
    
    
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(HaoyouScene::shareCallBack));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(HaoyouScene::hiddenCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
}
void HaoyouScene::shareCallBack(CCObject* pSender){
    allMenu->setVisible(false);
    BaseScene::hideBaseScene();
    
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    CCLog("图片 === %s", path.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    rt->saveToFile(path.c_str());
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JNIController::setShareImage(path.c_str());
    rt->saveToFile(path.c_str());
    
    JNIController::showShare();
    this->schedule(SEL_SCHEDULE(&HaoyouScene::shareStatus), .1f);
#endif
}
void HaoyouScene::shareStatus(float dt){
    allMenu->setVisible(true);
    BaseScene::openBaseScene();
    
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        if (this->getChildByTag(0x334455) != NULL) {
            this->removeChildByTag(0x334455);
        }
        CCSprite* shareSpr1;
        CCSprite* shareSpr2;
        if (DATA->getNews()->dailyShareCount == 0) {
            shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
            shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
            shareSpr2->setScale(1.02f);
        }else{
            shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
            shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
            shareSpr2->setScale(1.02f);
        }
        shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(HaoyouScene::shareCallBack));
        shareItem->setAnchorPoint(ccp(0, .5f));
        shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
        shareMenu = CCMenu::create(shareItem, NULL);
        shareMenu->setPosition(CCPointZero);
        shareMenu->setTag(0x334455);
        this->addChild(shareMenu, 20);
        
        LOADING->show_loading();
        NET->daily_share_321();
        
        this->unschedule(SEL_SCHEDULE(&HaoyouScene::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        this->unschedule(SEL_SCHEDULE(&HaoyouScene::shareStatus));
    }
}
void HaoyouScene::_321CallBack(CCObject* pObj){
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}


void HaoyouScene::openChat() {
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击聊天");
    
    AUDIO->comfirm_effect();
    DATA->setChatOut(false);
    if (WS->isConnected()) {
        ChatPanel* panel = ChatPanel::create();
//        CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
        panel->setTag(0x1008);
        this->addChild(panel, 100000);
    }else{
        WS->connect();
    }
}

void HaoyouScene::displayChatItem() {
    if (item_chat->isVisible()) {
        item_chat->setVisible(false);
    }else{
        item_chat->setVisible(true);
    }
}

void HaoyouScene::_605CallBack(CCObject* pObj){
    LOADING->remove();
    
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void HaoyouScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击返回");
    
    if (DATA->getTaskGameBool4()) {
        LOADING->show_loading();
        NET->commit_extra_mission_605(DATA->getTaskTempID(), 4, 0);
    }else{
        if (DATA->getHomeBool()) {
            DATA->setHomeBool(false);
            
            CCScene* scene = HomeLayer::scene();
            CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
            CCDirector::sharedDirector()->replaceScene(trans);
        }else{
            CCScene* scene = MainScene::scene();
            CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
            CCDirector::sharedDirector()->replaceScene(trans);
        }
    }
}
void HaoyouScene::_600CallBack(CCObject* pObj){
    LOADING->remove();
    
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void HaoyouScene::xiaoxiCallBack(CCObject* pSender){
    
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击消息");
    
    LOADING->show_loading();
    
    NET->all_messages_804();
}

void HaoyouScene::zhitiaoCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击纸条");
    
    LOADING->show_loading();
    
    NET->all_paper_808();
}

void HaoyouScene::all_paper_callback_808(CCObject* pSender){
    LOADING->remove();
    CCScene* scene = NoteLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void HaoyouScene::all_message_callback_804(CCObject* pSender){
    CCScene* scene = MessageLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void HaoyouScene::haoyouCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击好友");
    
    LOADING->show_loading();
    NET->all_friends_806();
}

void HaoyouScene::all_friends_callback_806(CCObject* pObj) {
    LOADING->remove();
    
    if (hasFriends) {
        CCLayer* layer = TotalRankScene::create_with_type(2);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else{
        CCScene* scene = FriendsScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
//    CCScene* scene = HaoyouRankLayer::scene();
//    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
//    CCDirector::sharedDirector()->replaceScene(trans);
    
}

void HaoyouScene::strangerCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击陌生人");
    
    LOADING->show_loading();
    NET->recommend_stranger_802();
}

void HaoyouScene::paihangCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击排行");
    
    LOADING->show_loading();
    NET->ranking_list_300();
}

void HaoyouScene::rank_list_callback_300(CCObject *pObj){    
    hasFriends = true;
    NET->all_friends_806();
    
}

void HaoyouScene::strangers_callback_802(cocos2d::CCObject *pSender){
    LOADING->remove();
    CCScene* scene = CCScene::create();
    StrangerScene* layer = StrangerScene::create();
    layer->setEnterType("main_friend");
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void HaoyouScene::openCallback(){
    // 隐藏0
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(HaoyouScene::hiddenCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
    
    allMenu->setVisible(true);
    BaseScene::openBaseScene();
}
void HaoyouScene::openCallback2(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(true);
    BaseScene::openBaseScene();
    
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(HaoyouScene::hiddenCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
}
void HaoyouScene::hiddenCallback(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(false);
    BaseScene::hideBaseScene();
}
void HaoyouScene::hiddenCallback2(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(false);
    BaseScene::hideBaseScene();
    
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_show.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_show.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(HaoyouScene::openCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
}


void HaoyouScene::creat_Man(){
    float widthFolt = .8f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void HaoyouScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .8f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_Hy_TouFa; i <= Tag_Hy_ZhuangRong; i++) {
        if (i == Tag_Hy_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_Hy_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_Hy_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_Hy_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_Hy_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_Hy_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_Hy_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_Hy_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_Hy_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_Hy_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_Hy_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_Hy_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_Hy_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_Hy_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Hy_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Hy_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }
                
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_Hy_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_Hy_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_Hy_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_Hy_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_Hy_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_Hy_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_Hy_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_Hy_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
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
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_Hy_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_Hy_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_Hy_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Hy_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
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
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_Hy_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_Hy_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_Hy_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_Hy_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_Hy_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_Hy_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_Hy_ZhuangRong1);
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
                            _zrSpr1->setTag(Tag_Hy_ZhuangRong1);
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

void HaoyouScene::update_news_status() {
    NewsComp* news = DATA->getNews();
    if (news->message > 0) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setScale(1.5);
        spt->setPosition(ccp(72, 92));
        _xiaoxiItem->addChild(spt);
    }
    
    if (news->paper) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setScale(1.5);
        spt->setPosition(ccp(72, 92));
        _zhitiaoItem->addChild(spt);
    }
}


void HaoyouScene::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->backCallBack(NULL);
}

