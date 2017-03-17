//
//  SalesPromotionLayer.cpp
//  tiegao
//
//  Created by mac on 17-3-8.
//
//

#include "SalesPromotionLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "AudioManager.h"
#include "Shower.h"
#include "QingjingScene.h"
#include "Loading2.h"
#include "NetManager.h"
#include "RewardLayer.h"
#include "ExchangeLayer.h"
#include "ClothesScene.h"
#include "MainScene.h"
#include "PromptLayer.h"


SalesPromotionLayer::~SalesPromotionLayer(){
    
}
bool SalesPromotionLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    clthedPhase = 0;
    
    this->creat_Clothes();
    
    CCSprite* bgSpr = CCSprite::create("res/pic/gashapon/gashapon_bg2.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->creat_View();
    
    
    return true;
}
void SalesPromotionLayer::onEnter(){
    CCLayer::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&SalesPromotionLayer::_161CallBack), "HTTP_FINISHED_161", NULL);
    
    this->scheduleOnce(SEL_SCHEDULE(&SalesPromotionLayer::keyBackStatus), .8f);
}
void SalesPromotionLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}

void SalesPromotionLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void SalesPromotionLayer::keyBackClicked(){
    num_child++;
    CCLog("===== GashaponLayer  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    AUDIO->goback_effect();
    num_child = 0;
//    this->removeFromParentAndCleanup(true);
    
    CCScene* scene = MainScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

bool SalesPromotionLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (! kuangSpr->boundingBox().containsPoint(location)) {
        AUDIO->goback_effect();
        
//        this->removeFromParentAndCleanup(true);
        num_child = 0;
        CCScene* scene = MainScene::scene();
        CCDirector::sharedDirector()->replaceScene(scene);
    }
    
    return true;
}
void SalesPromotionLayer::creat_View(){
    CCSprite* bgSpr2 = CCSprite::create("res/pic/mask.png");
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr2);
    
    kuangSpr = CCSprite::create("res/pic/salesPromotion/sp_kuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(kuangSpr, 2);
    
    _ManSpr = CCSprite::create();
    kuangSpr->addChild(_ManSpr, 10);
    
    // 提示
    CCSprite* tishiSpr = CCSprite::create("res/pic/salesPromotion/sp_tishi.png");
    tishiSpr->setAnchorPoint(ccp(.5f, 0));
    tishiSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, 3));
    kuangSpr->addChild(tishiSpr);
    
    
    if (!this->judgeClothes()) {
        // 购买按钮
        CCSprite* buySpr1 = CCSprite::create("res/pic/salesPromotion/sp_goumai.png");
        CCSprite* buySpr2 = CCSprite::create("res/pic/salesPromotion/sp_goumai.png");
        buySpr2->setScale(1.02f);
        CCMenuItem* buyItem = CCMenuItemSprite::create(buySpr1, buySpr2, this, menu_selector(SalesPromotionLayer::buyCallBack));
        buyItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
        buyMenu = CCMenu::create(buyItem, NULL);
        buyMenu->setPosition(CCPointZero);
        buyMenu->setTag(0x889988);
        kuangSpr->addChild(buyMenu, 20);
        
        CCString* zheStr = CCString::createWithFormat("%d折", (int)DATA->getFlashSaleInfo()->getDiscount());
        CCLabelTTF* zheLabel = CCLabelTTF::create(zheStr->getCString(), DISPLAY->fangzhengFont(), 23);
        zheLabel->setAnchorPoint(ccp(0, .5f));
        zheLabel->setPosition(ccp(7, buyItem->getContentSize().height* .82f));
        zheLabel->setColor(ccc3(254, 253, 131));
        buyItem->addChild(zheLabel);
        
        // title
        CCLabelTTF* titleLabel = CCLabelTTF::create("全部获得", DISPLAY->fangzhengFont(), 24);
        titleLabel->setPosition(ccp(buyItem->getContentSize().width* .5f, buyItem->getContentSize().height* .72f));
        titleLabel->setColor(ccc3(160, 75, 46));
        buyItem->addChild(titleLabel);
        
        // 钻石 old
        CCString* oldPriceStr = CCString::createWithFormat("%d", DATA->getFlashSaleInfo()->getOldPrice());
        CCLabelTTF* oldPriceLabel = CCLabelTTF::create(oldPriceStr->getCString(), DISPLAY->fangzhengFont(), 27);
        oldPriceLabel->setPosition(ccp(buyItem->getContentSize().width* .2f, buyItem->getContentSize().height* .33f));
        oldPriceLabel->setColor(ccc3(230, 98, 98));
        buyItem->addChild(oldPriceLabel);
        CCSprite* goldSpr1 = CCSprite::create("res/pic/salesPromotion/sp_gold.png");
        goldSpr1->setPosition(ccp(oldPriceLabel->getContentSize().width + 10, oldPriceLabel->getContentSize().height* .5f));
        oldPriceLabel->addChild(goldSpr1);
        CCSprite* xianSpr = CCSprite::create("res/pic/salesPromotion/sp_xiegang.png");
        xianSpr->setPosition(ccp(buyItem->getContentSize().width* .23f, buyItem->getContentSize().height* .33f));
        buyItem->addChild(xianSpr, 5);
        
        // 钻石 now
        CCSprite* nowPriceDi = CCSprite::create("res/pic/salesPromotion/sp_di.png");
        nowPriceDi->setPosition(ccp(buyItem->getContentSize().width* .73f, buyItem->getContentSize().height* .33f));
        buyItem->addChild(nowPriceDi);
        CCString* nowPriceStr = CCString::createWithFormat("%d", DATA->getFlashSaleInfo()->getNowPrice());
        CCLabelTTF* nowPriceLabel = CCLabelTTF::create(nowPriceStr->getCString(), DISPLAY->fangzhengFont(), 28);
        nowPriceLabel->setPosition(ccp(nowPriceDi->getContentSize().width* .31f, nowPriceDi->getContentSize().height* .48f));
        nowPriceLabel->setColor(ccc3(254, 253, 131));
        nowPriceDi->addChild(nowPriceLabel);
        CCSprite* goldSpr2 = CCSprite::create("res/pic/salesPromotion/sp_gold.png");
        goldSpr2->setPosition(ccp(nowPriceLabel->getContentSize().width + 10, nowPriceLabel->getContentSize().height* .5f));
        nowPriceLabel->addChild(goldSpr2);
    }else{
        CCSprite* changeSpr1 = CCSprite::create("res/pic/salesPromotion/sp_change.png");
        CCSprite* changeSpr2 = CCSprite::create("res/pic/salesPromotion/sp_change.png");
        changeSpr2->setScale(1.02f);
        CCMenuItem* buyItem = CCMenuItemSprite::create(changeSpr1, changeSpr2, this, menu_selector(SalesPromotionLayer::changeCallBack));
        buyItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
        buyMenu = CCMenu::create(buyItem, NULL);
        buyMenu->setPosition(CCPointZero);
        buyMenu->setTag(0x889988);
        kuangSpr->addChild(buyMenu, 20);
    }
    
    
    
    
    // 倒计时
    CCSprite* timeSpr = CCSprite::create("res/pic/salesPromotion/sp_time.png");
    timeSpr->setAnchorPoint(ccp(1, .5f));
    timeSpr->setPosition(ccp(kuangSpr->getContentSize().width - 7, kuangSpr->getContentSize().height* .12f));
    kuangSpr->addChild(timeSpr, 20);
    
    //初始化时间
    this->initTime();
    CCString* timeStr;
    CCString* hourStr;
    CCString* minuteStr;
    CCString* secondStr;
    if (_hour >= 0 && _hour < 10) {
        hourStr = CCString::createWithFormat("0%d", _hour);
    }else{
        hourStr = CCString::createWithFormat("%d", _hour);
    }
    
    if (_minute >= 0 && _minute < 10) {
        minuteStr = CCString::createWithFormat("0%d", _minute);
    }else{
        minuteStr = CCString::createWithFormat("%d", _minute);
    }
    
    if (_second >= 0 && _second < 10) {
        secondStr = CCString::createWithFormat("0%d", _second);
    }else{
        secondStr = CCString::createWithFormat("%d", _second);
    }
    timeStr = CCString::createWithFormat("  倒计时\n %s:%s:%s", hourStr->getCString(), minuteStr->getCString(), secondStr->getCString());
    
    timeLabel = CCLabelTTF::create(timeStr->getCString(), DISPLAY->fangzhengFont(), 18);
    timeLabel->setPosition(ccp(timeSpr->getContentSize().width* .53f, timeSpr->getContentSize().height* .5f));
    timeLabel->setColor(ccc3(255, 255, 255));
    timeSpr->addChild(timeLabel);
    this->schedule(SEL_SCHEDULE(&SalesPromotionLayer::updateTime), 1.f);
    
    
    this->creat_kuang();
    this->creat_Man();
    this->initClothes();
}
bool SalesPromotionLayer::judgeClothes(){
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    CCArray* suitsArr = (CCArray* )DATA->getFlashSaleInfo()->getClothes();
    
    bool judgeBool[suitsArr->count()];
    for (int i = 0; i < suitsArr->count(); i++) {
        judgeBool[i] = false;
    }
    
    for (int i = 0; i < suitsArr->count(); i++) {
        int index = ((CCInteger* )suitsArr->objectAtIndex(i))->getValue();
        if (index >= 10000 && index < 20000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_TouFa, index);
        }else if (index >= 20000 && index < 30000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_WaiTao, index);
        }else if (index >= 30000 && index < 40000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_ShangYi, index);
        }else if (index >= 40000 && index < 50000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_KuZi, index);
        }else if (index >= 50000 && index < 60000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_WaZi, index);
        }else if (index >= 60000 && index < 70000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_XieZi, index);
        }else if (index >= 70000 && index < 80000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_ShiPin, index);
        }else if (index >= 80000 && index < 90000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_Bao, index);
        }else if (index >= 90000 && index < 100000){
            judgeBool[i] = DATA->getClothes()->is_owned(Tag_CL_ZhuangRong, index);
        }
    }
    
    bool tempBool = true;
    for (int i = 0; i < suitsArr->count(); i++) {
        if (judgeBool[i]) {
            tempBool = true;
        }else{
            tempBool = false;
            break;
        }
    }
    
    return tempBool;
}
void SalesPromotionLayer::changeCallBack(CCObject* pSender){
    this->save_Clothes();
    
    CCLayer* layer = ClothesScene::create_with_type(6, 0, 0);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void SalesPromotionLayer::initTime(){
    long timeLong = DATA->getFlashSaleInfo()->getDeadline();
    _hour = (int)timeLong/3600;                             // 小时
    _minute = (int)(timeLong - _hour*3600)/60;              // 分钟
    _second = (int)(timeLong - _hour*3600 - _minute*60);    // 秒
}
void SalesPromotionLayer::updateTime(float dt){
    CCString* hourStr;
    CCString* minuteStr;
    CCString* secondStr;
    CCString* timeStr;
    bool closedBool;
    
    _second--;
    if (_hour == 0 && _minute == 0 && _second == 0) {
        _hour = 0;
        _minute = 0;
        _second = 0;
        closedBool = true;
    }else{
        if (_second < 0) {
            _minute--;
            if (_minute < 0) {
                _hour--;
                if (_hour < 0) {
                    _hour = 0;
                    _minute = 0;
                    _second = 0;
                    
                    closedBool = true;
                }else{
                    _minute = 59;
                    _second = 59;
                }
            }else{
                _second = 59;
            }
        }
    }
    
    if (_hour >= 0 && _hour < 10) {
        hourStr = CCString::createWithFormat("0%d", _hour);
    }else{
        hourStr = CCString::createWithFormat("%d", _hour);
    }
    
    if (_minute >= 0 && _minute < 10) {
        minuteStr = CCString::createWithFormat("0%d", _minute);
    }else{
        minuteStr = CCString::createWithFormat("%d", _minute);
    }
    
    if (_second >= 0 && _second < 10) {
        secondStr = CCString::createWithFormat("0%d", _second);
    }else{
        secondStr = CCString::createWithFormat("%d", _second);
    }
    timeStr = CCString::createWithFormat("  倒计时\n %s:%s:%s", hourStr->getCString(), minuteStr->getCString(), secondStr->getCString());
    timeLabel->setString(timeStr->getCString());
    
    if (closedBool) {
        this->unschedule(SEL_SCHEDULE(&SalesPromotionLayer::updateTime));
        return;
    }
}
void SalesPromotionLayer::buyCallBack(CCObject* pSender){
    
    if (clthedPhase > DATA->getPlayer()->phase) {//衣服等级大于当前等级
        AHMessageBox2* mb = AHMessageBox2::create_with_message2("衣服等级高于当前等级,是否继续购买?", this, AH_AVATAR_TYPE2_NO, AH_BUTTON_TYPE2_YESNO, false);
        mb->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 4000);
    }else{
        LOADING->show_loading();
        NET->flash_sale_buying_161();
    }
}
void SalesPromotionLayer::message_box_did_selected_button2(AHMessageBox2* box, AH_BUTTON_TYPE2 button_type, AH_BUTTON_TAGS2 button_tag){
    box->animation_out();
    
    if (button_type == AH_BUTTON_TYPE2_YESNO) {
        if (button_tag == AH_BUTTON_TAG2_YES) {
            LOADING->show_loading();
            NET->flash_sale_buying_161();
        }else if (button_tag == AH_BUTTON_TAG2_NO){
            
        }
    }
}

