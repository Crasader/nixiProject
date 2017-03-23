//
//  MainScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "MainScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "QingjingScene.h"
#include "StoryScene.h"
#include "VipQingjingScene.h"
#include "TaskScene.h"
#include "ClothesScene.h"
#include "NoticeManager.h"
#include "AudioManager.h"
#include "WSManager.h"
#include "GashaponScene.h"
#include "SalesPromotionLayer.h"
#include "MonthCardLayer.h"

//#include "HaoyouRankLayer.h"
#include "Shower.h"

#include "StrangerScene.h"
//#include "TotalRankScene.h"
#include "RankListScene.h"
#include "HaoyouScene.h"

#include "DragLayer.h"
#include "PromptLayer.h"

#include "Loading2.h"
#include "NetManager.h"
#include "ConfigManager.h"

#include "MailPanel.h"
#include "OperationPanel.h"
#include "ChatPanel.h"
#include "TotalRechargePanel.h"
#include "Signin7Panel.h"
#include "NewSignin7Panel.h"
#include "HomeLayer.h"
#include "EnergyLargessPanel.h"
#include "TempSignin.h"
#include "MysteryLayer.h"
#include "TrystScene.h"
#include "TrystProgress.h"
#include "DailySigninRewardPanel.h"
#include "WelfarePanel.h"
#include "pkScene.h"

#include "AppUtil.h"

#include <time.h>

#include "RewardLayer.h"
#include "AHMessageBox.h"
#include "GuideLayer.h"

#include "JNIController.h"
#include "TDCCAccount.h"


// --------------- test ----------------



MainScene::MainScene(){
    
}
MainScene::~MainScene(){
    CC_SAFE_DELETE(_arrGroup1);
    CC_SAFE_DELETE(_arrGroup2);
    CC_SAFE_DELETE(_arrPlay);
}

bool MainScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    num_child = 0;
    
    
    if (DATA->getInit_TalkingBool()) {
        DATA->setInit_TalkingBool(false);
        // talkingData初始化玩家信息
        CCString* accountStr = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        accountStr = CCString::createWithFormat("%s", DATA->getLogin()->obtain_sid());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        accountStr = CCString::createWithFormat("%s", DATA->getLogin()->obtain_sid());
#endif
        if (accountStr != NULL) {
            CCString* diamStr = CCString::createWithFormat("初始化赠予%d钻石", DATA->getPlayer()->diam);
            DATA->onReward(DATA->getPlayer()->diam, diamStr->getCString());
            
            DATA->setTaskTalkingdataID(DATA->getPlayer()->mission - 1);
            
            CCString* accountNameStr = CCString::createWithFormat("%s", DATA->getShow()->nickname());
            TDCCAccount* account = TDCCAccount::setAccount(accountStr->getCString());
            account->setGender(TDCCAccount::TDCCGender::kGenderUnknown);
            account->setAccountType(TDCCAccount::kAccountRegistered);
            account->setLevel(DATA->getTaskTalkingdataID());
            account->setAccountName(accountNameStr->getCString());
        }
    }else{
        // talkingData初始化玩家信息
        CCString* accountStr = NULL;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        accountStr = CCString::createWithFormat("%s", DATA->getLogin()->obtain_sid());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        accountStr = CCString::createWithFormat("%s", DATA->getLogin()->obtain_sid());
#endif
        DATA->setTaskTalkingdataID(DATA->getPlayer()->mission - 1);
        
        if (accountStr != NULL) {
            
            DATA->setTaskTalkingdataID(DATA->getPlayer()->mission - 1);
            
            TDCCAccount* account = TDCCAccount::setAccount(accountStr->getCString());
            account->setLevel(DATA->getTaskTalkingdataID());
        }
    }
    
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->creat_guideBool();
    
    _arrGroup1 = NULL;
    _arrGroup2 = NULL;
    _arrPlay = NULL;
    isrenwuBool = false;
    ishomeBool = false;
    
    
//    _ManSpr = CCSprite::create();
//    this->addChild(_ManSpr, 10);
//    
//    allClothesDic = CONFIG->clothes();// 所有衣服
//    
    this->creat_view();
//    this->creat_Man();
//    this->initClothes();
//    
//    this->schedule(schedule_selector(MainScene::gengxin), .1f);
    
    this->scheduleUpdate();
    
    isEffective = true;
    move_x = 0;
    
    opacity = 200;
    sp = 1.0;
    if (DATA->current_guide_step() == 6) {
        isOpen = false;
    }else{
        isOpen = true;
    }
    
    
    
    
    if (DATA->current_guide_step() == 0) {
        
    }else if (DATA->current_guide_step() == 1){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->current_guide_step());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 5){
        DATA->getPlayer()->setGuide(100);
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->getPlayer()->getGuide());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 6){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->getPlayer()->getGuide());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }else if (DATA->current_guide_step() == 8){
        GuideLayer* layer = GuideLayer::create_with_guide(DATA->getPlayer()->getGuide());
        layer->setTag(0x445566);
        this->addChild(layer, 500);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CONFIG->baiOrYijie == 0) {// 白包
        
    }else if (CONFIG->baiOrYijie == 1){// 易接
        JNIController::setUserId(DATA->getLogin()->obtain_sid());
        JNIController::setPlayerName(DATA->getShow()->nickname());
        CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
        JNIController::setPlayerLevel(phaseStr->getCString());
        CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
        JNIController::setPlayerGold(diamStr->getCString());
        
        bool creatNameBool = CCUserDefault::sharedUserDefault()->getBoolForKey("CreatName");
        if (!creatNameBool) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("CreatName", true);
            JNIController::setData(1);
        }else{
            JNIController::setData(5);
            this->scheduleOnce(SEL_SCHEDULE(&MainScene::setStartGameData), .5f);
        }
        JNIController::isExtendData();
    }else if (CONFIG->baiOrYijie == 2){// 鱼丸
        JNIController::setUserId(DATA->getLogin()->obtain_sid());
        JNIController::setPlayerName(DATA->getShow()->nickname());
        CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
        JNIController::setPlayerLevel(phaseStr->getCString());
        CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
        JNIController::setPlayerGold(diamStr->getCString());
        
        bool creatNameBool = CCUserDefault::sharedUserDefault()->getBoolForKey("CreatName");
        if (!creatNameBool) {
            CCUserDefault::sharedUserDefault()->setBoolForKey("CreatName", true);
            JNIController::setData(1);
        }else{
            if (DATA->getYuwanBool()) {
            }else{
                DATA->setYuwanBool(true);
                this->scheduleOnce(SEL_SCHEDULE(&MainScene::setStartGameData), .5f);
            }
        }
    }else if (CONFIG->baiOrYijie == 3) {// 单独支付
        JNIController::setUserId(DATA->getLogin()->obtain_sid());
    }
#endif
    
    
    struct timeval star;
    struct tm* time;
    gettimeofday(&star,  NULL);
    time = localtime(&star.tv_sec);
    std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("Tm_MDay", "0");
    int num = atoi(str.c_str());
    if (num < time->tm_mday) {
        
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_MDay", CCString::createWithFormat("%d", time->tm_mday)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_One", CCString::createWithFormat("%d", 0)->getCString());
        CCUserDefault::sharedUserDefault()->setStringForKey("Tm_Ten", CCString::createWithFormat("%d", 0)->getCString());
    }
    
    
    
    
//    time_t t;
//    struct tm *p;
//    t = 1467241111;
//    p = gmtime(&t);
//    char s[80];
//    strftime(s, 80, "%Y-%m-%d %H:%M:%S", p);
//    CCLog("<><><><>time == %d: %s\n", (int)t, s);
//    
//    struct cc_timeval now;
//    CCTime::gettimeofdayCocos2d(&now, NULL);
//    CCLog("<><><><> time == %ld", now.tv_sec * 1000 + now.tv_usec / 1000);
    
    
//    this->scheduleOnce(SEL_SCHEDULE(&MainScene::creat_160), 0.3f);
    
    return true;
}
void MainScene::setStartGameData(float dt){
    JNIController::setData(3);
}

CCScene* MainScene::scene(){
    CCScene* scene = CCScene::create();
    MainScene* layer = MainScene::create();
    scene->addChild(layer);
    
    if (DATA->current_guide_step() == 0 || DATA->current_guide_step() == 9) {
        DragLayer* drag = DragLayer::create();
        scene->addChild(drag);
    }
    
    return scene;
}

void MainScene::onEnter(){
    BaseScene::onEnter();
    BaseScene::openChat(true);
    
    this->setAccelerometerEnabled(true); // ?
    
    CCString* strBGM = AUDIO->getCurBGM();
    if (strBGM && strBGM->compare("MAIN") == 0) {
        
    }
    else {
        AUDIO->play_main_bgm();
    }
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_huanzhuangCallBack), "HTTP_FINISHED_400", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_500CallBack), "HTTP_FINISHED_500", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_504CallBack), "HTTP_FINISHED_504", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_mail_callback_700), "HTTP_FINISHED_700", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_704CallBack), "HTTP_FINISHED_704", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_600CallBack), "HTTP_FINISHED_600", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::social_info_callback_800), "HTTP_FINISHED_800", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::all_friends_callback_806), "HTTP_FINISHED_806", NULL);
//    nc->addObserver(this, SEL_CallFuncO(&MainScene::rankList_callback_300), "HTTP_FINISHED_300", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::competition_callback_820), "HTTP_FINISHED_820", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_signin_info_312), "HTTP_FINISHED_312", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_recharge_info_304), "HTTP_FINISHED_304", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_gashapon_info_306), "HTTP_FINISHED_306", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_905CallBack), "HTTP_FINISHED_905", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_fetch_mystery_info_610), "HTTP_FINISHED_610", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::after_fetch_tryst_info_620), "HTTP_FINISHED_620", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::after_start_tryst_621), "HTTP_FINISHED_621", NULL);

    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_welfare_user_info), "HTTP_FINISHED_630", NULL);
    
    // 节日临时签到
    nc->addObserver(this, SEL_CallFuncO(&MainScene::nc_temp_signin_info_340), "HTTP_FINISHED_340", NULL);
    
    // 限时购买
    nc->addObserver(this, SEL_CallFuncO(&MainScene::_160CallBack), "HTTP_FINISHED_160", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::update_news_status), "UPDATE_NEWS_STATUS", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::check_free_gashapon), "CHECK_FREE_GASHAPON", NULL);
    
    // 监听是否从7日签到退出
    nc->addObserver(this, SEL_CallFuncO(&MainScene::check_dailysignin), "WHEN_SIGNIN7_EXIT", NULL);
    // 是否从每日签到退出
    nc->addObserver(this, SEL_CallFuncO(&MainScene::purchaseAchievementCallBack), "DAILYSIGNIN_PANEL_EXIT", NULL);
    
    // 从别处调用签到
    nc->addObserver(this, SEL_CallFuncO(&MainScene::qiandaoCallBack), "NEED_SHOW_SIGNIN7", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::gashaponCallBack), "NEED_SHOW_GASHAPON", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::purchaseAchievementCallBack), "NEED_SHOW_RECHARTE", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::energyLargessCallBack), "NEED_SHOW_ENERGY_GARLESS", NULL);
    
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::linshiMethod), "linshiMethod", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&MainScene::check_begin_position), "TOUCH_BEGIN", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::change_position), "DRAGING", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::setIsEffective), "EFFECTIVE", NULL);
//    nc->addObserver(this, SEL_CallFuncO(&MainScene::displayChatItem), "ON_CHAT_PANEL_CLOSE", NULL);
    
    
    // guide
    nc->addObserver(this, SEL_CallFuncO(&MainScene::richangMethods), "GuideRichangMethods", NULL);
    nc->addObserver(this, SEL_CallFuncO(&MainScene::isTxt_Bar), "GuideIsTxt_Bar", NULL);
    // 检查最新状态
    this->update_news_status();
    // 检查是否有可用的免费抽奖
    this->check_free_gashapon();
    //
    this->schedule(SEL_SCHEDULE(&MainScene::check_welfare_new), 1);
    
    this->setArrPlay(this->rand_array(_arrGroup1));
    this->schedule(SEL_SCHEDULE(&MainScene::delayPlay), 10, kCCRepeatForever, 3);
    
