//
//  ShareManager.cpp
//  mm3c
//
//  Created by lakkey on 16-1-15.
//
//

#include "ShareManager.h"
#include "Loading2.h"
#include "PromptLayer.h"
//#include "MMJNIManager.h"

using namespace cn::sharesdk;

//分享回调
void shareContentResultHandler(int seqId, cn::sharesdk::C2DXResponseState state, cn::sharesdk::C2DXPlatType platType, CCDictionary *result)
{
    switch (state)
    {
        case cn::sharesdk::C2DXResponseStateSuccess:
        {
            CCLog("Success");
        }
            break;
        case cn::sharesdk::C2DXResponseStateFail:
        {
            CCLog("Fail");
            //回调错误信息
            CCArray *allKeys = result->allKeys();
            allKeys->retain();
            for (int i = 0; i < allKeys-> count(); i++)
            {
                CCString *key = (CCString*)allKeys->objectAtIndex(i);
                CCObject *obj = result->objectForKey(key->getCString());
                
                CCLog("key = %s", key -> getCString());
                if (dynamic_cast<CCString *>(obj))
                {
                    CCLog("value = %s", dynamic_cast<CCString *>(obj) -> getCString());
                }
                else if (dynamic_cast<CCInteger *>(obj))
                {
                    CCLog("value = %d", dynamic_cast<CCInteger *>(obj) -> getValue());
                }
                else if (dynamic_cast<CCDouble *>(obj))
                {
                    CCLog("value = %f", dynamic_cast<CCDouble *>(obj) -> getValue());
                }
            }
        }
            break;
        case cn::sharesdk::C2DXResponseStateCancel:
        {
            CCLog("Cancel");
        }
            break;
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

ShareManager* ShareManager::Inst()
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
//    CCDictionary *wcConfigDict = CCDictionary::create();
//    wcConfigDict -> setObject(CCString::create(""), "app_id");
//    wcConfigDict -> setObject(CCString::create(""), "app_secret");
//    wcConfigDict -> setObject(CCString::create("http://www.sharesdk.cn"), "redirect_uri");
//    C2DXShareSDK::registerAppAndSetPlatformConfig("wxad4d9004b6d5777c", wcConfigDict);
}

void ShareManager::show_share_menu(CCDictionary* content) {
    
    if (! C2DXShareSDK::isClientValid(C2DXPlatTypeWeChatMoments)) {
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "未安装微信客户端，无法使用分享~!");
        SHARE->stop_share();
        return;
    }
    
    // 第4，5个参数传入 iPad 视图要显示的坐标点，详见API说明
    C2DXShareSDK::showShareMenu(NULL, content, 100, 100, shareContentResultHandler);
}

void ShareManager::share_pic() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CCDictionary *content = CCDictionary::create();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append(TEMPORARY_SHARE_PIC_NAME);
    
    if (CCFileUtils::sharedFileUtils()->isFileExist(path.c_str())) {
        content -> setObject(CCString::create(path), "image");
        content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
        this->show_share_menu(content);
    }
    else {
        SHARE->stop_share();
    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    MMJNIManager::showShare();
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this, .1f, false);
#endif
}

void ShareManager::shareStatus(float dt){
//    CCLog("<><><><>shareStatus == %d", MMJNIManager::getShareStatus());
//    if (MMJNIManager::getShareStatus() == 1) {
//        MMJNIManager::shareText();
//        MMJNIManager::setShareStatus(0);
//        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this, 0.1, 1, 2.f, false);
//        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this);
//    }else if (MMJNIManager::getShareStatus() == 2 || MMJNIManager::getShareStatus() == 3){
//        MMJNIManager::shareText();
//        MMJNIManager::setShareStatus(0);
//        CCNotificationCenter::sharedNotificationCenter()->postNotification("NC_STOP_SHARE");
//        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::shareStatus), this);
//    }
}

void ShareManager::add_event_listener() {
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&ShareManager::notification_4500_callback), "NC_ON_SHARE", NULL);
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
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NC_STOP_SHARE");
}

void ShareManager::did_share_success() {
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this, 0.1, 1, 2.f, false);
}

void ShareManager::delay_share_success() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&ShareManager::delay_share_success), this);
//    LOADING->show_loading();
//    MMNetManager::get_instance()->http_4500_share_success();
}

//void ShareManager::notification_4500_callback(CCObject* pObj){
//    if (AHLoading::isLoading) {
//        AHLoading::stop();
//    }
//    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->postNotification(NOTIFICATION_Share_Rewards, pObj);
//}

void ShareManager::test() {    
//    reqID += 1; // 分享计数
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("分享文本"), "text");  // 分享文本
    content -> setObject(CCString::create("HelloWorld.png"), "image");// 分享图片
    content -> setObject(CCString::create("测试标题"), "title"); // 分享标题
    content -> setObject(CCString::create("http://www.mob.com"), "url"); // 分享url
    content -> setObject(CCString::createWithFormat("%d", cn::sharesdk::C2DXContentTypeWebPage), "type"); // 分享类型

    this->show_share_menu(content);
}