void SalesPromotionLayer::_161CallBack(CCObject* pSender){
    LOADING->remove();
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney", NULL);
    PromptLayer* layer = PromptLayer::create();
    layer->show_prompt(this->getScene(), "购买成功!");
    
    if (buyMenu != NULL) {
        buyMenu->removeAllChildren();
        if (this->getChildByTag(0x889988) != NULL) {
            this->removeChildByTag(0x889988);
        }
    }
    
    if (!this->judgeClothes()) {
        // 购买按钮
        CCSprite* buySpr1 = CCSprite::create("res/pic/salesPromotion/sp_goumai.png");
        CCSprite* buySpr2 = CCSprite::create("res/pic/salesPromotion/sp_goumai.png");
        buySpr2->setScale(1.02f);
        CCMenuItem* buyItem = CCMenuItemSprite::create(buySpr1, buySpr2, this, menu_selector(SalesPromotionLayer::buyCallBack));
        buyItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
        buyMenu = CCMenu::create(buyItem, NULL);
        buyMenu->setPosition(CCPointZero);
        buyMenu->setTag(0x889988);
        kuangSpr->addChild(buyMenu, 20);
        
        CCString* zheStr = CCString::createWithFormat("%d折", (int)DATA->getFlashSaleInfo()->getDiscount());
        CCLabelTTF* zheLabel = CCLabelTTF::create(zheStr->getCString(), DISPLAY->fangzhengFont(), 23);
        zheLabel->setAnchorPoint(ccp(0, .5f));
        zheLabel->setPosition(ccp(7, buyItem->getContentSize().height* .82f));
        zheLabel->setColor(ccc3(254, 253, 131));
        buyItem->addChild(zheLabel);
        
        // title
        CCLabelTTF* titleLabel = CCLabelTTF::create("全部获得", DISPLAY->fangzhengFont(), 24);
        titleLabel->setPosition(ccp(buyItem->getContentSize().width* .5f, buyItem->getContentSize().height* .72f));
        titleLabel->setColor(ccc3(160, 75, 46));
        buyItem->addChild(titleLabel);
        
        // 钻石 old
        CCString* oldPriceStr = CCString::createWithFormat("%d", DATA->getFlashSaleInfo()->getOldPrice());
        CCLabelTTF* oldPriceLabel = CCLabelTTF::create(oldPriceStr->getCString(), DISPLAY->fangzhengFont(), 27);
        oldPriceLabel->setPosition(ccp(buyItem->getContentSize().width* .2f, buyItem->getContentSize().height* .33f));
        oldPriceLabel->setColor(ccc3(230, 98, 98));
        buyItem->addChild(oldPriceLabel);
        CCSprite* goldSpr1 = CCSprite::create("res/pic/salesPromotion/sp_gold.png");
        goldSpr1->setPosition(ccp(oldPriceLabel->getContentSize().width + 10, oldPriceLabel->getContentSize().height* .5f));
        oldPriceLabel->addChild(goldSpr1);
        CCSprite* xianSpr = CCSprite::create("res/pic/salesPromotion/sp_xiegang.png");
        xianSpr->setPosition(ccp(buyItem->getContentSize().width* .23f, buyItem->getContentSize().height* .33f));
        buyItem->addChild(xianSpr, 5);
        
        // 钻石 now
        CCSprite* nowPriceDi = CCSprite::create("res/pic/salesPromotion/sp_di.png");
        nowPriceDi->setPosition(ccp(buyItem->getContentSize().width* .73f, buyItem->getContentSize().height* .33f));
        buyItem->addChild(nowPriceDi);
        CCString* nowPriceStr = CCString::createWithFormat("%d", DATA->getFlashSaleInfo()->getNowPrice());
        CCLabelTTF* nowPriceLabel = CCLabelTTF::create(nowPriceStr->getCString(), DISPLAY->fangzhengFont(), 28);
        nowPriceLabel->setPosition(ccp(nowPriceDi->getContentSize().width* .31f, nowPriceDi->getContentSize().height* .48f));
        nowPriceLabel->setColor(ccc3(254, 253, 131));
        nowPriceDi->addChild(nowPriceLabel);
        CCSprite* goldSpr2 = CCSprite::create("res/pic/salesPromotion/sp_gold.png");
        goldSpr2->setPosition(ccp(nowPriceLabel->getContentSize().width + 10, nowPriceLabel->getContentSize().height* .5f));
        nowPriceLabel->addChild(goldSpr2);
    }else{
        CCSprite* changeSpr1 = CCSprite::create("res/pic/salesPromotion/sp_change.png");
        CCSprite* changeSpr2 = CCSprite::create("res/pic/salesPromotion/sp_change.png");
        changeSpr2->setScale(1.02f);
        CCMenuItem* buyItem = CCMenuItemSprite::create(changeSpr1, changeSpr2, this, menu_selector(SalesPromotionLayer::changeCallBack));
        buyItem->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .12f));
        buyMenu = CCMenu::create(buyItem, NULL);
        buyMenu->setPosition(CCPointZero);
        buyMenu->setTag(0x889988);
        kuangSpr->addChild(buyMenu, 20);
    }
}
void SalesPromotionLayer::creat_kuang(){
    CCArray* suitsArr = (CCArray* )DATA->getFlashSaleInfo()->getClothes();
    for (int i = 0; i < suitsArr->count(); i++) {
        int idIndex = ((CCInteger* )suitsArr->objectAtIndex(i))->getValue();
//        for (int i = 0; i < 8; i++) {
        CCSprite* diSpr = CCSprite::create("res/pic/salesPromotion/sp_kuang2.png");
        float heightFloat1 = .81f;
        float heightFloat2 = .18f;
        if (i == 1 || i == 3 || i == 5 || i == 7) {
            if (i == 1) {
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 0)));
            }else if (i == 3){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 1)));
            }else if (i == 5){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 2)));
            }else if (i == 7){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .85f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 3)));
            }
        }else{
            if (i == 0) {
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 0)));
            }else if (i == 2){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 1)));
            }else if (i == 4){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 2)));
            }else if (i == 6){
                diSpr->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height* (heightFloat1 - heightFloat2* 3)));
            }
        }
        this->creat_Clothes(diSpr, idIndex);
        diSpr->setTag(i);
        kuangSpr->addChild(diSpr, 10);
        
    }
}
void SalesPromotionLayer::creat_Clothes(CCSprite* spr, int index){
    CCString* iconStr;
    if (index >= 10000 && index < 20000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%d.png", index);
    }else if (index >= 20000 && index < 30000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%d.png", index);
    }else if (index >= 30000 && index < 40000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%d.png", index);
    }else if (index >= 40000 && index < 50000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%d.png", index);
    }else if (index >= 50000 && index < 60000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%d.png", index);
    }else if (index >= 60000 && index < 70000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%d.png", index);
    }else if (index >= 70000 && index < 80000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%d.png", index);;
    }else if (index >= 80000 && index < 90000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%d.png", index);
    }else if (index >= 90000 && index < 100000){
        iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%d.png", index);
    }
    CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
    iconSpr->setPosition(ccp(spr->getContentSize().width* .5f, spr->getContentSize().height* .5f));
    iconSpr->setScale(.7f);
    spr->addChild(iconSpr);
    
    this->haveClothes(spr, index);
}
void SalesPromotionLayer::haveClothes(CCSprite* spr, int index){
    bool haveBool = false;
    if (index >= 10000 && index < 20000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_TouFa, index);
    }else if (index >= 20000 && index < 30000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_WaiTao, index);
    }else if (index >= 30000 && index < 40000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_ShangYi, index);
    }else if (index >= 40000 && index < 50000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_KuZi, index);
    }else if (index >= 50000 && index < 60000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_WaZi, index);
    }else if (index >= 60000 && index < 70000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_XieZi, index);
    }else if (index >= 70000 && index < 80000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_ShiPin, index);
    }else if (index >= 80000 && index < 90000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_Bao, index);
    }else if (index >= 90000 && index < 100000){
        haveBool = DATA->getClothes()->is_owned(Tag_CL_ZhuangRong, index);
    }
    
    if (haveBool) {
        CCSprite* haveSpr = CCSprite::create("res/pic/salesPromotion/sp_have.png");
        haveSpr->setPosition(ccp(spr->getContentSize().width* .15f, spr->getContentSize().height* .8f));
        spr->addChild(haveSpr, 10);
    }
}


