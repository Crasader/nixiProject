//
//  ShareManager.cpp
//  mm3c
//
//  Created by lakkey on 16-1-15.
//
//

#include "ShareManager.h"
#include "PromptLayer.h"
#include "JNIController.h"
#include "Loading2.h"
#include "NetManager.h"

void ShareManager::test() {
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("这是一条测试内容"), "content");
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/test.png");
    if (CCFileUtils::sharedFileUtils()->isFileExist(path.c_str())) {
        content -> setObject(CCString::create(path), "image");
    }
    content -> setObject(CCString::create("测试标题"), "title");
    content -> setObject(CCString::create("测试描述"), "description");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
    
    this->show_share_menu(content);
}

void ShareManager::share_pic() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CCDictionary *content = CCDictionary::create();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    if (CCFileUtils::sharedFileUtils()->isFileExist(path.c_str())) {
        content -> setObject(CCString::create(path), "image");
        content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
        this->show_share_menu(content);
    }
    else {
        ShareManager::get_instance()->stop_share();
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    JNIController::showShare();
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this, .1f, false);
#endif
    
}

#pragma Inner

void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateCancel: {
            CCLog("ShareManager::取消");
            ShareManager::get_instance()->stop_share();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("IOS_SHARE_FINISH");
        } break;
        
        case C2DXResponseStateSuccess: {
            CCLog("ShareManager::分享成功");
            ShareManager::get_instance()->did_share_success();
            CCNotificationCenter::sharedNotificationCenter()->postNotification("IOS_SHARE_FINISH");
        } break;
        
        case C2DXResponseStateFail: {
            CCLog("ShareManager::分享失败");
            ShareManager::get_instance()->stop_share();
        } break;
        
        default:
            break;
    }
}

static ShareManager* _shared_manager = NULL;

ShareManager::~ShareManager()
{
    if (_shared_manager) {
        this->remove_event_listener();
        delete _shared_manager;
        _shared_manager = NULL;
    }
}

ShareManager* ShareManager::get_instance()
{
    if (!_shared_manager) {
        _shared_manager = new ShareManager();
        _shared_manager->init();
    }
    
    _shared_manager->add_event_listener();
    return _shared_manager;
}

void ShareManager::init()
{
    
}

void ShareManager::config() {
//    CCDictionary *sinaConfigDict = CCDictionary::create();
//    sinaConfigDict -> setObject(CCString::create("568898243"), "app_key");
//    sinaConfigDict -> setObject(CCString::create("38a4f8204cc784f81f9f0daaf31e02e3"), "app_secret");
//    sinaConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSinaWeibo, sinaConfigDict);
    
    //微信朋友  iOS别忘在info.plist中注册  移动端上必须要装微信客户端，并且已连接互联网。
    CCDictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create("wxad4d9004b6d5777c"), "app_id");
    wcConfigDict -> setObject(CCString::create("7b6d4e3777540fa2c7ae53714520dacb"), "app_secret");
    wcConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
}

void ShareManager::show_share_menu(CCDictionary* content) {
    if (! C2DXShareSDK::isClientInstalled(C2DXPlatTypeWeixiTimeline)) {
        PromptLayer* layer = PromptLayer::create();
        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "未安装微信客户端，无法使用分享!~");
        
        ShareManager::get_instance()->stop_share();
        return;
    }
    
    C2DXShareSDK::showShareView(C2DXPlatTypeWeixiTimeline, content, shareResultHandler);
//    C2DXShareSDK::showShareMenu(NULL, content, NULL);
//    C2DXShareSDK::showShareMenu(NULL, content, ccp(300, 500), C2DXMenuArrowDirectionDown, NULL);
}



void ShareManager::shareStatus(float dt){
    CCLog("<><><><>shareStatus == %d", JNIController::getShareStatus());
    if (JNIController::getShareStatus() == 1) {
//        JNIController::shareText();
        JNIController::setShareStatus(0);
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this, 0.1, 1, 2.f, false);
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this);
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
//        JNIController::shareText();
        JNIController::setShareStatus(0);
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this);
    }
}

void ShareManager::add_event_listener() {
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&ShareManager::notification_4500_callback), NOTIFICATION_4500_DONE, NULL);
}

void ShareManager::remove_event_listener() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

//void ShareManager::notification_http_error(cocos2d::CCObject *pObj) {
//    AHLoading::stop();
//    
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_Button_errorCommon, false);
//    }
//    
//    long err = (long)pObj;
//    CCString* error_message = MZAppUtils::http_error_message_with_status_code((HTTP_ERROR)err);
//    MMPromptLayer* promptLayer = MMPromptLayer::create();
//    promptLayer->promptBox(error_message);
//    this->addChild(promptLayer, 1000);
//}

void ShareManager::stop_share() {
//    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_Share_Stop);
}

void ShareManager::did_share_success() {
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this, 0.1, 1, 2.f, false);
}

void ShareManager::delay_share_success() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this);
//    AHLoading::showLoading();
//    MMNetManager::get_instance()->http_4500_share_success();
}

void ShareManager::notification_4500_callback(CCObject* pObj){
//    if (AHLoading::isLoading) {
//        AHLoading::stop();
//    }
//    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->postNotification(NOTIFICATION_Share_Rewards, pObj);
}