//    int count = arr->count();
//    for (int i = 0; i < count; i++) {
//        CCSprite* spt = (CCSprite*)arr->objectAtIndex(i);
//        this->play(spt);
//    }

    this->scheduleOnce(SEL_SCHEDULE(&MainScene::keyBackStatus), .8f);
    
    if (DATA->current_guide_step() == 0) {
//        // 节日临时签到
//        if (DATA->getNews()->tempSignin == 1) {
//            this->show_guoqing_signin();
//        }
        if (DATA->getNews()->signin7 == 1) {
            isOk = true;
            this->qiandaoCallBack(NULL);
        }
        else {
            this->check_dailysignin();
        }
    }
    
    this->checkVersion();
    
    this->showTrystEntrance();
//    if (DATA->getNews()->trystOn == 1) {
//        
//    }
    NET->fetch_tryst_info_620();
    
    // test
//    NET->buy_monthly_card1_151();
//    NET->take_monthly_card1_daily_reward_153();
//    NET->before_send_shout_831();
}

void MainScene::checkVersion() {
    CCDictionary* mainConf = DATA->getLogin()->config();
    CCInteger* versionFlag = (CCInteger*)mainConf->objectForKey("ver");
    if (NULL != versionFlag) {
        int isNewVersion = versionFlag->getValue();
        if (0 != isNewVersion) {
            AHMessageBox* mb = AHMessageBox::create_with_message("检测到您当前不是最新版本!\n请前往应用商店更新~!", NULL, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YES, false);
            mb->setPosition(DISPLAY->center());
            this->addChild(mb, 3900);
        }
    }
}

void MainScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void MainScene::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
//    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
//    DATA->setChatOut(true);
//    DATA->getChat()->setItems(CCArray::create());
    
    BaseScene::onExit();
}

void MainScene::keyBackClicked(){
    CCLog("===== MMChooseLayer::keyBackClicked");
    if (DATA->current_guide_step() == 0 || DATA->current_guide_step() >= 7) {
        num_child++;
        CCLog("===== MainScene  children_num: %d", num_child);
        if (num_child> 1) {
            num_child = 0;
            return;
        }
        
        if (CONFIG->baiOrYijie == 0) {// 白包
            JNIController::setUserId(DATA->getLogin()->obtain_sid());
            JNIController::setPlayerName(DATA->getShow()->nickname());
            CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
            JNIController::setPlayerLevel(phaseStr->getCString());
            CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
            JNIController::setPlayerGold(diamStr->getCString());
            JNIController::setData(4);
            
            num_child = 0;
            JNIController::exitGame(0);
        }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2){// 易接
            JNIController::setUserId(DATA->getLogin()->obtain_sid());
            JNIController::setPlayerName(DATA->getShow()->nickname());
            CCString* phaseStr = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
            JNIController::setPlayerLevel(phaseStr->getCString());
            CCString* diamStr = CCString::createWithFormat("%d", DATA->getPlayer()->diam);
            JNIController::setPlayerGold(diamStr->getCString());
            JNIController::setData(4);
            
            num_child = 0;
            JNIController::exitGame(1);
        }else if (CONFIG->baiOrYijie == 3){
            num_child = 0;
            JNIController::exitGame(1);
        }
    }
}

void MainScene::didAccelerate( CCAcceleration* pAccelerationValue){
    float deceleration = 0 ;
    float sensitivity = 500;
    
    playerVelocity.x = playerVelocity.x * deceleration+ pAccelerationValue->x*sensitivity;
    playerVelocity.x = playerVelocity.x / 10;
}

void MainScene::gengxin(float dt){
    if (roomSpr == NULL) {
        return;
    }
    
    CCPoint pos = roomSpr->getPosition();
    pos.x = DISPLAY->ScreenWidth()* 0.5 + playerVelocity.x;
    if (pos.x < DISPLAY->ScreenWidth()* 0.5 - 70) {
        pos.x = DISPLAY->ScreenWidth()* 0.5 - 70;
        playerVelocity = CCPointZero;
    }
    else if (pos.x > DISPLAY->ScreenWidth()* 0.5 + 70){
        pos.x = DISPLAY->ScreenWidth()* 0.5 + 70;
        playerVelocity = CCPointZero;
    }
    roomSpr->runAction(CCMoveTo::create(0.1f, ccp(pos.x, pos.y)));
}