void SalesPromotionLayer::creat_Man(){
    float widthFolt = .63f;
    float heightFloat = .55f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height * heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void SalesPromotionLayer::initClothes(){//穿衣服
    float widthFolt = .63f;
    float heightFloat = .55f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    int sub_part = 0;
    
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _tfSpr2->setTag(Tag_QJ_TouFa2);
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
                            _tfSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _wtSpr1->setTag(Tag_QJ_WaiTao1);
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
                            _wtSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _sySpr1->setTag(Tag_QJ_ShangYi1);
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
                            _sySpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
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
                            _kzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _wzSpr1->setTag(Tag_QJ_WaZi1);
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
                            _wzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _xzSpr1->setTag(Tag_QJ_XieZi1);
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
                            _xzSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
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
                                _spSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_QJ_Bao){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                _bSpr1->setTag(Tag_QJ_Bao1);
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
                            _bSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_QJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)nowClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
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
                            _zrSpr1->setPosition(ccp(kuangSpr->getContentSize().width* widthFolt, kuangSpr->getContentSize().height* heightFloat));
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
void SalesPromotionLayer::creat_Clothes(){
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    CCArray* suitsArr = (CCArray* )DATA->getFlashSaleInfo()->getClothes();
    nowClothesTemp = CCDictionary::create();
    
    for (int i = Tag_CL_TouFa; i <= Tag_CL_ZhuangRong; i++) {
        CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
        bool clothesBool = false;
        if (i != Tag_CL_ShiPin) {
            CCInteger* clothesId;
            CCString* keyStr;
            for (int j = 0; j < clothesArr->count(); j++) {
                CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                int clthedIndex = clothDic->valueForKey("id")->intValue();
//                CCLog("clthedIndex == %d", clthedIndex);
                keyStr = CCString::createWithFormat("%d", i);
                for (int k = 0; k < suitsArr->count(); k++) {
                    int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                    if (nowClothesId == clthedIndex) {
                        clothesId = CCInteger::create(clthedIndex);
                        clothesBool = true;
                        break;
                    }
                }
                if (clothesBool) {
                    clthedPhase = clothDic->valueForKey("phase")->intValue();
                    break;
                }
            }
            if (clothesBool) {
                clothesBool = false;
                nowClothesTemp->setObject(clothesId, keyStr->getCString());
            }else{
                clothesBool = false;
                CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                nowClothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else{
            CCDictionary* shipinDic = CCDictionary::create();
            CCString* keyStr = CCString::createWithFormat("%d", i);
            
            for (int n = 11; n <= 20; n++) {
                CCInteger* clothesId;
                CCString* subkeyStr;
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int clthedIndex = clothDic->valueForKey("id")->intValue();
//                    CCLog("clthedIndex == %d", clthedIndex);
                    subkeyStr = CCString::createWithFormat("%d", n);
                    for (int k = 0; k < suitsArr->count(); k++) {
                        int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                        if (nowClothesId == clthedIndex) {
                            clothesBool = true;
                            clothesId = CCInteger::create(clthedIndex);
                            break;
                        }
                    }
                    if (clothesBool) {
                        break;
                    }
                }
                if (clothesBool) {
                    clothesBool = false;
                    shipinDic->setObject(clothesId, subkeyStr->getCString());
                }else{
                    clothesBool = false;
                    CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                    shipinDic->setObject(cloth_integer, subkeyStr->getCString());
                }
            }
            nowClothesTemp->setObject(shipinDic, keyStr->getCString());
        }
    }
    
}
void SalesPromotionLayer::save_Clothes(){
    CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
    CCArray* suitsArr = (CCArray* )DATA->getFlashSaleInfo()->getClothes();
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp();
    
    for (int i = Tag_CL_TouFa; i <= Tag_CL_ZhuangRong; i++) {
        CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
        bool clothesBool = false;
        if (i != Tag_CL_ShiPin) {
            CCInteger* clothesId;
            CCString* keyStr;
            for (int j = 0; j < clothesArr->count(); j++) {
                CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                int clthedIndex = clothDic->valueForKey("id")->intValue();
//                CCLog("clthedIndex == %d", clthedIndex);
                keyStr = CCString::createWithFormat("%d", i);
                for (int k = 0; k < suitsArr->count(); k++) {
                    int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                    if (nowClothesId == clthedIndex) {
                        clothesId = CCInteger::create(clthedIndex);
                        clothesBool = true;
                        break;
                    }
                }
                if (clothesBool) {
                    break;
                }
            }
            if (clothesBool) {
                clothesBool = false;
                myClothesTemp->setObject(clothesId, keyStr->getCString());
            }else{
                clothesBool = false;
                CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                myClothesTemp->setObject(cloth_integer, keyStr->getCString());
            }
        }else{
            CCDictionary* shipinDic = CCDictionary::create();
            CCString* keyStr = CCString::createWithFormat("%d", i);
            
            for (int n = 11; n <= 20; n++) {
                CCInteger* clothesId;
                CCString* subkeyStr;
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int clthedIndex = clothDic->valueForKey("id")->intValue();
//                    CCLog("clthedIndex == %d", clthedIndex);
                    subkeyStr = CCString::createWithFormat("%d", n);
                    for (int k = 0; k < suitsArr->count(); k++) {
                        int nowClothesId = ((CCInteger* )suitsArr->objectAtIndex(k))->getValue();
                        if (nowClothesId == clthedIndex) {
                            clothesBool = true;
                            clothesId = CCInteger::create(clthedIndex);
                            break;
                        }
                    }
                    if (clothesBool) {
                        break;
                    }
                }
                if (clothesBool) {
                    clothesBool = false;
                    shipinDic->setObject(clothesId, subkeyStr->getCString());
                }else{
                    clothesBool = false;
                    CCInteger* cloth_integer = CCInteger::create(updataClothes(i));
                    shipinDic->setObject(cloth_integer, subkeyStr->getCString());
                }
            }
            myClothesTemp->setObject(shipinDic, keyStr->getCString());
        }
    }
}
int SalesPromotionLayer::updataClothes(int type){
    return (type) * 10000;
}







