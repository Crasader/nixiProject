
#include "cocos2d.h"
#include "AppDelegate.h"
//#include "SimpleAudioEngine.h"
#include "support/CCNotificationCenter.h"
#include "CCLuaEngine.h"
#include <string>

#include "ConfigManager.h"
#include "DataManager.h"
#include "WSManager.h"
#include "AudioManager.h"
#include "SpecialManager.h"
#include "LogoScene.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

#include "C2DXShareSDK.h"
#include "ShareManager.h"

using namespace cn::sharesdk;

AppDelegate::AppDelegate()
{
    // fixed me
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
}

AppDelegate::~AppDelegate()
{
    // end simple audio engine here, or it may crashed on win32
    SimpleAudioEngine::sharedEngine()->end();
}

void AppDelegate::rand_seed() {
    struct timeval t;
    gettimeofday(&t, NULL);
    int n = t.tv_usec % 9;
    for (int i = 0; i < n; i++) {
        rand();
    }
}

void AppDelegate::write_debug_info()
{
//#if (0 == TARGET_IPHONE_SIMULATOR) //&& (defined DEBUG))
    CCLog("write debug info to file ....");
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("debuginfo.txt");
    freopen(path.c_str(), "a+", stdout);
//#endif
}

void AppDelegate::init_shareSDK() {
//    //设置平台配置
//    //Platforms
//    CCDictionary *totalDict = CCDictionary::create();
//    
//    //微信
//    //微信朋友  iOS别忘在info.plist中注册  移动端上必须要装微信客户端，并且已连接互联网。
//    CCDictionary *wechatConf = CCDictionary::create();
//    wechatConf->setObject(CCString::create("wxad4d9004b6d5777c"), "app_id");
//    wechatConf->setObject(CCString::create("7b6d4e3777540fa2c7ae53714520dacb"), "app_secret");
//    stringstream wechat;
//    wechat << cn::sharesdk::C2DXPlatTypeWechatPlatform;
//    totalDict->setObject(wechatConf, wechat.str());
//    
//    //在 ShareSDK 官网后台注册应用并获取Appkey，并填入此方法的第一个参数中
//    cn::sharesdk::C2DXShareSDK::registerAppAndSetPlatformConfig("16f5723c11eb0", totalDict);
    
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    cn::sharesdk::C2DXShareSDK::open(CCString::create("16f5723c11eb0"), false);
    ShareManager::get_instance()->config();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
#endif
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    pDirector->setProjection(kCCDirectorProjection2D);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);
    //
    init_shareSDK();
    //
    rand_seed();
    //
    float fixed_height = 1136.0f;
    float fixed_width = 852.0f;
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(fixed_width, fixed_height, kResolutionFixedHeight);
    // 配置文件搜索路径
    CCFileUtils::sharedFileUtils()->addSearchPath("res/");
    this->init_lua_env();
    CONFIG->config();
    //
//    write_debug_info();
    //
    AUDIO->play_main_bgm();
    //
    DATA->getLogin()->config_UUID();
    SPECIAL->init();
    const char* uuid = DATA->getLogin()->obtain_UUID();
    CCLog("========UUID=========\n%s\n", uuid);
    // first scene
    CCLog("========LoginScene::scene=========");
    CCScene* pScene = LogoScene::scene();
    CCDirector::sharedDirector()->runWithScene(pScene);
    
    return true;
}

void AppDelegate::init_lua_env() {
    // register lua engine
    CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    
    CCLuaStack *pStack = pEngine->getLuaStack();
    pStack->setXXTEAKeyAndSign("tiegao", strlen("tiegao"), "yunlook", strlen("yunlook"));
/*
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // load framework
    pStack->loadChunksFromZIP("res/framework_precompiled.zip");
    
    // set script path
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename("scripts/main.lua");
#else
    // load framework
    if (m_projectConfig.isLoadPrecompiledFramework())
    {
        const string precompiledFrameworkPath = SimulatorConfig::sharedDefaults()->getPrecompiledFrameworkPath();
        pStack->loadChunksFromZIP(precompiledFrameworkPath.c_str());
    }
    
    // set script path
    string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(m_projectConfig.getScriptFileRealPath().c_str());
#endif
    
    size_t pos;
    while ((pos = path.find_first_of("\\")) != std::string::npos)
    {
        path.replace(pos, 1, "/");
    }
    size_t p = path.find_last_of("/\\");
    if (p != path.npos)
    {
        const string dir = path.substr(0, p);
        pStack->addSearchPath(dir.c_str());
        
        p = dir.find_last_of("/\\");
        if (p != dir.npos)
        {
            pStack->addSearchPath(dir.substr(0, p).c_str());
        }
    }
    
    string env = "__LUA_STARTUP_FILE__=\"";
    env.append(path);
    env.append("\"");
    pEngine->executeString(env.c_str());
    
    CCLOG("------------------------------------------------");
    CCLOG("LOAD LUA FILE: %s", path.c_str());
    CCLOG("------------------------------------------------");
    pEngine->executeScriptFile(path.c_str());
*/
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();
//    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
//    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    AUDIO->pause_music();
    AUDIO->pause_effects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_ENTER_BACKGROUND_EVENT");
    DATA->setChatOut(true);
    WSManager::Inst()->disconnect();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();
//    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
//    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    AUDIO->resume_music();
    AUDIO->resume_all_effects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_ENTER_FOREGROUND_EVENT");
    DATA->setChatOut(false);
    WSManager::Inst()->connect();
}

void AppDelegate::setProjectConfig(const ProjectConfig& config)
{
    m_projectConfig = config;
}