void MainScene::creat_view(){
    
    _arrGroup1 = CCArray::create();
    _arrGroup1->retain();
    _arrGroup2 = CCArray::create();
    _arrGroup2->retain();
    
//    // 首冲
//    CCSprite* scSpr1 = CCSprite::create("res/pic/mainScene/main_chong.png");
//    CCSprite* scSpr2 = CCSprite::create("res/pic/mainScene/main_chong.png");
//    scSpr2->setScale(1.02f);
//    CCMenuItem* shouchongItem = CCMenuItemSprite::create(scSpr1, scSpr2, this, menu_selector(MainScene::shouchongCallBack));
//    shouchongItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .63f));

    // 活动
    CCSprite* hdSpr1 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    CCSprite* hdSpr2 = CCSprite::create("res/pic/mainScene/main_huodong.png");
    hdSpr2->setScale(1.02f);
    _welfareItem = CCMenuItemSprite::create(hdSpr1, hdSpr2, this, menu_selector(MainScene::welfareCallBack));

    // 签到
    CCSprite* qdSpr1 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    CCSprite* qdSpr2 = CCSprite::create("res/pic/mainScene/main_qiandao.png");
    qdSpr2->setScale(1.02f);
    _qiandaoItem = CCMenuItemSprite::create(qdSpr1, qdSpr2, this, menu_selector(MainScene::qiandaoCallBack));
//    qiandaoItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .45f));

    // 好友
//    CCSprite* hySpr1 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
//    CCSprite* hySpr2 = CCSprite::create("res/pic/mainScene/main_haoyou.png");
//    hySpr2->setScale(1.02f);
//    _haoyouItem = CCMenuItemSprite::create(hySpr1, hySpr2, this, menu_selector(MainScene::haoyouCallBack));
//    _haoyouItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .9f));

    // 换装
    CCSprite* hzSpr1 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    CCSprite* hzSpr2 = CCSprite::create("res/pic/mainScene/main_huanzhuang.png");
    hzSpr2->setScale(1.02f);
    CCMenuItem* huanzhuangItem = CCMenuItemSprite::create(hzSpr1, hzSpr2, this, menu_selector(MainScene::huanzhuangCallBack));
//    huanzhuangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .81f));
    
    // 排行
//    CCSprite* phSpr1 = CCSprite::create("res/pic/mainScene/main_paihang.png");
//    CCSprite* phSpr2 = CCSprite::create("res/pic/mainScene/main_paihang.png");
//    phSpr2->setScale(1.02f);
//    CCMenuItem* paihangItem = CCMenuItemSprite::create(phSpr1, phSpr2, this, menu_selector(MainScene::paihangCallBack));
//    paihangItem->setPosition(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .72f));
    
    // 体力福利
    CCSprite* energyLargess1 = CCSprite::create("res/pic/mainScene/btn_energylargess.png");
    CCSprite* energyLargess2 = CCSprite::create("res/pic/mainScene/btn_energylargess.png");
    energyLargess2->setScale(1.02f);
    _btnEnergyLargess = CCMenuItemSprite::create(energyLargess1, energyLargess2, this, menu_selector(MainScene::energyLargessCallBack));
//    btnEnergyLargess->setPosition(qiandaoItem->getPosition() - ccp(0, DISPLAY->ScreenHeight()* 0.09f));
    
    // 充值成就
    CCSprite* purchaseAchievement1 = CCSprite::create("res/pic/mainScene/btn_purchaseachievement.png");
    CCSprite* purchaseAchievement2 = CCSprite::create("res/pic/mainScene/btn_purchaseachievement.png");
    purchaseAchievement2->setScale(1.02f);
    CCMenuItem* btnPurchaseAchievement = CCMenuItemSprite::create(purchaseAchievement1, purchaseAchievement2, this, menu_selector(MainScene::purchaseAchievementCallBack));
//    btnPurchaseAchievement->setPosition(btnEnergyLargess->getPosition() - ccp(0, DISPLAY->ScreenHeight()* 0.09f));
    
    // 扭蛋
    CCSprite* gashapon1 = CCSprite::create("res/pic/mainScene/btn_gashapon.png");
    CCSprite* gashapon2 = CCSprite::create("res/pic/mainScene/btn_gashapon.png");
    gashapon2->setScale(1.02f);
    _btnGashapon = CCMenuItemSprite::create(gashapon1, gashapon2, this, menu_selector(MainScene::gashaponCallBack));
    
    // 事件
    CCMenuItem* eventItem = NULL;
    CCSprite* eventSpr1 = CCSprite::create("res/pic/mainScene/main_mystery.png");
    CCSprite* eventSpr2 = CCSprite::create("res/pic/mainScene/main_mystery.png");
    eventSpr2->setScale(1.02f);
    eventItem = CCMenuItemSprite::create(eventSpr1, eventSpr2, this, menu_selector(MainScene::onEventCallback));
    

    // PK
//    CCSprite* pk = CCSprite::create("res/pic/mainScene/main_pk1.png");
//    CCSprite* pk2 = CCSprite::create("res/pic/mainScene/main_pk1.png");
//    pk2->setScale(1.02f);
//    CCMenuItem* pkItem = CCMenuItemSprite::create(pk, pk2, this, menu_selector(MainScene::onPkCalled));
//    pkItem->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
//    
//    CCAnimate* anim = CCAnimate::create(AppUtil::animationWithPics("res/pic/mainScene/main_pk%d.png", 2, 1, 0.5f));
//    pk->runAction(CCRepeatForever::create(anim));
    
    
    // 限时抢购
    CCSprite* xianshiSpr1 = CCSprite::create("res/pic/mainScene/xianshicuxiao.png");
    CCSprite* xianshiSpr2 = CCSprite::create("res/pic/mainScene/xianshicuxiao.png");
    xianshiSpr2->setScale(1.02f);
    CCMenuItem* pkItem = CCMenuItemSprite::create(xianshiSpr1, xianshiSpr2, this, menu_selector(MainScene::xianshiCallBack));
    pkItem->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
    
    
    
    // 月卡
    CCSprite* monthSpr1 = CCSprite::create("res/pic/mainScene/main_month.png");
    CCSprite* monthSpr2 = CCSprite::create("res/pic/mainScene/main_month.png");
    monthSpr2->setScale(1.02f);
    CCMenuItem* monthItem = CCMenuItemSprite::create(monthSpr1, monthSpr2, this, menu_selector(MainScene::monthCallBack));
    monthItem->setPosition(ccp(DISPLAY->ScreenWidth()* .075f, DISPLAY->ScreenHeight()* .19f));
    
    
    
//    //设置
//    CCSprite* szSpr1 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
//    CCSprite* szSpr2 = CCSprite::create("res/pic/mainScene/main_shezhi.png");
//    szSpr2->setScale(1.02f);
//    _shezhiItem = CCMenuItemSprite::create(szSpr1, szSpr2, this, menu_selector(MainScene::shezhiCallBack));
//    _shezhiItem->setPosition(ccp(DISPLAY->ScreenWidth()* .09f, DISPLAY->ScreenHeight()* .05f));

    
    //new-----------------------------------new
    
    //-----6层天空------
    _layer_6 = CCSprite::create("res/pic/mainScene/far.png");
    _layer_6->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_6);
    
    //-----云------
    _cloud_1 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_1->setPosition(ccp(_layer_6->getContentSize().width* .5f, _layer_6->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    _layer_6->addChild(_cloud_1);
    
    _cloud_2 = CCSprite::create("res/pic/mainScene/cloud.png");
    _cloud_2->setPosition(ccp(_cloud_1->getPositionX() + _cloud_2->getContentSize().width, _layer_6->getContentSize().height - _cloud_1->getContentSize().height* .5f + 50));
    _layer_6->addChild(_cloud_2);
    
    //-----鸟------
    _bird_1 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_1->setPosition(ccp(_layer_6->getContentSize().width* .9f, _layer_6->getContentSize().height* .77));//0.25f
    _bird_1->setScale(0.3f);
    _layer_6->addChild(_bird_1);
    
    CCAnimation* animation = CCAnimation::create();
    for (int i = 0; i < 5; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/bird_%d.png", i + 1);
        animation->addSpriteFrameWithFileName(str->getCString());
    }
    animation->setDelayPerUnit(0.22f);
    animation->setRestoreOriginalFrame(true);
    
    CCAnimate* ac1 = CCAnimate::create(animation);
    CCRepeatForever* rep = CCRepeatForever::create(CCSequence::create(ac1, ac1->reverse(), NULL));
    _bird_1->runAction(rep);
    
    _bird_2 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_2->setPosition(ccp(_layer_6->getContentSize().width* .4f, _layer_6->getContentSize().height* .71));
    _bird_2->setScale(0.2f);
    _layer_6->addChild(_bird_2);
    animation->setDelayPerUnit(0.18f);
    CCAnimate* ac2 = CCAnimate::create(animation);
    CCRepeatForever* rep2 = CCRepeatForever::create(CCSequence::create(ac2, ac2->reverse(), NULL));
    _bird_2->runAction(rep2);
    
    _bird_3 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_3->setPosition(ccp(_layer_6->getContentSize().width* .75, _layer_6->getContentSize().height* .75));
    _bird_3->setScale(0.4f);
    _layer_6->addChild(_bird_3);
    animation->setDelayPerUnit(0.19f);
    CCAnimate* ac3 = CCAnimate::create(animation);
    CCRepeatForever* rep3 = CCRepeatForever::create(CCSequence::create(ac3, ac3->reverse(), NULL));
    _bird_3->runAction(rep3);
    
    _bird_4 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_4->setPosition(ccp(_layer_6->getContentSize().width* .85, _layer_6->getContentSize().height* .8));
    _bird_4->setScale(0.3f);
    _layer_6->addChild(_bird_4);
    animation->setDelayPerUnit(0.21f);
    CCAnimate* ac4 = CCAnimate::create(animation);
    CCRepeatForever* rep4 = CCRepeatForever::create(CCSequence::create(ac4, ac4->reverse(), NULL));
    _bird_4->runAction(rep4);
    
    _bird_5 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_5->setPosition(ccp(_layer_6->getContentSize().width* .45f, _layer_6->getContentSize().height* .77));
    _bird_5->setScale(0.4f);
    _layer_6->addChild(_bird_5);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac5 = CCAnimate::create(animation);
    CCRepeatForever* rep5 = CCRepeatForever::create(CCSequence::create(ac5, ac5->reverse(), NULL));
    _bird_5->runAction(rep5);
    
    _bird_6 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_6->setPosition(ccp(_layer_6->getContentSize().width + 100, _layer_6->getContentSize().height* .73));
    _bird_6->setScale(0.2f);
    _layer_6->addChild(_bird_6);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac6 = CCAnimate::create(animation);
    CCRepeatForever* rep6 = CCRepeatForever::create(CCSequence::create(ac6, ac6->reverse(), NULL));
    _bird_6->runAction(rep6);
    
    _bird_7 = CCSprite::create("res/pic/mainScene/bird_1.png");
    _bird_7->setPosition(ccp(_layer_6->getContentSize().width* .3f, _layer_6->getContentSize().height* .76));
    _bird_7->setScale(0.3f);
    _layer_6->addChild(_bird_7);
    animation->setDelayPerUnit(0.1f);
    CCAnimate* ac7 = CCAnimate::create(animation);
    CCRepeatForever* rep7 = CCRepeatForever::create(CCSequence::create(ac7, ac7->reverse(), NULL));
    _bird_7->runAction(rep7);
    
    //-----5层河流------
    _river_1 = CCSprite::create("res/pic/mainScene/river.png");
    _river_1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_river_1);
    
    _river_2 = CCSprite::create("res/pic/mainScene/river.png");
    _river_2->setPosition(ccp(_river_1->getPositionX() + _river_2->getContentSize().width - 5, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_river_2);
    
    //-----4层背景-----
    _layer_4 = CCSprite::create("res/pic/mainScene/four_bg.png");
    _layer_4->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f - 1));
    this->addChild(_layer_4);
    
    //---塔(排行榜)---
    CCSprite* tower_spr1 = CCSprite::create("res/pic/mainScene/paihang.png");
    CCSprite* tower_spr2 = CCSprite::create("res/pic/mainScene/paihang.png");
    
    paihang_bar1 = CCSprite::create("res/pic/mainScene/paihang_bar.png");
    paihang_bar1->setPosition(ccp(tower_spr1->getContentSize().width* .505f, tower_spr1->getContentSize().height* .55f));
    tower_spr1->addChild(paihang_bar1);
    
    CCSprite* paihang_bar2 = CCSprite::create("res/pic/mainScene/paihang_bar.png");
    paihang_bar2->setPosition(ccp(tower_spr2->getContentSize().width* .505f, tower_spr2->getContentSize().height* .55f));
    tower_spr2->addChild(paihang_bar2);
    
    tower_spr2->setScale(1.02f);
    CCMenuItem* paihang_Item = CCMenuItemSprite::create(tower_spr1, tower_spr2, this, menu_selector(MainScene::paihangCallBack));
    paihang_Item->setPosition(ccp(_layer_4->getContentSize().width* .60f, _layer_4->getContentSize().height* .78f));
    menu_paihang = CCMenu::create(paihang_Item, NULL);
    menu_paihang->setPosition(CCPointZero);
    _layer_4->addChild(menu_paihang);
    paihang_bar1->setUserObject(ccs("res/pic/mainScene/paihang_bar.png"));
    _arrGroup1->addObject(paihang_bar1);
    
    // Pk动画

    
    CCSprite* pkLight = CCSprite::create("pic/mainScene/main_pk_light.png");
    pkLight->setPosition(paihang_bar1->getPosition() + ccp(paihang_bar1->getContentSize().width * 0.5 - 7, paihang_bar1->getContentSize().height * 0.5 - 5));
    paihang_Item->addChild(pkLight);
    CCSequence* scale = CCSequence::create(CCScaleTo::create(0,5, 1.1), CCScaleTo::create(0.5, 0.9), NULL);
    CCSpawn* spawn = CCSpawn::create(scale, CCRotateBy::create(2, 360), NULL);
    pkLight->runAction(CCRepeatForever::create(spawn));
    
    CCSprite* pkHole = CCSprite::create("pic/mainScene/main_pk_hole.png");
    pkHole->setPosition(pkLight->getPosition());
    paihang_Item->addChild(pkHole);
    pkHole->runAction(CCRepeatForever::create(CCRotateBy::create(4, -360)));
    
    CCSprite* pkText = CCSprite::create("pic/mainScene/main_pk_text.png");
    pkText->setPosition(pkLight->getPosition());
    paihang_Item->addChild(pkText);
    
    
    //-----3层背景------
    _layer_3 = CCSprite::create("res/pic/mainScene/three_bg.png");
    _layer_3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_3);
    
    
    //---日常btn(公司)---
    CCSprite* rc_Spr1 = CCSprite::create("res/pic/mainScene/company.png");
    company_bar1 = CCSprite::create("res/pic/mainScene/company_bar.png");
//    company_bar1->setScale(1.0f/0.55f);
    company_bar1->setPosition(ccp(rc_Spr1->getContentSize().width* .61f, rc_Spr1->getContentSize().height* .20f));
    rc_Spr1->addChild(company_bar1);
//    rc_Spr1->setScale(0.550f);
//    rc_Spr1->setContentSize(rc_Spr1->getContentSize()* .550f);
    
    CCSprite* rc_Spr2 = CCSprite::create("res/pic/mainScene/company.png");
    CCSprite* company_bar2 = CCSprite::create("res/pic/mainScene/company_bar.png");
//    company_bar2->setScale(1.0f/0.55f);
    company_bar2->setPosition(ccp(rc_Spr2->getContentSize().width* .61f, rc_Spr2->getContentSize().height* .20f));
    rc_Spr2->addChild(company_bar2);
    rc_Spr2->setScale(1.02f);
//    rc_Spr2->setContentSize(rc_Spr1->getContentSize()* .550f);
    CCMenuItem* richang_Item = CCMenuItemSprite::create(rc_Spr1, rc_Spr2, this, menu_selector(MainScene::richangCallBack));
    richang_Item->setPosition(ccp(richang_Item->getContentSize().width* .52f, _layer_3->getContentSize().height* .65f));
    menu_richang = CCMenu::create(richang_Item, NULL);
    menu_richang->setPosition(CCPointZero);
    _layer_3->addChild(menu_richang);
    company_bar1->setUserObject(ccs("res/pic/mainScene/company_bar.png"));
    _arrGroup1->addObject(company_bar1);
    
    
    //---活动---
