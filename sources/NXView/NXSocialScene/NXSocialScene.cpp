//
//  NXSocialScene.cpp
//  tiegao
//
//  Created by stevenLi on 17/7/10.
//
//

#include "NXSocialScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "HomeLayer.h"
#include "ConfigManager.h"
#include "TaskScene.h"

//#include "HaoyouRankLayer.h"
#include "FriendsScene.h"

#include "StrangerScene.h"
//#include "TotalRankScene.h"
#include "RankListScene.h"
#include "Loading2.h"
#include "NetManager.h"
#include "MessageLayer.h"
#include "NoteLayer.h"
#include "AudioManager.h"
#include "WSManager.h"
#include "ChatPanel.h"
#include "AppUtil.h"
#include "JNIController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ShareManager.h"
#endif


NXSocialScene::NXSocialScene() {}
NXSocialScene::~NXSocialScene() {}

bool NXSocialScene::init() {
    if(!NXBaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    _isLoadRanklist = false;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    this->setTouchEnabled(false);
    
    return true;
}

CCScene* NXSocialScene::scene() {
    CCScene* scene = CCScene::create();
    NXSocialScene* layer = NXSocialScene::create();
    scene->addChild(layer);
    return scene;
}

void NXSocialScene::onEnter() {
    NXBaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::competition_callback_820), "HTTP_FINISHED_820", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::strangers_callback_802), "HTTP_FINISHED_802", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::all_message_callback_804), "HTTP_FINISHED_804", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::all_paper_callback_808), "HTTP_FINISHED_808", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::all_friends_callback_806), "HTTP_FINISHED_806", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::rank_list_callback_300), "HTTP_FINISHED_300", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::displayChatItem), "CLOSE_CHATPANEL", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::_605CallBack), "HTTP_FINISHED_605", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::_321CallBack), "HTTP_FINISHED_321", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::iOS_share_finish), "IOS_SHARE_FINISH", NULL);
    nc->addObserver(this, SEL_CallFuncO(&NXSocialScene::createChatPanel), "OPEN_CHAT", NULL);
    
    this->update_news_status();
    this->schedule(SEL_SCHEDULE(&NXSocialScene::update_news_status), 3);
    
    this->scheduleOnce(SEL_SCHEDULE(&NXSocialScene::keyBackStatus), .8f);
    //
    DISPLAY->setZRSpr(_zrSpr1);
    DISPLAY->blink();
}

void NXSocialScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void NXSocialScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    DISPLAY->stopBlink();
    
    NXBaseScene::onExit();
}

