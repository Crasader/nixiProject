//
//  PurchasePanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "PurchasePanel.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"
#include "PromptLayer.h"
#include "AudioManager.h"
//#include "Reward.h"
#include "Loading2.h"
#include "ConfigManager.h"
#include "JNIController.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSIAPManager.h"
#endif

#define CELL_WIDTH          542
#define CELL_HEIGHT         228


PurchasePanel::~PurchasePanel() {
}

bool PurchasePanel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _content = CCLayer::create();
//        _content->setScale(0.1);
//        _content->setVisible(false);
        this->addChild(_content);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void PurchasePanel::onEnter() {
    CCLayer::onEnter();
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::nc_verify_iOS_107), "HTTP_FINISHED_107", NULL);
    nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::nc_verify_iOS_107), "HTTP_FINISHED_133", NULL);
    nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::update_content), "UPDATE_MAIL_PURCHASE", NULL);
    
    
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    if (CONFIG->baiOrYijie == 0) {// 白包
        nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::nc_verify_iOS_107), "HTTP_FINISHED_107", NULL);
    }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2){// 易接
        nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::nc_verify_android_105), "HTTP_FINISHED_105", NULL);
    }
#endif
    
    this->init_content();

    this->scheduleOnce(SEL_SCHEDULE(&PurchasePanel::keyBackStatus), .8f);
}
void PurchasePanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void PurchasePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

bool PurchasePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
//        this->do_exit();
        remove();
    }
    
    return true;
}

#pragma mark - export

void PurchasePanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}


#pragma mark - inner

void PurchasePanel::init_content() {
    _panel = CCSprite::create("res/pic/panel/iap/iap_plane2.png");
    _panel->setPosition(DISPLAY->center());
    _content->addChild(_panel);
    
    CCSize panelsize = _panel->boundingBox().size;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCLabelTTF* lblTiShi = CCLabelTTF::create("购买的钻石会通过邮件发放!", DISPLAY->fangzhengFont(), 20);
    lblTiShi->setColor(DISPLAY->defalutColor());
    lblTiShi->setPosition(ccp(panelsize.width * 0.5, panelsize.height * 0.93));
    _panel->addChild(lblTiShi);
#endif
    
    CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
    txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
    _content->addChild(txt_close);
    
//    CCSprite* prompt = CCSprite::create("res/pic/panel/iap/iap_prompt.png");
//    prompt->setPosition(ccp(panelsize.width * 0.5, panelsize.height * 0.84));
//    _panel->addChild(prompt);
    this->update_content();
}