//
//    CCSprite* huodong_bar2 = CCSprite::create("res/pic/mainScene/huodong_bar.png");
//    huodong_bar2->setPosition(ccp(hd_Spr2->getContentSize().width* .45f, hd_Spr1->getContentSize().height* .33f));
//    hd_Spr2->addChild(huodong_bar2);
    
    CCSprite* hd_Spr1 = CCSprite::create("res/pic/mainScene/qiqiu.png");
    CCSprite* hd_Spr2 = CCSprite::create("res/pic/mainScene/qiqiu.png");
    hd_Spr2->setScale(1.02f);
    CCMenuItem* huodong_Item = CCMenuItemSprite::create(hd_Spr1, hd_Spr2, this, menu_selector(MainScene::huodongCallBack));
    huodong_Item->setPosition(ccp(_layer_3->getContentSize().width* .38f, _layer_3->getContentSize().height* .82f));
    menu_huodong = CCMenu::create(huodong_Item, NULL);
    menu_huodong->setPosition(CCPointZero);
    _layer_3->addChild(menu_huodong);
    
    _qiqiu = CCSprite::create("pic/mainScene/huodong.png");
    _qiqiu->setPosition(ccp(hd_Spr1->getContentSize().width * 0.5, hd_Spr1->getContentSize().height * 0.5));
    huodong_Item->addChild(_qiqiu);
    
    huodong_bar1 = CCSprite::create("res/pic/mainScene/huodong_bar.png");
    huodong_bar1->setPosition(ccp(_qiqiu->getContentSize().width* .45f, _qiqiu->getContentSize().height* .33f));
    _qiqiu->addChild(huodong_bar1);
    
    huodong_bar1->setUserObject(ccs("res/pic/mainScene/huodong_bar.png"));
    _arrGroup1->addObject(huodong_bar1);
    
    CCMoveBy* mb = CCMoveBy::create(2.5f, CCPoint(0, 8));
    CCMoveBy* mb2 = CCMoveBy::create(0.5f, CCPoint(0, -1.6));
    CCSequence* seq = CCSequence::create(mb, CCDelayTime::create(0.2f), mb->reverse(), mb2, CCDelayTime::create(0.2f), mb2->reverse(), NULL);
    CCRepeatForever* ac_huodong = CCRepeatForever::create(seq);
    menu_huodong->runAction(ac_huodong);
    
    
    //---樱花树---
    tree = CCSprite::create("res/pic/mainScene/tree.png");
    tree->setPosition(ccp(_layer_3->getContentSize().width* .8f, _layer_3->getContentSize().height* .66f));
    _layer_3->addChild(tree);
    
//    CCAnimation* amt = CCAnimation::create();
//    for (int i = 0; i < 6; i++) {
//        CCString* str = CCString::createWithFormat("res/pic/mainScene/tree_%d.png", i + 1);
//        amt->addSpriteFrameWithFileName(str->getCString());
//    }
//    amt->setDelayPerUnit(1.5f);
//    amt->setRestoreOriginalFrame(true);
//    CCAnimate* animate_tree = CCAnimate::create(amt);
//    
//    CCRepeatForever* rep_tree = CCRepeatForever::create(CCSequence::create(animate_tree, animate_tree->reverse(), NULL));
//    
//    tree->runAction(rep_tree);
    
    
    //---社交btn(咖啡厅)---
    CCSprite* hy_Spr1 = CCSprite::create("res/pic/mainScene/cafeiting.png");
    CCSprite* hy_Spr2 = CCSprite::create("res/pic/mainScene/cafeiting.png");
    hy_Spr2->setScale(1.02f);
    haoyou_Item = CCMenuItemSprite::create(hy_Spr1, hy_Spr2, this, menu_selector(MainScene::haoyouCallBack));
    haoyou_Item->setPosition(ccp(_layer_3->getContentSize().width* .7f, _layer_3->getContentSize().height* .6f));
    menu_haoyou = CCMenu::create(haoyou_Item, NULL);
    menu_haoyou->setPosition(CCPointZero);
    _layer_3->addChild(menu_haoyou);
    
    _cafeiting = CCSprite::create("pic/mainScene/coffee.png");
    _cafeiting->setAnchorPoint(ccp(0.5, 0));
    _cafeiting->setPosition(ccp(hy_Spr1->getContentSize().width * 0.5, -70));
    haoyou_Item->addChild(_cafeiting);
    
    coffee_bar1 = CCSprite::create("res/pic/mainScene/coffee_bar.png");
    coffee_bar1->setUserObject(ccs("res/pic/mainScene/coffee_bar.png"));
    coffee_bar1->setPosition(ccp(hy_Spr1->getContentSize().width* .625f, hy_Spr1->getContentSize().height* .595));
    haoyou_Item->addChild(coffee_bar1);
    _arrGroup1->addObject(coffee_bar1);

    
    CCSprite* coffee_grass = CCSprite::create("res/pic/mainScene/coffee_grass.png");
    coffee_grass->setPosition(ccp(_layer_3->getContentSize().width* .60f, _layer_3->getContentSize().height* .49f));
    _layer_3->addChild(coffee_grass);
    
    CCSprite* san = CCSprite::create("res/pic/mainScene/san.png");
    san->setPosition(ccp(_layer_3->getContentSize().width* .56f, _layer_3->getContentSize().height* .5f));
    _layer_3->addChild(san);
    
    
    //---路灯---
    CCSprite* ludeng_spr = CCSprite::create("res/pic/mainScene/ludeng.png");
    ludeng_spr->setPosition(ccp(_layer_3->getContentSize().width* .08f, _layer_3->getContentSize().height* .512f));
    _layer_3->addChild(ludeng_spr);
    
    //---车---
    int defaultZOrder = 2;

    car_1 = CCSprite::create("res/pic/mainScene/car_1.png");
    car_1->setScale(0.5f);
    car_1->setPosition(ccp(_layer_3->getContentSize().width* .35f, _layer_3->getContentSize().height* .455f));
    _layer_3->addChild(car_1, defaultZOrder);
    
    car_2 = CCSprite::create("res/pic/mainScene/car_2.png");
    car_2->setScale(0.5f);
    car_2->setPosition(ccp(_layer_3->getContentSize().width + 100, _layer_3->getContentSize().height* .455f));
    _layer_3->addChild(car_2, defaultZOrder);
    
    car_3 = CCSprite::create("res/pic/mainScene/car_3.png");
    car_3->setFlipX(true);
    car_3->setScale(0.5f);
    car_3->setPosition(ccp(_layer_3->getContentSize().width* .2f, _layer_3->getContentSize().height* .443f));
    _layer_3->addChild(car_3, defaultZOrder);
    
    
    //-----2层花园背景-----
    _layer_2 = CCSprite::create("res/pic/mainScene/home_bg.png");
    _layer_2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f - 2));
    this->addChild(_layer_2);
    
    
    //---家btn---
    CCSprite* home_spr1 = CCSprite::create("res/pic/mainScene/home_nor.png");
    home_bar1 = CCSprite::create("res/pic/mainScene/home_bar.png");
    home_bar1->setPosition(ccp(home_spr1->getContentSize().width* .54f, home_spr1->getContentSize().height* .405f));
    home_spr1->addChild(home_bar1);
    CCSprite* home_spr2 = CCSprite::create("res/pic/mainScene/home_sel.png");
    CCSprite* home_bar2 = CCSprite::create("res/pic/mainScene/home_bar.png");
    home_bar2->setPosition(ccp(home_spr2->getContentSize().width* .54f, home_spr2->getContentSize().height* .405f));
    home_spr2->addChild(home_bar2);
    home_spr2->setScale(1.02f);
    home_item = CCMenuItemSprite::create(home_spr1, home_spr2, this, menu_selector(MainScene::homeCallBack));
    home_item->setPosition(ccp(_layer_2->getContentSize().width* .265f, _layer_2->getContentSize().height* .344f));
    menu_home = CCMenu::create(home_item, NULL);
    menu_home->setPosition(CCPointZero);
    _layer_2->addChild(menu_home);
    home_bar1->setUserObject(ccs("res/pic/mainScene/home_bar.png"));
    _arrGroup1->addObject(home_bar1);
    
    
    //---邮箱btn---
    CCSprite* mail_box1 = CCSprite::create("res/pic/mainScene/mail_nor.png");
    xinfeng_spr1 = CCSprite::create("res/pic/mainScene/xinfeng.png");
    xinfeng_spr1->setPosition(ccp(mail_box1->getContentSize().width* .6f, mail_box1->getContentSize().height * 0.88));
    mail_box1->addChild(xinfeng_spr1);
    CCSprite* mail_box2 = CCSprite::create("res/pic/mainScene/mail_sel.png");
    CCSprite* xinfeng_spr2 = CCSprite::create("res/pic/mainScene/xinfeng.png");
    xinfeng_spr2->setPosition(ccp(mail_box2->getContentSize().width* .6f, mail_box2->getContentSize().height * 0.88));
    mail_box2->addChild(xinfeng_spr2);
    mail_box2->setScale(1.02f);
    CCMenuItemSprite* youjian_Item = CCMenuItemSprite::create(mail_box1, mail_box2, this, menu_selector(MainScene::youjianCallBack));
    youjian_Item->setPosition(ccp(_layer_2->getContentSize().width* .38f, _layer_2->getContentSize().height* .47f));
    menu_mail = CCMenu::create(youjian_Item, NULL);
    menu_mail->setPosition(CCPointZero);
    _layer_2->addChild(menu_mail);
    
    // 信封
    CCMoveBy* ac_xin_1 = CCMoveBy::create(2.5f, CCPoint(0, 8));
    CCScaleTo* st = CCScaleTo::create(2.5f, 1.05f);
    CCSpawn* spa_xin = CCSpawn::create(ac_xin_1, st, NULL);
    CCScaleTo* st_2 = CCScaleTo::create(2.5f, 1.0f);
    CCMoveBy* ac_xin_2 = CCMoveBy::create(2.5f, CCPoint(0, -8));
    CCSpawn* spa_xin_2 = CCSpawn::create(ac_xin_2, st_2, NULL);
    CCSequence* seq_xin = CCSequence::create(spa_xin, CCDelayTime::create(0.3f), spa_xin_2, CCDelayTime::create(0.3), NULL);
    CCRepeatForever* rep_xin = CCRepeatForever::create(seq_xin);
    xinfeng_spr1->runAction(rep_xin);
    
    
    //---换装btn---
