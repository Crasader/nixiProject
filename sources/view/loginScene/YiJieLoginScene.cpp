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
//    CCLog("<><><><><> YiJieLoginScene::init()");
    if (CCLayer::init()) {
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/loginScene/login_hua.plist");
        
        bg = CCSprite::create("res/pic/loginScene/login_bg2.png");
        bg->setPosition(DISPLAY->center());
        this->addChild(bg);
        
        CCSprite* logo = CCSprite::create("res/pic/loginScene/login_logo.png");
        logo->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.2));
        this->addChild(logo);
        
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
    
    if (CONFIG->channelId != 7) {
        CCSprite* startSpr1 = CCSprite::create("res/pic/loginScene/login_btn_fast3.png");
        CCSprite* startSpr2 = CCSprite::create("res/pic/loginScene/login_btn_fast3.png");
        startSpr2->setScale(1.01f);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(YiJieLoginScene::startCallBack));
        startItem->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 160, DISPLAY->ScreenHeight()* .3f));
        startItem->setTag(1);
        CCMenu* startMenu = CCMenu::create(startItem, NULL);
        startMenu->setPosition(CCPointZero);
        this->addChild(startMenu, 10);
    }else{
        CCSprite* qqSpr1 = CCSprite::create("res/pic/loginScene/login_qq.png");
        CCSprite* qqSpr2 = CCSprite::create("res/pic/loginScene/login_qq.png");
        qqSpr1->setScale(1.01f);
        startItem1 = CCMenuItemSprite::create(qqSpr1, qqSpr2, this, menu_selector(YiJieLoginScene::startCallBack));
        startItem1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 150, DISPLAY->ScreenHeight()* .3f));
        startItem1->setTag(1);
        
        CCSprite* weixinSpr1 = CCSprite::create("res/pic/loginScene/login_wechat.png");
        CCSprite* weixinSpr2 = CCSprite::create("res/pic/loginScene/login_wechat.png");
        weixinSpr2->setScale(1.01f);
        startItem2 = CCMenuItemSprite::create(weixinSpr1, weixinSpr2, this, menu_selector(YiJieLoginScene::startCallBack));
        startItem2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 150, DISPLAY->ScreenHeight()* .3f));
        startItem2->setTag(2);
        
        CCMenu* startMenu = CCMenu::create(startItem1, startItem2, NULL);
        startMenu->setPosition(CCPointZero);
        this->addChild(startMenu, 10);
    }
    
    CCSprite* guangSpr = CCSprite::create("res/pic/loginScene/login_guang.png");
    guangSpr->setPosition(ccp(bg->boundingBox().size.width* .18f, bg->boundingBox().size.height* .22f));
    bg->addChild(guangSpr, 5);
    
    CCFadeTo* fadeTo1 = CCFadeTo::create(.5f, 100);
    CCFadeTo* fadeTo2 = CCFadeTo::create(.5f, 255);
//    CCBlink* blink = CCBlink::create(1.f, 2);
    CCSequence* seq = CCSequence::create(fadeTo1, fadeTo2, NULL);
    guangSpr->runAction(CCRepeatForever::create(seq));
    
    
    CCString* strVersion = CCString::createWithFormat("v%s - %d", CONFIG->version.c_str(), CONFIG->netId);
    CCLabelTTF* lblVersion = CCLabelTTF::create(strVersion->getCString(), DISPLAY->fangzhengFont(), 20.f);
    lblVersion->setColor(ccORANGE);
    lblVersion->setPosition(ccp(DISPLAY->W() * 0.85, DISPLAY->H() * 0.04));
    this->addChild(lblVersion);
    
    for (int i = 1; i <= 6; i++) {
        init_snow(i);
    }
    
}
void YiJieLoginScene::startCallBack(CCObject* pSender){
    LOADING->show_loading();
    CCMenuItem* item = (CCMenuItem*)pSender;
    JNIController::isLanding(item->getTag());
    this->schedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus), .5f);
}
void YiJieLoginScene::updataLoginStatus(float dt){
    if (JNIController::getLandStatus() == 1) {
        JNIController::setLandStatus(0);
        LOADING->remove();
        this->unschedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus));
        if (CONFIG->channelId != 7) {
            startItem->setVisible(false);
        }else{
            startItem1->setVisible(false);
            startItem2->setVisible(false);
        }
        
        CCString* seccionStr;
        seccionStr = CCString::createWithFormat("%s", JNIController::getSessionid().c_str());
        DATA->getLogin()->setUUid(seccionStr);