void PurchasePanel::update_content() {
    if (_menu) {
        _menu->removeFromParent();
    }
    
    CCSize panelsize = _panel->boundingBox().size;
    float padding = 9;
    
    PurchaseComp* purchase = DATA->getPurchase();
    CCArray* products = purchase->products();
    int count = products->count();
    const char* png_format = "pic/panel/iap/iap_bar2_%d.png";
    const char* png_format2 = "pic/panel/iap/iap_bar2_%dx2.png";
    CCArray* arr = CCArray::createWithCapacity(count);
    for (int i = 0; i < 5; ++i) {
        CCObject* pObj = products->objectAtIndex(i);
        ProductItem* pro = (ProductItem* )pObj;
        CCSprite* pic1 = NULL;
        CCSprite* pic2 = NULL;
        if (purchase->has_purchased(pro->id.c_str())) {
            CCString* file = CCString::createWithFormat(png_format, i);
            pic1 = CCSprite::create(file->getCString());
            pic2 = CCSprite::create(file->getCString());
        }
        else {
            CCString* file = CCString::createWithFormat(png_format2, i);
            pic1 = CCSprite::create(file->getCString());
            pic2 = CCSprite::create(file->getCString());
        }
        pic2->setScale(DISPLAY->btn_scale());
        
        CCPoint dimaPos = ccp(492 * 0.13, 109 * 0.5);
        if (0 < i && i < 3) {
            CCSprite* diam1 = CCSprite::create("res/pic/panel/iap/diam_bit.png");
            diam1->setPosition(dimaPos);
            diam1->setRotation(-22);
            pic1 ->addChild(diam1);
            
            CCSprite* diam2 = CCSprite::create("res/pic/panel/iap/diam_bit.png");
            diam2->setPosition(dimaPos);
            diam2->setRotation(-22);
            pic2 ->addChild(diam2);
        }
        else if (3 <= i && i < 5) {
            CCSprite* diam1 = CCSprite::create("res/pic/panel/iap/diam_large.png");
            diam1->setPosition(dimaPos);
            diam1->setRotation(-14);
            
            pic1 ->addChild(diam1);
            CCSprite* diam2 = CCSprite::create("res/pic/panel/iap/diam_large.png");
            diam2->setPosition(dimaPos);
            diam2->setRotation(-14);
            pic2 ->addChild(diam2);
        }
        else if (5 <= i) {
            CCSprite* diam1 = CCSprite::create("res/pic/panel/iap/diam_huge.png");
            diam1->setPosition(dimaPos);
            diam1->setRotation(-10);
            pic1 ->addChild(diam1);
            
            CCSprite* diam2 = CCSprite::create("res/pic/panel/iap/diam_huge.png");
            diam2->setPosition(dimaPos);
            diam2->setRotation(-10);
            pic2 ->addChild(diam2);
        }
        
        CCMenuItemSprite* btn = CCMenuItemSprite::create(pic1, pic2, this, SEL_MenuHandler(&PurchasePanel::on_bar_clicked));
        btn->setUserObject(pObj);
        btn->setTag(i + 1);
        arr->addObject(btn);
    }
    
    _menu = CCMenu::createWithArray(arr);
    _menu->alignItemsVerticallyWithPadding(padding);
    _menu->setPosition(_menu->getPosition() - ccp(8, 18));
    _content->addChild(_menu);
}

void PurchasePanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.8f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
}