//    CCSprite* hz_Spr1 = CCSprite::create("res/pic/mainScene/shop_nor.png");
//    shop_bar1 = CCSprite::create("res/pic/mainScene/shop_bar.png");
//    shop_bar1->setPosition(ccp(hz_Spr1->getContentSize().width* .29f, hz_Spr1->getContentSize().height* .33f));
//    hz_Spr1->addChild(shop_bar1);
//    CCSprite* hz_Spr2 = CCSprite::create("res/pic/mainScene/shop_sel.png");
//    CCSprite* shop_bar2 = CCSprite::create("res/pic/mainScene/shop_bar.png");
//    shop_bar2->setPosition(ccp(hz_Spr2->getContentSize().width* .29f, hz_Spr2->getContentSize().height* .33f));
//    hz_Spr2->addChild(shop_bar2);
//    hz_Spr2->setScale(1.02f);
//    huanzhuang_Item = CCMenuItemSprite::create(hz_Spr1, hz_Spr2, this, menu_selector(MainScene::huanzhuangCallBack));
//    huanzhuang_Item->setPosition(ccp(_layer_2->getContentSize().width* .68f, _layer_2->getContentSize().height* .345f));
//    huanzhuang_Item->setEnabled(false);
//    menu_hz = CCMenu::create(huanzhuang_Item, NULL);
//    menu_hz->setPosition(CCPointZero);
//    _layer_2->addChild(menu_hz);
//    shop_bar1->setUserObject(ccs("res/pic/mainScene/shop_bar.png"));
//    _arrGroup1->addObject(shop_bar1);
    
    CCSprite* shop = CCSprite::create("res/pic/mainScene/shop.png");
    CCSprite* test = CCSprite::create("res/pic/mainScene/shop_nor.png");
    test->setAnchorPoint(ccp(0, 0));
    test->setPosition(ccp(0, 0));
    shop->addChild(test);
    shop_bar1 = CCSprite::create("res/pic/mainScene/shop_bar.png");
    shop_bar1->setPosition(ccp(test->getContentSize().width* .29f, test->getContentSize().height* .33f));
    shop->addChild(shop_bar1);
    CCSprite* shop2 = CCSprite::create("res/pic/mainScene/shop_sel.png");
    CCSprite* shop_bar2 = CCSprite::create("res/pic/mainScene/shop_bar.png");
    shop_bar2->setPosition(ccp(test->getContentSize().width* .29f, test->getContentSize().height* .33f));
    shop2->addChild(shop_bar2);
    shop2->setScale(1.02f);
    CCMenuItemSprite* shop_item2 = CCMenuItemSprite::create(shop, shop2, this, menu_selector(MainScene::huanzhuangCallBack));
    shop_item2->setAnchorPoint(ccp(0, 0));
    shop_item2->setPosition(ccp(_layer_2->getContentSize().width* .68f - shop2->getContentSize().width / 2, _layer_2->getContentSize().height* .345f - shop2->getContentSize().height / 2));
    CCMenu* menu_shop2 = CCMenu::create(shop_item2, NULL);
    menu_shop2->setPosition(CCPointZero);
    _layer_2->addChild(menu_shop2);

    shop_bar1->setUserObject(ccs("res/pic/mainScene/shop_bar.png"));
    _arrGroup1->addObject(shop_bar1);
    
    //---银行---
    CCSprite* spr_bk = CCSprite::create("res/pic/mainScene/blank_nor.png");
    CCSprite* spr_bk2 = CCSprite::create("res/pic/mainScene/blank_sel.png");
    spr_bk2->setScale(1.02f);
    CCMenuItemSprite* blank_item = CCMenuItemSprite::create(spr_bk, spr_bk2, this, menu_selector(MainScene::blankCallback));
    blank_item->setPosition(ccp(_layer_2->getContentSize().width - spr_bk->getContentSize().width * .37f, _layer_2->getContentSize().height* .30f));
    blank_item->setEnabled(false);
    CCMenu* menu_blank = CCMenu::create(blank_item, NULL);
    menu_blank->setPosition(CCPointZero);
    _layer_2->addChild(menu_blank);
    
    //门前草
    CCSprite* grass = CCSprite::create("res/pic/mainScene/grass.png");
    grass->setPosition(ccp(_layer_2->getContentSize().width* .23f, _layer_2->getContentSize().height* .23f));
//    grass->setVisible(false);
    _layer_2->addChild(grass);
    
    CCSprite* zhalan = CCSprite::create("res/pic/mainScene/zhalan.png");
    zhalan->setPosition(ccp(_layer_2->getContentSize().width* .242f, _layer_2->getContentSize().height* .19f));
    _layer_2->addChild(zhalan);

    //-----1层路------
    _layer_1 = CCSprite::create("res/pic/mainScene/road.png");
    _layer_1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_1);
    
    //---剧情btn---
    CCSprite* jq_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_1_nor.png");
    juqing_bar1 = CCSprite::create("res/pic/mainScene/juqing_bar.png");
    juqing_bar1->setPosition(ccp(jq_Car1->getContentSize().width* .76f, jq_Car1->getContentSize().height* .27f));
    jq_Car1->addChild(juqing_bar1);
    CCSprite* jq_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_1_sel.png");
    CCSprite* juqing_bar2 = CCSprite::create("res/pic/mainScene/juqing_bar.png");
    juqing_bar2->setPosition(ccp(jq_Car2->getContentSize().width* .76f, jq_Car2->getContentSize().height* .27f));
    jq_Car2->addChild(juqing_bar2);
    jq_Car2->setScale(1.02f);
    CCMenuItem* juqing_Item = CCMenuItemSprite::create(jq_Car1, jq_Car2, this, menu_selector(MainScene::juqingCallBack));
    juqing_bar1->setUserObject(ccs("res/pic/mainScene/juqing_bar.png"));
    _arrGroup1->addObject(juqing_bar1);
    
    menu_car1 = CCMenu::create(juqing_Item, NULL);
    menu_car1->setPosition(ccp(_layer_1->getContentSize().width* .65f, _layer_1->getContentSize().height* .13f));
    _layer_1->addChild(menu_car1);
    
    //---vip剧情btn---
    CCSprite* jq2_Car1 = CCSprite::create("res/pic/mainScene/car_juqing_2_nor.png");
    unknow_bar1 = CCSprite::create("res/pic/mainScene/unknow_bar.png");
    unknow_bar1->setPosition(ccp(jq2_Car1->getContentSize().width* .75f, jq2_Car1->getContentSize().height* .23f));
    jq2_Car1->addChild(unknow_bar1);
    CCSprite* jq2_Car2 = CCSprite::create("res/pic/mainScene/car_juqing_2_sel.png");
    CCSprite* unknow_bar2 = CCSprite::create("res/pic/mainScene/unknow_bar.png");
    unknow_bar2->setPosition(ccp(jq2_Car2->getContentSize().width* .75f, jq2_Car2->getContentSize().height* .23f));
    jq2_Car2->addChild(unknow_bar2);
    jq2_Car2->setScale(1.02f);
    CCMenuItem* juqing2_Item = CCMenuItemSprite::create(jq2_Car1, jq2_Car2, this, menu_selector(MainScene::juqing_vipCallBack));
    
    menu_car2 = CCMenu::create(juqing2_Item, NULL);
    menu_car2->setPosition(ccp(_layer_1->getContentSize().width* .35f, _layer_1->getContentSize().height* .15f));
    _layer_1->addChild(menu_car2);

//    unknow_bar1->setUserObject(ccs("res/pic/mainScene/unknow_bar.png"));
//    _arrGroup1->addObject(unknow_bar1);
    if(DATA->getPlayer()->phase > 1) {
        unknow_bar1->setUserObject(ccs("res/pic/mainScene/unknow_bar.png"));
        _arrGroup1->addObject(unknow_bar1);
    }else {
        unknow_bar1->setColor(ccGRAY);
        unknow_bar2->setColor(ccGRAY);
        
        CCSprite* lock1 = CCSprite::create("res/pic/mainScene/lock.png");
        lock1->setPosition(ccp(unknow_bar1->getContentSize().width / 2, unknow_bar1->getContentSize().height / 2));
        unknow_bar1->addChild(lock1);
        
        CCSprite* lock2 = CCSprite::create("res/pic/mainScene/lock.png");
        lock2->setPosition(ccp(unknow_bar2->getContentSize().width / 2, unknow_bar2->getContentSize().height / 2));
        unknow_bar2->addChild(lock2);
    }
    
    
    
    //-----花------
    _layer_0 = CCSprite::create("res/pic/mainScene/near.png");
    _layer_0->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(_layer_0);
    
    
    CCSprite* lingdang = CCSprite::create("res/pic/mainScene/lingdang.png");
    CCSprite* lingdang2 = CCSprite::create("res/pic/mainScene/lingdang.png");
    CCMenuItemSprite* item_lingdang = CCMenuItemSprite::create(lingdang, lingdang2, this, menu_selector(MainScene::lingdang_callback));
    
    CCSprite* lingdang_effect = CCSprite::create("res/pic/mainScene/lingdang_effect.png");
    lingdang_effect->setPosition(ccp(item_lingdang->getContentSize().width* .5f, item_lingdang->getContentSize().height* .5f));
    item_lingdang->addChild(lingdang_effect);
    lingdang_effect->runAction(this->getIntervalAction());
    
    menu = CCMenu::create(_qiandaoItem,
                          huanzhuangItem,
                          btnPurchaseAchievement,
                          _welfareItem,
                          _btnEnergyLargess,
                          _btnGashapon,
                          eventItem,
                          pkItem,
                          monthItem,
                          item_lingdang,
                          NULL);
    menu->alignItemsVerticallyWithPadding(5);
    if (DATA->current_guide_step() == 6){
        menu->setPosition(ccp(0, 90 * 9.5));
    }else{
        menu->setPosition(ccp(0, 0));
    }
    
    
    CCSprite* stencil = CCSprite::create();
    stencil->setTextureRect(CCRect(0, 0, _welfareItem->getContentSize().width, _welfareItem->getContentSize().height* 10 + 40));
    stencil->setColor(ccGRAY);
    node = CCClippingNode::create(stencil);
    node->setColor(ccGRAY);
    node->setPosition(ccp(DISPLAY->ScreenWidth() - _btnEnergyLargess->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .5f));
    node->setInverted(false);
    node->addChild(menu);
    this->addChild(node);
}



void MainScene::monthCallBack(CCObject* pSender){
    MonthCardLayer* layer = MonthCardLayer::create();
    this->addChild(layer, 100);
}



void MainScene::isTxt_Bar(){
    if (DATA->current_guide_step() == 8) {
        if (this->getChildByTag(0x456777) != NULL) {
            CCNode* node = this->getChildByTag(0x456777);
            
            CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.02f);
            CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
            CCScaleTo* scaleTo3 = CCScaleTo::create(.2f, 1.02f);
            CCScaleTo* scaleTo4 = CCScaleTo::create(.1f, 1.f);
            CCSequence* seq = CCSequence::create(scaleTo1, scaleTo2, scaleTo3, scaleTo4, CCDelayTime::create(.6f), NULL);
            node->runAction(CCRepeatForever::create(seq));
        }
    }
}

void MainScene::lingdang_callback(cocos2d::CCObject *pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击铃铛");
    
    if (isOk) {
        CCMoveTo* mt = NULL;
        if (isOpen) {
            mt = CCMoveTo::create(0.3, ccp(0, 90 * 9.5));
            isOpen = false;
        }else{
            if (DATA->current_guide_step() == 6) {
                CCNotificationCenter::sharedNotificationCenter()->postNotification("CloseSwallowEnabled");
            }
            mt = CCMoveTo::create(0.3, ccp(0, 0));
            isOpen = true;
        }
        
        
        menu->runAction(mt);
    }
}