void NXSocialScene::creat_view(){
    roomSpr = CCSprite::create("res/pic/haoyoupaihang/social_bg.png");
    roomSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(roomSpr);
    
    shareBgSpr = CCSprite::create("res/pic/baseScene/base_shareBg.png");
    shareBgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(shareBgSpr, 15);
    shareBgSpr->setVisible(false);
    
    CCSprite* backSpr1 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    CCSprite* backSpr2 = CCSprite::create("res/nxpic/NXMainscene/nx_fanhui.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(NXSocialScene::backCallBack));
    backItem->setAnchorPoint(ccp(.0f, 0.5f));
    backItem->setPosition(ccp(.0f, DISPLAY->ScreenHeight()* .84f));
    
    CCSprite* xiaoxiSpr1 = CCSprite::create("res/nxpic/NXSocialScene/messageButton.png");
    CCSprite* xiaoxiSpr2 = CCSprite::create("res/nxpic/NXSocialScene/messageButton.png");
    xiaoxiSpr2->setScale(1.02f);
    _xiaoxiItem = CCMenuItemSprite::create(xiaoxiSpr1, xiaoxiSpr2, this, menu_selector(NXSocialScene::xiaoxiCallBack));
    _xiaoxiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .1667f, DISPLAY->ScreenHeight()* .06f));
    
    CCSprite* zhitiaoSpr1 = CCSprite::create("res/nxpic/NXSocialScene/noteButton.png");
    CCSprite* zhitiaoSpr2 = CCSprite::create("res/nxpic/NXSocialScene/noteButton.png");
    zhitiaoSpr2->setScale(1.02f);
    _zhitiaoItem = CCMenuItemSprite::create(zhitiaoSpr1, zhitiaoSpr2, this, menu_selector(NXSocialScene::zhitiaoCallBack));
    _zhitiaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .3333f, DISPLAY->ScreenHeight()* .06f));
    
    CCSprite* haoyouSpr1 = CCSprite::create("res/nxpic/NXSocialScene/friendButton.png");
    CCSprite* haoyouSpr2 = CCSprite::create("res/nxpic/NXSocialScene/friendButton.png");
    haoyouSpr2->setScale(1.02f);
    CCMenuItem* haoyouItem = CCMenuItemSprite::create(haoyouSpr1, haoyouSpr2, this, menu_selector(NXSocialScene::haoyouCallBack));
    haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5000f, DISPLAY->ScreenHeight()* .06f));
    
    CCSprite* strangerSpr1 = CCSprite::create("res/nxpic/NXSocialScene/strangerButton.png");
    CCSprite* strangerSpr2 = CCSprite::create("res/nxpic/NXSocialScene/strangerButton.png");
    strangerSpr2->setScale(1.02f);
    CCMenuItem* strangerItem = CCMenuItemSprite::create(strangerSpr1, strangerSpr2, this, menu_selector(NXSocialScene::strangerCallBack));
    strangerItem->setPosition(ccp(DISPLAY->ScreenWidth()* .6667f, DISPLAY->ScreenHeight()* .06f));
    
    CCSprite* paihangSpr1 = CCSprite::create("res/nxpic/NXSocialScene/rankButton.png");
    CCSprite* paihangSpr2 = CCSprite::create("res/nxpic/NXSocialScene/rankButton.png");
    paihangSpr2->setScale(1.02f);
    CCMenuItem* paihangItem = CCMenuItemSprite::create(paihangSpr1, paihangSpr2, this, menu_selector(NXSocialScene::paihangCallBack));
    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .8333f, DISPLAY->ScreenHeight()* .06f));
    
    // 聊天
    CCSprite* qipao = CCSprite::create("res/pic/panel/chat/qipao.png");
    CCSprite* qipao2 = CCSprite::create("res/pic/panel/chat/qipao.png");
    qipao2->setScale(1.02f);
    item_chat = CCMenuItemSprite::create(qipao, qipao2, this, menu_selector(NXSocialScene::openChat));
    item_chat->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
    
    allMenu = CCMenu::create(backItem, _xiaoxiItem, _zhitiaoItem, haoyouItem, strangerItem, paihangItem, item_chat, NULL);
    allMenu->setPosition(CCPointZero);
    this->addChild(allMenu, 20);
    
}
void NXSocialScene::shareCallBack(CCObject* pSender){
    //    allMenu->setVisible(false);
    //
    //    if (this->getChildByTag(0x1008)) {
    //        this->getChildByTag(0x1008)->setVisible(false);
    //    }
    //
    //    BaseScene::hideBaseScene();
    
    this->hiddenCallback2();
    shareBgSpr->setVisible(true);
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    CCLOG("图片 === %s", path.c_str());
    
    rt->saveToFile(path.c_str());
    ShareManager::get_instance()->share_pic();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    JNIController::setShareImage(path.c_str());
    rt->saveToFile(path.c_str());
    
    JNIController::showShare(1, 0);
    this->schedule(SEL_SCHEDULE(&HaoyouScene::shareStatus), .1f);
#endif
}

void NXSocialScene::iOS_share_finish(CCObject* pSender) {
    //    allMenu->setVisible(true);
    //
    //    if(this->getChildByTag(0x1008)) {
    //        this->getChildByTag(0x1008)->setVisible(true);
    //    }
    //
    //    BaseScene::openBaseScene();
    this->openCallback2();
    
    LOADING->show_loading();
    NET->daily_share_321();
}

void NXSocialScene::shareStatus(float dt){
    allMenu->setVisible(true);
    shareBgSpr->setVisible(false);
    
    if(this->getChildByTag(0x1008)) {
        this->getChildByTag(0x1008)->setVisible(true);
    }
    
    NXBaseScene::openBaseScene();
    
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        LOADING->show_loading();
        NET->daily_share_321();
        CCLog("《《《《《《 分享成功 》》》》》》》");
        this->unschedule(SEL_SCHEDULE(&NXSocialScene::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        CCLog("《《《《《《 分享失败 》》》》》》》");
        this->unschedule(SEL_SCHEDULE(&NXSocialScene::shareStatus));
    }
}
void NXSocialScene::_321CallBack(CCObject* pObj){
    LOADING->remove();
    
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
    shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(NXSocialScene::shareCallBack));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}