void PurchasePanel::do_exit() {
    float duration = 0.6f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&PurchasePanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void PurchasePanel::remove() {
    AUDIO->goback_effect();
    num_child = 0;
    this->removeFromParentAndCleanup(true);
}

void PurchasePanel::on_bar_clicked(CCMenuItem *item) {
    ProductItem* pro = (ProductItem* )item->getUserObject();
    CCLOG("clicked %s", pro->id.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (true) {
        LOADING->show_loading();
        IOSIAPManager* d = IOSIAPManager::Inst();
        if (d->canMakePurchases()) {
            CCLOG("can purchases");
            d->buyProduct(pro->id.c_str());

            // TalkingData Record
            string orderId2 = DATA->getLogin()->obtain_UUID();
            string productId = pro->id.c_str();
            CCString* iapId = CCString::createWithFormat("%d钻石", pro->diam);
            DATA->onChargeRequest(orderId2, iapId->getCString(), pro->money, pro->diam);
        }
        else {
            LOADING->remove();
            CCLOG("can not purchases");
        }
    }
    else {
        LOADING->show_loading();
        string orderId = "";
        
        string orderId2 = DATA->getLogin()->obtain_UUID();
        string productId = pro->id.c_str();
        CCString* iapId = CCString::createWithFormat("%d钻石", pro->diam);
        DATA->onChargeRequest(orderId2, iapId->getCString(), pro->money, pro->diam);
        
        NET->verify_order_iOS_107(orderId, pro->id);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CONFIG->baiOrYijie == 0) {// 白包
        if (CONFIG->openPay == 0) {
            // talkingData
            DATA->onEvent("支付意向", "支付界面", "点击购买钻石");
        }else if (CONFIG->openPay == 1){
            LOADING->show_loading();
            string orderId = "";
            
            string orderId2 = DATA->getLogin()->obtain_UUID();
            string productId = pro->id.c_str();
            CCString* iapId = CCString::createWithFormat("%d钻石", pro->diam);
            DATA->onChargeRequest(orderId2, iapId->getCString(), pro->money, pro->diam);
            
            NET->verify_order_iOS_107(orderId, pro->id);
        }
    }else if (CONFIG->baiOrYijie == 1 || CONFIG->baiOrYijie == 2 || CONFIG->baiOrYijie == 3){// 易接
        if (CONFIG->openPay == 0) {
            // talkingData
            DATA->onEvent("支付意向", "支付界面", "点击购买钻石");
        }else if (CONFIG->openPay == 1){
            LOADING->show_loading();
            
            JNIController::setChannelId(CONFIG->channelId);
            JNIController::setMoneyStatus(pro->money * 100);
            JNIController::setGoldStatus(pro->diam);
            JNIController::setPlayerName(DATA->getShow()->nickname());
            JNIController::setProductId(pro->id.c_str());
            JNIController::setSidId(DATA->getLogin()->obtain_sid());
            JNIController::isGamePay(item->getTag());
            
            this->schedule(schedule_selector(PurchasePanel::updatePay), 1.f);
        }
    }
#endif
    
}

void PurchasePanel::send105(){
    
    CCLog("<><><><><><> send105");
    string orderId = JNIController::getCpOrderId();
    string productId = JNIController::getProductId();
    CCString* iapId = CCString::createWithFormat("%d钻石", JNIController::getGoldStatus());
    
    DATA->onChargeRequest(orderId, iapId->getCString(), JNIController::getMoneyStatus()/100, JNIController::getGoldStatus());
    
    
    NET->verify_order_android_105(orderId, productId);
}

void PurchasePanel::updatePay(float dt){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (JNIController::getSmsStatus() == 1) {
        JNIController::setSmsStatus(0);
        CCUserDefault::sharedUserDefault()->setBoolForKey("PayBool", false);
        CCLog("<><><><><><> updatePay");
        this->unschedule(SEL_SCHEDULE(&PurchasePanel::updatePay));
        
        LOADING->show_loading();
        string orderId = JNIController::getCpOrderId();
        string productId = JNIController::getProductId();
        CCString* iapId = CCString::createWithFormat("%d钻石", JNIController::getGoldStatus());
        
        DATA->onChargeRequest(orderId, iapId->getCString(), JNIController::getMoneyStatus()/100, JNIController::getGoldStatus());
        
        
//        this->scheduleOnce(SEL_SCHEDULE(&PurchasePanel::send105), 2.f);
        this->scheduleOnce(SEL_SCHEDULE(&PurchasePanel::sendPay), 2.f);
    }else if (JNIController::getSmsStatus() == 2) {
        LOADING->remove();
        
        CCUserDefault::sharedUserDefault()->setStringForKey("CpOrderId", "");
        CCUserDefault::sharedUserDefault()->setIntegerForKey("Product_Index", 100);
        CCUserDefault::sharedUserDefault()->setBoolForKey("PayBool", false);
        JNIController::setSmsStatus(0);
        this->unschedule(SEL_SCHEDULE(&PurchasePanel::updatePay));
    }
#endif
}
void PurchasePanel::sendPay(float dt){
    LOADING->remove();
    this->update_content();
    
    string orderId = JNIController::getCpOrderId();
    DATA->onChargeSuccess(orderId);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "购买成功~!稍后请去邮件查收.");
}

void PurchasePanel::keyBackClicked(){
    num_child++;
    CCLog("===== PurchasePanel  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    this->remove();
}

void PurchasePanel::nc_verify_android_105(CCObject *pObj) {
    LOADING->remove();
    this->update_content();
    
    string orderId = JNIController::getCpOrderId();
    DATA->onChargeSuccess(orderId);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "钻石购买成功~!");
}

void PurchasePanel::nc_verify_iOS_107(CCObject *pObj) {
    LOADING->remove();
    
    string orderId2 = DATA->getLogin()->obtain_UUID();
    DATA->onChargeSuccess(orderId2);
    
    this->update_content();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "钻石购买成功~!");
}