CCActionInterval* MainScene::getIntervalAction(){
    CCFadeIn* fi = CCFadeIn::create(0.8f);
    CCFadeOut* fo = CCFadeOut::create(0.8f);
//    CCRotateBy* rb = CCRotateBy::create(2.0f, 200);
    CCSequence* s = CCSequence::create(fi, CCDelayTime::create(0.2f), fo, CCDelayTime::create(0.2f), NULL);
//    CCSpawn* sp = CCSpawn::create(s, rb, NULL);
    CCRepeatForever* rep = CCRepeatForever::create(s);
    
    return rep;
}

CCArray* MainScene::rand_array(CCArray *arr) {
    if (arr == NULL) {
        return NULL;
    }
    
    int count = arr->count();
    CCArray* arrTemp = CCArray::createWithCapacity(count);
    for (int i = 0; i < count; i++) {
        arrTemp->addObject(arr->objectAtIndex(i));
    }
    
    return arrTemp;
}

void MainScene::play(CCSprite* spt) {
    CCString* name = (CCString*)spt->getUserObject();
    CCSprite* actionSpt = CCSprite::create(name->getCString());
    
    CCScaleTo* st = NULL;
    CCScaleTo* st1 = NULL;
    CCDelayTime* dt = NULL;
    
//    if (name->compare("res/pic/mainScene/company_bar.png") == 0) {
//        actionSpt->setScale(1.0f/0.55f);
//        
//        st = CCScaleTo::create(0.5f, 1.02f * (1.0f/0.55f));
//        st1 = CCScaleTo::create(0.5f, 1.0f * (1.0f/0.55f));
//    
//    }else{
        st = CCScaleTo::create(0.5f, 1.05f);
        dt = CCDelayTime::create(0.3f);
        st1 = CCScaleTo::create(0.5f, 1.0f);

//    }
    
    actionSpt->setPosition(spt->getPosition());
    spt->getParent()->addChild(actionSpt);
    

    
    CCSprite* flower_spr = CCSprite::create("res/pic/mainScene/flower_1.png");
    flower_spr->setPosition(ccp(shop_bar1->getContentSize().width* .5f, shop_bar1->getContentSize().height* .5f));
    actionSpt->addChild(flower_spr);
    
    CCAnimation* animation_flower = CCAnimation::create();
    for (int i = 0; i < 4; i++) {
        CCString* str = CCString::createWithFormat("res/pic/mainScene/flower_%d.png", i + 1);
        animation_flower->addSpriteFrameWithFileName(str->getCString());
    }
    animation_flower->setDelayPerUnit(0.15f);
    animation_flower->setRestoreOriginalFrame(true);
    CCAnimate* animate_flower = CCAnimate::create(animation_flower);
    flower_spr->runAction(CCRepeatForever::create(animate_flower));
    

    actionSpt->runAction(CCSequence::create(st, dt, st1, dt, st, dt, st1, dt, st, dt, st1, dt, st, dt, st1, CCRemoveSelf::create(), NULL));
}

void MainScene::delayPlay(float dt) {
    if (_arrPlay == NULL) {
        unschedule(SEL_SCHEDULE(&MainScene::delayPlay));
        return;
    }
    
    if (_arrPlay->count() > 0) {
        CCSprite* spt = (CCSprite*)_arrPlay->randomObject();
        this->play(spt);
        _arrPlay->removeObject(spt);
    }
    else {
        this->setArrPlay(this->rand_array(_arrGroup1));
    }
}



void MainScene::check_begin_position(CCObject* pos){
    CCPoint last_pos = CCPointFromString(((CCString*)pos)->getCString());
    if (menu_car1->boundingBox().containsPoint(last_pos) ||
        menu_car2->boundingBox().containsPoint(last_pos) ||
        menu_home->boundingBox().containsPoint(last_pos) ||
        menu_hz->boundingBox().containsPoint(last_pos) ||
        menu_mail->boundingBox().containsPoint(last_pos) ||
        menu_richang->boundingBox().containsPoint(last_pos) ||
        menu_haoyou->boundingBox().containsPoint(last_pos) ||
        menu_paihang->boundingBox().containsPoint(last_pos) ||
        menu_huodong->boundingBox().containsPoint(last_pos)) {
        isMenu = true;
    }
}


void MainScene::update(float dt){
    //云
    if (_cloud_1->getPositionX() <= -_cloud_1->getContentSize().width* .5f) {
        _cloud_1->setPositionX(_cloud_2->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_1->setPositionX(_cloud_1->getPositionX() - 3 * dt);
    }
    
    if (_cloud_2->getPositionX() <= -_cloud_2->getContentSize().width* .5f) {
        _cloud_2->setPositionX(_cloud_1->getPositionX() + _cloud_1->getContentSize().width);
    }else{
        _cloud_2->setPositionX(_cloud_2->getPositionX() - 3 * dt);
    }
    
    //河流
    if (_river_1->getPositionX() <= -_river_1->getContentSize().width* .5f) {
        _river_1->setPositionX(_river_2->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_1->setPositionX(_river_1->getPositionX() - 13 * dt);
    }
    
    if (_river_2->getPositionX() <= -_river_2->getContentSize().width* .5f) {
        _river_2->setPositionX(_river_1->getPositionX() + _river_1->getContentSize().width* .5f + _river_2->getContentSize().width* .5f - 5);
    }else{
        _river_2->setPositionX(_river_2->getPositionX() - 13 * dt);
    }
    
    if (_bird_1->getPositionX() <= -100) {
        _bird_1->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_1->setPositionX(_bird_1->getPositionX() - 35 * dt);
    }
    
    if (_bird_2->getPositionX() <= -100) {
        _bird_2->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_2->setPositionX(_bird_2->getPositionX() - 35 * dt);
    }
    
    if (_bird_3->getPositionX() <= -100) {
        _bird_3->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_3->setPositionX(_bird_3->getPositionX() - 35 * dt);
    }
    
    if (_bird_4->getPositionX() <= -100) {
        _bird_4->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_4->setPositionX(_bird_4->getPositionX() - 35 * dt);
    }
    
    if (_bird_5->getPositionX() <= -100) {
        _bird_5->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_5->setPositionX(_bird_5->getPositionX() - 45 * dt);
    }
    
    if (_bird_6->getPositionX() <= -100) {
        _bird_6->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_6->setPositionX(_bird_6->getPositionX() - 55 * dt);
    }
    
    if (_bird_7->getPositionX() <= -100) {
        _bird_7->setPositionX(_layer_6->getContentSize().width + 100);
    }else{
        _bird_7->setPositionX(_bird_7->getPositionX() - 65 * dt);
    }
    
    if (car_1->getPositionX() <= -200) {
        car_1->setPositionX(_layer_3->getContentSize().width + 100);
    }else{
        car_1->setPositionX(car_1->getPositionX() - 70 * dt);
    }
    
    if (car_2->getPositionX() <= -200) {
        car_2->setPositionX(_layer_3->getContentSize().width + 100);
    }else{
        car_2->setPositionX(car_2->getPositionX() - 70 * dt);
    }
    
    if (car_3->getPositionX() >= 100 + _layer_3->getContentSize().width) {
        car_3->setPositionX(-_layer_3->getContentSize().width - 100);
    }else{
        car_3->setPositionX(car_3->getPositionX() + 70 * dt);
    }
    
//    if(opacity <= 150) {
//        sp = 20.0;
//    }
//    if(opacity >= 255) {
//        sp = -20.0;
//    }
//    opacity += sp * dt;
//    opacity = opacity > 255 ? 255 : opacity;
//    tree->setOpacity((GLubyte)opacity);
    
    if (isEffective) {
        isOk = true;
    }
}

void MainScene::change_position(CCObject* pObj){
    offset_x = ((CCFloat*)pObj)->getValue();
    
    if (isMenu) {
        if (isMoving) {
            this->updateLayerPosition(offset_x);
        }else{
            move_x += offset_x;
            if (fabsf(move_x) >= 30) {
                this->updateLayerPosition(offset_x);
                isMoving = true;
            }
        }
        
    }else{
        this->updateLayerPosition(offset_x);
    }
}

void MainScene::updateLayerPosition(float offset_x){
    isEffective= false;
    isOk = false;
    
    if ((_layer_0->getPositionX() + offset_x >= _layer_0->getContentSize().width* .5f - 5 && offset_x > 0) ||
        (_layer_0->getPositionX() + offset_x <= DISPLAY->ScreenWidth() - _layer_0->getContentSize().width* .5f + 5 && offset_x < 0)) {
        
    }else{
        _layer_0->setPositionX(_layer_0->getPositionX() + offset_x);
        _layer_1->setPositionX(_layer_1->getPositionX() + offset_x);
        _layer_2->setPositionX(_layer_2->getPositionX() + offset_x* .8f);
        _layer_3->setPositionX(_layer_3->getPositionX() + offset_x* .6f);
        _layer_4->setPositionX(_layer_4->getPositionX() + offset_x* .35f);
        _river_1->setPositionX(_river_1->getPositionX() + offset_x* .3f);
        _river_2->setPositionX(_river_2->getPositionX() + offset_x* .3f);
        _layer_6->setPositionX(_layer_6->getPositionX() + offset_x* .15f);
    }
}

void MainScene::setIsEffective(){
    isEffective = true;
    isMenu = false;
    isMoving = false;
    move_x = 0;
}

void MainScene::juqing_vipCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击vip剧情");
    
    if (isOk) {
//        PromptLayer* layer = PromptLayer::create();
//        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待");
//        WS->connect();
        
        
        if(DATA->getPlayer()->phase >1) {
            if (DATA->getStory()->has_init_story2()) {
                this->_504CallBack(NULL);
            }else{
                LOADING->show_loading();
                NET->completed_story2_504();
            }
        }else {
            PromptLayer* layer = PromptLayer::create();
            layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "二级公司开启");
        }
    }
}
void MainScene::_504CallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    CCScene* scene = VipQingjingScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::homeCallBack(CCObject *pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击home");
    
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            ishomeBool = true;
            LOADING->show_loading();
            NET->completed_mission_600();
        }else {
            AUDIO->comfirm_effect();
            LOADING->show_loading();
            NET->home_info_704(! DATA->getHome()->has_init_house_template());
        }
    }
}
void MainScene::_704CallBack(CCObject* pSender){
    LOADING->remove();
    CCScene* scene = HomeLayer::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

//void MainScene::shouchongCallBack(CCObject* pSender){
//    if (isOk) {
//        PromptLayer* layer = PromptLayer::create();
//        layer->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待");
//    }
//}

void MainScene::huodongCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击活动");
    
    if (isOk) {
        AUDIO->comfirm_effect();
        OperationPanel::show();
        
        if (_qiqiu) {
            _qiqiu->runAction(CCSequence::create(CCScaleTo::create(0.15, 1.03), CCScaleTo::create(0.15, 1.f), NULL));
        }
    }
}