//        CCLog("<><><><> seccionStr == %s", seccionStr->getCString());
        LOADING->show_loading();
//        DATA->setLoginType(1);
        CONFIG->save_login_type(1);
        NET->fast_login_900(seccionStr->getCString(), CONFIG->channelId);
    }else if (JNIController::getLandStatus() == 2){
        JNIController::setLandStatus(0);
        LOADING->remove();
        this->unschedule(SEL_SCHEDULE(&YiJieLoginScene::updataLoginStatus));
        if (CONFIG->channelId != 7) {
            startItem->setVisible(true);
        }else{
            startItem1->setVisible(true);
            startItem2->setVisible(true);
        }
        
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
            LOADING->remove();
            JNIController::exitGame(CONFIG->baiOrYijie);
        }
    }
}



void YiJieLoginScene::init_snow(int huaIndex){
    CCParticleSnow * snow = CCParticleSnow::create();
    // 自定义雪花图片
    CCString* snowStr = CCString::createWithFormat("res/pic/loginScene/login_hua%d.png", huaIndex);
    CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(snowStr->getCString());
    snow->setTexture(texture);
    // 粒子总数
    snow->setTotalParticles(700);
    // duration
    snow->setDuration(kCCParticleDurationInfinity);
    // set gravity mode.
    snow->setEmitterMode(kCCParticleModeGravity);
    snow->setAngle(-90);
    snow->setAngleVar(5);
    // Gravity Mode: radial acceleration
    snow->setRadialAccel(0);
    snow->setRadialAccelVar(0);
    // 下雪时间秒
    snow->setDuration(kCCParticleDurationInfinity);
    // 雪花开始大小
    if (huaIndex == 1) {
        snow->setStartSize(150.0f);
    }else if (huaIndex == 2){
        snow->setStartSize(125.0f);
    }else if (huaIndex == 3){
        snow->setStartSize(125.0f);
    }else if (huaIndex == 4){
        snow->setStartSize(77.0f);
    }else if (huaIndex == 5){
        snow->setStartSize(98.0f);
    }else if (huaIndex == 6){
        snow->setStartSize(107.0f);
    }
    // 雪花大小浮动
    snow->setStartSizeVar(30.0f);
    // 雪花结束大小
    snow->setEndSize(kCCParticleStartSizeEqualToEndSize);
    snow->setEndSizeVar(0);
    // 初始速度
    snow->setSpeed(30);
    snow->setSpeedVar(5);
    // Gravity Mode: radial
    snow->setRadialAccel(0);
    snow->setRadialAccelVar(1);
    // Gravity mode: tangential
    snow->setTangentialAccel(0);
    snow->setTangentialAccelVar(1);
    // 下降速度 y负数表示下落 x负数表示向西吹
    snow->setGravity(ccp(0, -.8f));
    // 每个雪花生命时间
    snow->setLife(30);
    snow->setLifeVar(10);
    // 释放雪花速度
    snow->setEmissionRate(.2);
    // 动画完成后自动从上层layer中移除粒子
    snow->setAutoRemoveOnFinish(true);
    // 释放粒子位置 如果你想在调用的时候设置位置，那么可以注视下面的代码
    snow->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight() + 15));
    snow->setPosVar(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .1f));
    this->addChild(snow, 50);
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
    // talkingData
    DATA->onEvent("启动事件", "昵称界面", "完成昵称界面");
    
    CCScene* scene = MainScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