void NXSocialScene::openChat() {
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击聊天");
    
    AUDIO->comfirm_effect();
    DATA->setChatOut(false);
    if (WS->isConnected()) {
        this->createChatPanel();
    }else{
        WS->connect();
    }
}

void NXSocialScene::createChatPanel() {
    ChatPanel* panel = ChatPanel::create(false);
    panel->setTag(0x1008);
    this->addChild(panel, 100000);
    item_chat->setVisible(false);
}

void NXSocialScene::displayChatItem() {
    if (item_chat->isVisible()) {
        item_chat->setVisible(false);
    }else{
        item_chat->setVisible(true);
    }
}

void NXSocialScene::_605CallBack(CCObject* pObj){
    LOADING->remove();
    
    num_child = 0;
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NXSocialScene::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击返回");
    
    if (DATA->getTaskGameBool4()) {
        LOADING->show_loading();
        NET->commit_extra_mission_605(DATA->getTaskTempID(), 4, DATA->getTaskGameIndex4());
    }else{
        if (DATA->getHomeBool()) {
            DATA->setHomeBool(false);
            num_child = 0;
            CCScene* scene = HomeLayer::scene();
            CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
            CCDirector::sharedDirector()->replaceScene(trans);
        }else{
            num_child = 0;
            CCScene* scene = MainScene::scene();
            CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
            CCDirector::sharedDirector()->replaceScene(trans);
        }
    }
}
void NXSocialScene::_600CallBack(CCObject* pObj){
    LOADING->remove();
    
    DATA->setTaskPhase(DATA->getPlayer()->phase);
    CCLayer* layer = TaskScene::create(false);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void NXSocialScene::xiaoxiCallBack(CCObject* pSender){
    
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击消息");
    
    LOADING->show_loading();
    
    NET->all_messages_804();
}

void NXSocialScene::zhitiaoCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击纸条");
    
    LOADING->show_loading();
    
    NET->all_paper_808();
}