void MainScene::welfareCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击福利");
    
    if (isOk) {
        LOADING->show_loading();
        AUDIO->comfirm_effect();
        NET->welfare_info_630();
        //
        CCMenuItem* btn = (CCMenuItem* )pSender;
        if (btn) {
            btn->setEnabled(false);
            CCCallFuncN* afterCalled = CCCallFuncN::create(this, SEL_CallFuncN(&MainScene::afterMenuItemCalled));
            btn->runAction(CCSequence::create(CCDelayTime::create(1.f), afterCalled, NULL));
        }
    }
}

void MainScene::nc_welfare_user_info(CCObject *pObj) {
    LOADING->remove();
    WelfarePanel::show();
    NET->achievement_info_640(true);
}

void MainScene::qiandaoCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击签到");
    
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
// 1.8.1前的版本
//        if (DATA->getSignin()->has_init_signin7_template()) {
//            NET->signin7_info_302(false);
//        }
//        else {
//            NET->signin7_info_302(true);
//        }
        // 1.8.1后的版本
        if (DATA->getSignin()->has_init_signin7_template()) {
            NET->signin7_info_312(false);
        }
        else {
            NET->signin7_info_312(true);
        }
        
        CCMenuItem* btn = (CCMenuItem* )pSender;
        if (btn) {
            btn->setEnabled(false);
            CCCallFuncN* afterCalled = CCCallFuncN::create(this, SEL_CallFuncN(&MainScene::afterMenuItemCalled));
            btn->runAction(CCSequence::create(CCDelayTime::create(1.f), afterCalled, NULL));
        }
    }
}

void MainScene::youjianCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击邮件");
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        NET->all_mails_700();
    }
}

void MainScene::haoyouCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击好友");
    
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        NET->social_info_800();
        if (_cafeiting) {
            _cafeiting->runAction(CCSequence::create(CCScaleTo::create(0.15, 1.03), CCScaleTo::create(0.15, 1.f), NULL));
        }
    }
}

void MainScene::energyLargessCallBack(CCObject *pSender) {
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击体力福利");
    
    if (isOk) {
        EnergyLargessPanel::show(this->getScene());
        //
        CCMenuItem* btn = (CCMenuItem* )pSender;
        if (btn) {
            btn->setEnabled(false);
            CCCallFuncN* afterCalled = CCCallFuncN::create(this, SEL_CallFuncN(&MainScene::afterMenuItemCalled));
            btn->runAction(CCSequence::create(CCDelayTime::create(1.f), afterCalled, NULL));
        }
    }
}

void MainScene::purchaseAchievementCallBack(CCObject *pSender) {
//    // talkingData
//    DATA->onEvent("点击事件", "主界面", "点击充值成就");  改动为必然弹出该面板了
    
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        if (DATA->getOperation()->has_init_purchase_achievement_template()) {
            NET->purchase_achievement_info_304(false);
        }
        else {
            NET->purchase_achievement_info_304(true);
        }
    }
}

void MainScene::gashaponCallBack(CCObject *pSender) {
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击扭蛋");
    
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        if (DATA->getOperation()->has_init_gashapon_template()) {
            NET->gashapon_info_306(false);
        }
        else {
            NET->gashapon_info_306(true);
        }
    }
//    Shower* shower = Shower::create();
//    shower->ondress((CCDictionary*)suits->objectAtIndex(0));
//    CCDirector::sharedDirector()->getRunningScene()->addChild(shower);
}

void MainScene::onEventCallback(CCObject *pSender) {
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        if (DATA->getMystery()->hasInitAchvTemplate()) {
            NET->fetch_mystery_info_610(false);
        }
        else {
            NET->fetch_mystery_info_610(true);
        }
    }
}

void MainScene::onPkCalled(CCObject *pSender) {
    if (isOk) {
        AUDIO->comfirm_effect();
        LOADING->show_loading();
        _isEnterPk = true;
        NET->competition_info_820(! DATA->getCompetition()->hasInitRankInfo());
    }
}

void MainScene::afterMenuItemCalled(CCMenuItem* btn) {
    CCLOG("MainScene::afterMenuItemCalled()");
    btn->setEnabled(true);
}

//void MainScene::mysteryUnlockPrompt(CCObject* pSender) {
//    PromptLayer* prompt = PromptLayer::create();
//    prompt->show_prompt(this->getScene(), "完成公司日常“培训会”解锁");
//}

//void MainScene::openChat(cocos2d::CCObject *pSender){
//    // talkingData
//    DATA->onEvent("点击事件", "主界面", "点击聊天");
//    
//    AUDIO->comfirm_effect();
//    DATA->setChatOut(false);
//    if (WS->isConnected()) {
//        ChatPanel* panel = ChatPanel::create();
//        this->addChild(panel, 100);
//    }else{
//        WS->connect();
//    }
//}
//
//void MainScene::displayChatItem(){
//    if (item_chat->isVisible()) {
//        item_chat->setVisible(false);
//    }else{
//        item_chat->setVisible(true);
//    }
//}

void MainScene::social_info_callback_800(CCObject* pObj) {
    LOADING->remove();
    CCScene* scene = HaoyouScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::renwuCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
}

void MainScene::huanzhuangCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击换装");
    
    if (isOk) {
        if (DATA->getClothes()->has_init_clothes == true) {
            this->_huanzhuangCallBack(pSender);
        }
        else {
            LOADING->show_loading();
            NET->owned_clothes_400();
        }
    }
}

