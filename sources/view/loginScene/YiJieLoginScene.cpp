//
//  YiJieLoginScene.cpp
//  tiegao
//
//  Created by mac on 16/9/9.
//
//

#include "ConfigManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "DisplayManager.h"
#include "AudioManager.h"

#include "YiJieLoginScene.h"
#include "Loading2.h"
#include "PromptLayer.h"
#include "CreateName.h"
#include "MainScene.h"

#include "JNIController.h"

YiJieLoginScene::~YiJieLoginScene() {

}

CCScene* YiJieLoginScene::scene() {
    CCScene* scene = CCScene::create();
    scene->addChild(YiJieLoginScene::create());
    return scene;
}

bool YiJieLoginScene::init() {
    CCLog("<><><><><> YiJieLoginScene::init()");
    if (CCLayer::init()) {
        CCSprite* bg = CCSprite::create("res/pic/loginScene/login_bg.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        
        return true;
    }
    else {
        return false;
    }
}

void YiJieLoginScene::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::fast_login_callback_900), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::game_login_callback_902), "HTTP_FINISHED_902", NULL);
    nc->addObserver(this, SEL_CallFuncO(&YiJieLoginScene::save_nickname_callback_904), "HTTP_FINISHED_904", NULL);
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    this->setTouchEnabled(true);
    
    this->create_views();
}

void YiJieLoginScene::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCEGLView::sharedOpenGLView()->setIMEKeyboardState(false);
    
    CCLayer::onExit();
}

//bool YiJieLoginScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
//    CCLOG("YiJieLoginScene::ccTouchBegan()~");
//    return true;
//}

#pragma - inner API

void YiJieLoginScene::create_views() {
    // 需要检查网络状况
    
    
    CCSprite* startSpr1 = CCSprite::create("res/pic/loginScene/login_btn_fast2.png");
    CCSprite* startSpr2 = CCSprite::create("res/pic/loginScene/login_btn_fast2.png");
    startSpr2->setScale(1.01f);
    startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(YiJieLoginScene::startCallBack));
    startItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    CCMenu* startMenu = CCMenu::create(startItem, NULL);
    startItem->setPosition(CCPointZero);
    this->addChild(startMenu, 10);
    
    CCString* strVersion = CCString::createWithFormat("v%s - %d", CONFIG->version.c_str(), CONFIG->netId);
    CCLabelTTF* lblVersion = CCLabelTTF::create(strVersion->getCString(), DISPLAY->fangzhengFont(), 20.f);
    lblVersion->setColor(ccORANGE);
    lblVersion->setPosition(ccp(DISPLAY->W() * 0.85, DISPLAY->H() * 0.04));
    this->addChild(lblVersion);
}
void YiJieLoginScene::startCallBack(CCObject* pSender){
    LOADING->show_loading();
    JNIController::isLanding(1);
    this->schedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus), .5f);
}
void YiJieLoginScene::updataLoginStatus(float dt){
    if (JNIController::getLandStatus() == 1) {
        JNIController::setLandStatus(0);
        this->unschedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus));
        startItem->setVisible(false);
        
        CCString* seccionStr;
        seccionStr = CCString::createWithFormat("%s", JNIController::getSessionid().c_str());
        CCLog("<><><><> seccionStr == %s", seccionStr->getCString());
        LOADING->show_loading();
//        DATA->setLoginType(1);
        CONFIG->save_login_type(1);
        NET->fast_login_900(seccionStr->getCString(), CONFIG->channelId);
    }else if (JNIController::getLandStatus() == 2){
        JNIController::setLandStatus(0);
        this->unschedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus));
        startItem->setVisible(true);
        
        // 添加提示 登录失败，是否重新登录   是发送isLanding  否推出游戏
        AHMessageBox* mb = AHMessageBox::create_with_message("登录失败,是否重新登录?", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
        mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3900);
    }
}

void YiJieLoginScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    box->animation_out();
    
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
            startCallBack(NULL);
        }else if (button_tag == AH_BUTTON_TAG_NO){
            JNIController::exitGame(CONFIG->baiOrYijie);
        }
    }
}


void YiJieLoginScene::show_nicknameview() {
    CreateName* layer = CreateName::create();
    this->addChild(layer);
}

void YiJieLoginScene::fast_login_callback_900(CCObject *pObj) {
    LOADING->remove();
    if (! CONFIG->has_saved_uuid()) {
        CONFIG->save_uuid(DATA->getLogin()->obtain_UUID());
    }
    LOADING->show_loading();
    NET->login_game_server_902();
}

void YiJieLoginScene::game_login_callback_902(CCObject *pObj) {
    LOADING->remove();
    
    const char* nickname = DATA->getShow()->nickname();
    if (strcmp(nickname, "") == 0) {

        this->show_nicknameview();
        AUDIO->first_run_config();
        AUDIO->play_main_bgm();
    }
    else {
        CCScene* scene = MainScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void YiJieLoginScene::save_nickname_callback_904(CCObject *pObj) {
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