void NXSocialScene::all_paper_callback_808(CCObject* pSender){
    LOADING->remove();
    CCScene* scene = NoteLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NXSocialScene::all_message_callback_804(CCObject* pSender){
    CCScene* scene = MessageLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NXSocialScene::haoyouCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击好友");
    
    LOADING->show_loading();
    NET->all_friends_806();
}

void NXSocialScene::competition_callback_820(CCObject *pObj) {
    _isLoadRanklist = true;
    if (DATA->getSocial()->getHasInitFriends()) {
        this->all_friends_callback_806(NULL);
    }
    else {
        NET->all_friends_806();
    }
}

void NXSocialScene::all_friends_callback_806(CCObject* pObj) {
    LOADING->remove();
    if (_isLoadRanklist) {
        RankListScene* layer = RankListScene::create();
        DATA->setComeFrom("social");
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }else{
        CCScene* scene = FriendsScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    
    _isLoadRanklist = false;
}

void NXSocialScene::strangerCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击陌生人");
    
    LOADING->show_loading();
    NET->recommend_stranger_802();
}

void NXSocialScene::paihangCallBack(CCObject* pSender){
    AUDIO->common_effect();
    // talkingData
    DATA->onEvent("点击事件", "好友界面", "点击排行");
    
    LOADING->show_loading();
    //    NET->ranking_list_300();
    NET->competition_info_820(! DATA->getCompetition()->hasInitRankInfo());
}

void NXSocialScene::rank_list_callback_300(CCObject *pObj){
    if (! _isLoadRanklist) {
        NET->all_friends_806();
    }
}

void NXSocialScene::strangers_callback_802(cocos2d::CCObject *pSender){
    LOADING->remove();
    CCScene* scene = CCScene::create();
    StrangerScene* layer = StrangerScene::create();
    layer->setEnterType("main_friend");
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void NXSocialScene::openCallback(){
    // 隐藏0
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(NXSocialScene::hiddenCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
    
    allMenu->setVisible(true);
    
    if(this->getChildByTag(0x1008)) {
        this->getChildByTag(0x1008)->setVisible(true);
    }
    
    NXBaseScene::openBaseScene();
}

void NXSocialScene::openCallback2(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(true);
    shareBgSpr->setVisible(false);
    
    if(this->getChildByTag(0x1008)) {
        this->getChildByTag(0x1008)->setVisible(true);
    }
    
    NXBaseScene::openBaseScene();
    
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_hidden.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(NXSocialScene::hiddenCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
}

void NXSocialScene::hiddenCallback(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(false);
    
    if(this->getChildByTag(0x1008)) {
        this->getChildByTag(0x1008)->setVisible(false);
    }
    
    NXBaseScene::hideBaseScene();
}

void NXSocialScene::hiddenCallback2(){
    if (this->getChildByTag(0x46577) != NULL) {
        this->removeChildByTag(0x46577);
    }
    
    allMenu->setVisible(false);
    
    if(this->getChildByTag(0x1008)) {
        this->getChildByTag(0x1008)->setVisible(false);
    }
    
    NXBaseScene::hideBaseScene();
    
    CCSprite* hidSpr1 = CCSprite::create("res/pic/haoyouScene/hy_show.png");
    CCSprite* hidSpr2 = CCSprite::create("res/pic/haoyouScene/hy_show.png");
    hidSpr2->setScale(1.02f);
    CCMenuItem* hidItem = CCMenuItemSprite::create(hidSpr1, hidSpr2, this, menu_selector(NXSocialScene::openCallback2));
    hidItem->setPosition(ccp(DISPLAY->ScreenWidth()* .92f, DISPLAY->ScreenHeight()* .85f));
    
    CCMenu* hidMenu = CCMenu::create(hidItem, NULL);
    hidMenu->setPosition(CCPointZero);
    hidMenu->setTag(0x46577);
    this->addChild(hidMenu, 5);
}


void NXSocialScene::creat_Man(){
//    float widthFolt = .8f;
//    float heightFloat = .3f;
//    float scaleFloat = 1.5f;
    float widthFolt = .5f;
    float heightFloat = .2f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_man.png");
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    manSpr->setScale(scaleFloat);
    _ManSpr->addChild(manSpr, 200);
    
    CCSprite* geboSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_gebo.png");
    geboSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    geboSpr->setScale(scaleFloat);
    _ManSpr->addChild(geboSpr, 415);
    
    _touSpr = CCSprite::create("res/nxpic/NXClothes/man/nx_lian.png");
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _touSpr->setScale(scaleFloat);
    _ManSpr->addChild(_touSpr, 210);

}
void NXSocialScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
//    float widthFolt = .8f;
//    float heightFloat = .3f;
//    float scaleFloat = 1.5f;
//    bool flipxBool = false;
//    int sub_part = 0;
    float widthFolt = DISPLAY->ScreenWidth()* .5f;
    float heightFloat = .2f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_SS_TouFa; i <= Tag_SS_ZhuangRong; i++) {
        if (i == Tag_SS_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_SS_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_SS_TouFa2);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_SS_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_SS_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_SS_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_SS_WaiTao1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_SS_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_SS_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_SS_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_SS_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
                
                //                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                //                xingSpr1->setFlipX(true);
                //                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
                //                _ManSpr->addChild(xingSpr1, 420);
                //
                //                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                //                xingSpr2->setFlipX(true);
                //                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
                //                _ManSpr->addChild(xingSpr2, 420);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_SS_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_SS_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_SS_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_SS_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_SS_KuZi1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_SS_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_SS_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_SS_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_TeXiao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/5texiao/%d.png", 50000);
                _txSpr1 = CCSprite::create(str->getCString());
                _txSpr1->setScale(scaleFloat);
                _txSpr1->setFlipX(flipxBool);
                _txSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _txSpr1->setTag(Tag_SS_TeXiao1);
                _ManSpr->addChild(_txSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/5texiao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _txSpr1 = CCSprite::create(str1->getCString());
                            _txSpr1->setScale(scaleFloat);
                            _txSpr1->setFlipX(flipxBool);
                            _txSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _txSpr1->setTag(Tag_SS_TeXiao1);
                            _ManSpr->addChild(_txSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_SS_XieZi1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_SS_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_SS_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_SS_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_SS_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* .5f));
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
                                CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_SS_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_SS_Bao1);
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
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_SS_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_SS_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_SS_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_SS_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/nxpic/NXClothes/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_SS_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
                //
                DISPLAY->setCurZRId(90000);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/nxpic/NXClothes/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_SS_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                            //
                            DISPLAY->setCurZRId(layer1->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}

void NXSocialScene::update_news_status() {
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


void NXSocialScene::keyBackClicked(){
    num_child++;
    CCLog("===== NXSocial children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->backCallBack(NULL);
}