void MainScene::_huanzhuangCallBack(CCObject* pSender){
    AUDIO->comfirm_effect();
    
    DATA->setClothesBool(true);
    
    CCLayer* layer = ClothesScene::create_with_type(2, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::blankCallback() {
    CCLOG("go to blank");
}

void MainScene::paihangCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击排行");
    if (isOk) {
        _isEnterPk = false;
        LOADING->show_loading();
        NET->competition_info_820(! DATA->getCompetition()->hasInitRankInfo());
    }
}

//void MainScene::rankList_callback_300(CCObject *pObj){
//    NET->all_friends_806();
//}

void MainScene::competition_callback_820(CCObject *pObj) {
    if (_isEnterPk) {
        DATA->setPkBool(false);
        CCScene* scene = pkScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
    else {
        if (DATA->getSocial()->getHasInitFriends()) {
            this->all_friends_callback_806(NULL);
        }
        else {
            NET->all_friends_806();
        }
    }
}

void MainScene::nc_recharge_info_304(CCObject *pObj){
    LOADING->remove();
    TotalRechargePanel::show();
}

void MainScene::nc_signin_info_312(CCObject *pObj) {
    LOADING->remove();
//    Signin7Panel::show(this->getScene());
    NewSignin7Panel::show(this->getScene());
}

void MainScene::nc_gashapon_info_306(CCObject *pObj) {
    LOADING->remove();
    
    CCLayer* layer = GashaponScene::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
}

void MainScene::all_friends_callback_806(CCObject *pObj){
    LOADING->remove();
//    CCLayer* layer = TotalRankScene::create_with_type(1);
//    CCScene* scene = CCScene::create();
//    scene->addChild(layer);
//    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
//    CCDirector::sharedDirector()->replaceScene(trans);

    RankListScene* layer = RankListScene::create();
    DATA->setComeFrom("main");
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void MainScene::juqingCallBack(CCObject* pSender){
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击剧情");
    
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            this->_500CallBack(NULL);
        }else{
            LOADING->show_loading();
            NET->completed_story_500();
        }
    }
//    CCScene* pScene = CCScene::create();
//    StoryScene* layer = StoryScene::create_with_story_id(0);
//    pScene->addChild(layer);
//    CCTransitionScene* trans = CCTransitionFade::create(.3f, pScene);
//    CCDirector::sharedDirector()->replaceScene(trans);
    
}

void MainScene::_500CallBack(CCObject* pSender) {
    if (isrenwuBool) {
        LOADING->show_loading();
        NET->completed_mission_600();
    }else {
        AUDIO->comfirm_effect();
        CCScene* scene = QingjingScene::scene();
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void MainScene::richangCallBack(CCObject* pSender) {
    // talkingData
    DATA->onEvent("点击事件", "主界面", "点击日常");
    
    if (isOk) {
        if (DATA->getStory()->has_init_story()) {
            LOADING->show_loading();
            NET->completed_mission_600();
        }else {
            isrenwuBool = true;
            LOADING->show_loading();
            NET->completed_story_500();
        }
    }
}
void MainScene::richangMethods() {
    PlayerComp* _player = DATA->getPlayer();
    if (_player->getGuide() == 1) {
        _player->setGuide(2);
        // talkingData
        DATA->onEvent("引导事件", "引导界面", "完成引导第1步");
    }else if (_player->getGuide() == 8){
        _player->setGuide(8);
    }else if (_player->getGuide() == 100){
        _player->setGuide(5);
    }
    
    LOADING->show_loading();
    NET->update_guide_905(_player->getGuide());
}
void MainScene::_905CallBack(CCObject *pObj){
    
    if (DATA->getStory()->has_init_story()) {
        NET->completed_mission_600();
    }else {
        isrenwuBool = true;
        NET->completed_story_500();
    }
}

void MainScene::_600CallBack(CCObject* pSender){
    LOADING->remove();
    if (ishomeBool) {
        LOADING->show_loading();
        NET->home_info_704(! DATA->getHome()->has_init_house_template());
    }else{
        AUDIO->comfirm_effect();
        DATA->setTaskPhase(DATA->getPlayer()->phase);
        CCLayer* layer = TaskScene::create(false);
        CCScene* scene = CCScene::create();
        scene->addChild(layer);
        CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
        CCDirector::sharedDirector()->replaceScene(trans);
    }
}

void MainScene::creat_Man(){
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}

void MainScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .65f;
    float heightFloat = .5f;
    float scaleFloat = 1.f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_Rank_TouFa; i <= Tag_Rank_ZhuangRong; i++) {
        if (i == Tag_Rank_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_Rank_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_Rank_TouFa2);
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
                            _tfSpr1->setTag(Tag_Rank_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_Rank_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_Rank_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_Rank_WaiTao1);
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
                            _wtSpr1->setTag(Tag_Rank_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_Rank_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_Rank_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _sySpr1->setTag(Tag_Rank_ShangYi1);
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
                            _sySpr1->setTag(Tag_Rank_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_Rank_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_Rank_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Rank_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                    _kzSpr1->setTag(Tag_Rank_KuZi1);
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
                            _kzSpr1->setTag(Tag_Rank_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_Rank_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_Rank_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_Rank_WaZi1);
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
                            _wzSpr1->setTag(Tag_Rank_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_Rank_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_Rank_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_Rank_XieZi1);
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
                            _xzSpr1->setTag(Tag_Rank_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_Rank_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_Rank_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_Rank_ShiPin){
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
        else if (i == Tag_Rank_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_Rank_Bao1);
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
                            _bSpr1->setTag(Tag_Rank_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_Rank_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_Rank_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
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
                            _zrSpr1->setTag(Tag_QJ_ZhuangRong1);
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
        
void MainScene::all_mail_callback_700(cocos2d::CCObject *pObj) {
    xinfeng_spr1->removeAllChildrenWithCleanup(true);
    
    LOADING->remove();
    MailPanel* panel = MailPanel::create();
    panel->show_from(ccp(DISPLAY->ScreenWidth()* .93f, DISPLAY->ScreenHeight()* .85f));
}

void MainScene::nc_fetch_mystery_info_610(CCObject *pObj) {
    LOADING->remove();
//    CCDirector::sharedDirector()->replaceScene(MysteryLayer::scene());
    MysteryLayer::show(this->getScene());
}

void MainScene::showTrystEntrance() {
    CCSprite* spr1 = CCSprite::create("pic/tryst/tryst_entrance.png");
    CCSprite* spr2 = CCSprite::create("pic/tryst/tryst_entrance.png");
    if (spr1 == NULL) {
        return;
    }
    
    spr2->setScale(1.012);
    CCMenuItem* itemEntrance = CCMenuItemSprite::create(spr1, spr2, this, SEL_MenuHandler(&MainScene::onBtnStartTryst));
    CCMenu* menuEntrance = CCMenu::createWithItem(itemEntrance);
    menuEntrance->setPosition(_layer_3->getContentSize().width * 0.5 - 13, DISPLAY->halfH() - 12);
    _layer_3->addChild(menuEntrance);
    // 时间文字
    CCLabelTTF* timePeriod = CCLabelTTF::create("约会仅在周2.4.6.日开启", DISPLAY->fangzhengFont(), 13.f);
    timePeriod->setPosition(menuEntrance->getPosition() + ccp(0, -38));
    _layer_3->addChild(timePeriod);
}

void MainScene::onBtnStartTryst() {
    DATA->onEvent("点击事件", "主界面", "点击 - 约会");
    if (isOk) {
        AHMessageBox* mb = AHMessageBox::create_with_message("是否开始约会？", this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO, false);
        mb->setPosition(ccp(DISPLAY->ScreenWidth() * .5f, DISPLAY->ScreenHeight() * .5f));
        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
        // 体力消耗显示
        CCString* strCost = CCString::createWithFormat("每次消耗 %d", DATA->getPlayer()->trystEnergyCost);
        CCLabelTTF* engLabel = CCLabelTTF::create(strCost->getCString(), DISPLAY->fangzhengFont(), 20);
        engLabel->setPosition(ccp(DISPLAY->halfW(), mb->getContentSize().height * 0.48f));
        engLabel->setColor(ccWHITE);
        mb->addChild(engLabel, 3);
        CCSprite* xinSpr = CCSprite::create("res/pic/clothesScene/gj_xin.png");
        xinSpr->setScale(.85f);
        xinSpr->setPosition(ccp(DISPLAY->halfW() + engLabel->getContentSize().width * 0.5f + 16, mb->getContentSize().height * 0.48f));
        mb->addChild(xinSpr, 2);
    }
}

void MainScene::after_fetch_tryst_info_620() {
    this->scheduleOnce(SEL_SCHEDULE(&MainScene::check_tryst_progress), 1);
}

void MainScene::after_start_tryst_621() {
    LOADING->remove();
    TrystUserdata* trystUserData = DATA->getTryst()->getUserData();
    CCScene* scene = TrystScene::create(trystUserData->curTrystId.c_str());
    CCDirector::sharedDirector()->replaceScene(scene);
}

void MainScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag) {
    box->animation_out();
    if (button_type == AH_BUTTON_TYPE_YESNO) {
        if (button_tag == AH_BUTTON_TAG_YES) {
            LOADING->show_loading();
            NET->take_tryst_task_621();
        }
    }
}

void MainScene::check_tryst_progress() {
    TrystUserdata* trystUserData = DATA->getTryst()->getUserData();
    TrystStatus state = trystUserData->status;
    if (state == TrystStatus::going || state == TrystStatus::reward) {
        TrystProgress* progress = TrystProgress::create(trystUserData->timeSpan, trystUserData->leftTime);
        CCSize size = progress->sizeOfContent();
        progress->setPosition(ccp(DISPLAY->halfW(), size.height * 0.5 + 2));
        this->addChild(progress);
    }
}

void MainScene::update_news_status() {
    NewsComp* news = DATA->getNews();
    if (news->mail > 0) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setPosition(ccp(26, 54));
        spt->setTag(171);
        xinfeng_spr1->addChild(spt);
    }
    else {
        CCNode* hongDian = xinfeng_spr1->getChildByTag(171);
        if (NULL != hongDian) {
            hongDian->removeFromParent();
        }
    }
    // 玩家消息
    if (news->paper + news->message > 0) {
        CCSprite* spt2 = CCSprite::create("res/pic/new.png");
        spt2->setPosition(ccp(164, 140));
        spt2->setTag(173);
        haoyou_Item->addChild(spt2);
    }
    else {
        CCNode* hongDian = haoyou_Item->getChildByTag(173);
        if (NULL != hongDian) {
            hongDian->removeFromParent();
        }
    }
    // 体力奖励
    if (news->energy1 == 1 || news->energy2 == 1) {
        CCNode* hongDian = _btnEnergyLargess->getChildByTag(174);
        if (! hongDian) {
            CCSprite* spt = CCSprite::create("res/pic/new.png");
            spt->setPosition(ccp(24, 74));
            spt->setTag(174);
            _btnEnergyLargess->addChild(spt);
        }
    }
    else {
        CCNode* hongDian = _btnEnergyLargess->getChildByTag(174);
        if (NULL != hongDian) {
            hongDian->removeFromParent();
        }
    }
    // 签到
    if (news->signin7 == 1) { // 可签
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setPosition(ccp(20, 74));
        spt->setTag(175);
        _qiandaoItem->addChild(spt);
    }
    else {
        CCNode* hongDian = _qiandaoItem->getChildByTag(175);
        if (NULL != hongDian) {
            hongDian->removeFromParent();
        }
    }

//    // 国庆临时签到
//    if (DATA->current_guide_step() == 0) {
//        float scale = 1;
//        CCSprite* pic1 = CCSprite::create("pic/mainScene/btn_guoqing.png");
//        pic1->setScale(scale);
//        CCSprite* pic2 = CCSprite::create("pic/mainScene/btn_guoqing.png");
//        pic2->setScale(DISPLAY->btn_scale() * scale);
//        CCMenuItem* item = CCMenuItemSprite::create(pic1, pic2, this, SEL_MenuHandler(&MainScene::show_guoqing_signin));
//        CCMenu* menuGuoqing = CCMenu::createWithItem(item);
//        item->setPosition(ccp(DISPLAY->halfW() - item->getContentSize().width * 1.6, DISPLAY->H() * 0.375));
//        this->addChild(menuGuoqing, 100);
//        
//        if (DATA->getNews()->tempSignin == 1) { // 可签
//
//        }
//        else {
//
//        }
//    }
}

void MainScene::check_dailysignin() {
    this->scheduleOnce(SEL_SCHEDULE(&MainScene::update_dailysignin), 0.1);
}

void MainScene::update_dailysignin() {
    if (DATA->getNews()->dailySignin == 1) { // 可签
        DailySigninRewardPanel::show(this->getScene());
    }
}

void MainScene::show_guoqing_signin() {
    NET->temp_signin_info_340();
}

void MainScene::nc_temp_signin_info_340(CCObject *pObj) {
    TempSignin::show(this->getScene());
}

void MainScene::check_free_gashapon() {
    unsigned long timeLeft = DATA->getOperation()->getFreePoint();
    if (timeLeft == 0) {
        CCSprite* spt = CCSprite::create("res/pic/new.png");
        spt->setPosition(ccp(20, 74));
        spt->setTag(182);
        _btnGashapon->addChild(spt);
    }
    else {
        CCNode* hongDian = _btnGashapon->getChildByTag(182);
        if (NULL != hongDian) {
            hongDian->removeFromParent();
        }
    }
}

//void MainScene::check_first_on() {
//    if (DATA->getFirstOnMainScene() == true) {
//        CCSprite* spt = CCSprite::create("res/pic/new.png");
//        spt->setPosition(ccp(20, 74));
//        spt->setTag(183);
//        _welfareItem->addChild(spt);
//    }
//    else {
//        CCNode* hongDian = _welfareItem->getChildByTag(183);
//        if (NULL != hongDian) {
//            hongDian->removeFromParent();
//        }
//    }
//    
//    DATA->setFirstOnMainScene(false);
//}

void MainScene::check_welfare_new() {
    //    if (news->welfare == 1) {
    int newCount = DATA->getWelfare()->getNewCount() + DATA->getAchievement()->getNewCount();
    CCNode* oldHongDian = _welfareItem->getChildByTag(185);
    if (newCount > 0) {
        if (! oldHongDian) {
            CCSprite* spt = CCSprite::create("res/pic/new.png");
            spt->setPosition(ccp(20, 74));
            spt->setTag(185);
            _welfareItem->addChild(spt);
        }
    }
    else {
        if (oldHongDian) {
            oldHongDian->removeFromParent();
        }
    }
}

void MainScene::linshiMethod(CCObject *pObj){
    RewardLayer* layer = RewardLayer::create_with_index((CCArray* )pObj);
    this->addChild(layer, 100);
}

void MainScene::creat_guideBool(){
    PlayerComp* _player = DATA->getPlayer();
    if (_player->getGuide() == 1) {
        for (int i = 0; i < 4; i++) {
            DATA->_guideBool1[i] = false;
        }
    }
    if (_player->getGuide() > 1 && _player->getGuide() <= 4){
        _player->setGuide(1);
        DATA->_guideBool1[0] = true;
        DATA->_guideBool1[1] = true;
        DATA->_guideBool1[2] = false;
        DATA->_guideBool1[3] = false;
        for (int i = 0; i < 6; i++) {
            DATA->_guideBool2[i] = false;
        }
        for (int i = 0; i < 3; i++) {
            DATA->_guideBool3[i] = false;
        }
        for (int i = 0; i < 8; i++) {
            DATA->_guideBool4[i] = false;
        }
    }
    if (_player->getGuide() == 5) {
        for (int i = 0; i < 5; i++) {
            DATA->_guideBool5[i] = false;
        }
    }
    if (_player->getGuide() == 6) {
        for (int i = 0; i < 10; i++) {
            DATA->_guideBool6[i] = false;
        }
    }
    if (_player->getGuide() == 7) {
        for (int i = 0; i < 10; i++) {
            DATA->_guideBool7[i] = false;
        }
    }
    if (_player->getGuide() == 8) {
        for (int i = 0; i < 10; i++) {
            DATA->_guideBool8[i] = false;
        }
    }
    if (_player->getGuide() == 9) {
        for (int i = 0; i < 10; i++) {
            DATA->_guideBool9[i] = false;
        }
    }
    
}


void MainScene::xianshiCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->flash_sale_today_160();
}
void MainScene::_160CallBack(CCObject* pSender){
    LOADING->remove();
    
    CCLayer* layer = SalesPromotionLayer::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(scene);
}






