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
//#include "Reward.h"
#include "Loading2.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSIAPManager.h"
#endif

#define CELL_WIDTH          542
#define CELL_HEIGHT         228


PurchasePanel::~PurchasePanel() {
}

bool PurchasePanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        _content->setScale(0.1);
        this->addChild(_content);
        _content->setVisible(false);
        
        return true;
    }
    else {
        return false;
    }
}

void PurchasePanel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&PurchasePanel::hanle_mail_oper), "HTTP_FINISHED_701", NULL);

    this->init_content();
    this->do_enter();
}

void PurchasePanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool PurchasePanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->do_exit();
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
    _panel = CCSprite::create("res/pic/panel/iap/iap_plane.png");
    _panel->setPosition(DISPLAY->center());
    _content->addChild(_panel);
    
    CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
    txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
    _content->addChild(txt_close);
    
    CCSize panelsize = _panel->boundingBox().size;
    float padding = 9;
    
//    CCSprite* prompt = CCSprite::create("res/pic/panel/iap/iap_prompt.png");
//    prompt->setPosition(ccp(panelsize.width * 0.5, panelsize.height * 0.84));
//    _panel->addChild(prompt);
    
    IAPComp* iap = DATA->getIAP();
    CCArray* products = iap->products();
    int count = products->count();
    const char* png_format = "res/pic/panel/iap/iap_bar_%d.png";
    const char* png_format2 = "res/pic/panel/iap/iap_bar_%dx2.png";
    CCArray* arr = CCArray::createWithCapacity(count);
    for (int i = 0; i < count; ++i) {
        CCObject* pObj = products->objectAtIndex(i);
        ProductItem* pro = (ProductItem* )pObj;
        CCSprite* pic1 = NULL;
        CCSprite* pic2 = NULL;
        if (iap->has_purchased(pro->id.c_str())) {
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
        CCMenuItemSprite* btn = CCMenuItemSprite::create(pic1, pic2, this, SEL_MenuHandler(&PurchasePanel::on_bar_clicked));
        btn->setUserObject(pObj);
        arr->addObject(btn);
    }
    
    CCMenu* menu = CCMenu::createWithArray(arr);
    menu->alignItemsVerticallyWithPadding(padding);
    menu->setPosition(menu->getPosition() - ccp(8, 18));
    _content->addChild(menu);
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
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
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
    this->removeFromParentAndCleanup(true);
}

void PurchasePanel::on_bar_clicked(CCMenuItem *item) {
    ProductItem* pro = (ProductItem* )item->getUserObject();
    CCLOG("clicked %s", pro->id.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//    LOADING->show_loading();
    IOSIAPManager* d = IOSIAPManager::Inst();
    if (d->canMakePurchases()) {
        CCLOG("can purchases");
        d->buyProduct(pro->id.c_str());
    }
    else {
        CCLOG("can not purchases");
    }
#endif
}


