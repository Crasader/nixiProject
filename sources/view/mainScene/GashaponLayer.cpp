//
//  GashaponLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-11.
//
//

#include "GashaponLayer.h"
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


GashaponLayer::~GashaponLayer(){
    
}

bool GashaponLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    freeBool = false;
    nowIndex = 0;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    suitsArr = (CCArray* )DATA->getOperation()->getSuits();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->creat_View();
    
    
    return true;
}
void GashaponLayer::onEnter(){
    CCLayer::onEnter();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&GashaponLayer::_307CallBack), "HTTP_FINISHED_307", NULL);
    nc->addObserver(this, SEL_CallFuncO(&GashaponLayer::_309CallBack), "HTTP_FINISHED_309", NULL);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&GashaponLayer::keyBackStatus), .8f);
}

void GashaponLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void GashaponLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    
    CCLayer::onExit();
}


void GashaponLayer::keyBackClicked(){
    
}

bool GashaponLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint location = pTouch->getLocation();
    if (! bgSpr->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

void GashaponLayer::creat_View(){
    CCSprite* bgSpr2 = CCSprite::create("res/pic/gashapon/gashapon_bg2.png");
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr2);
    
    bgSpr = CCSprite::create("res/pic/gashapon/gashapon_bg.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr, 2);
    
    _ManSpr = CCSprite::create();
    bgSpr->addChild(_ManSpr, 10);
    
    CCSprite* shopSpr1 = CCSprite::create("res/pic/gashapon/gashapon_label1.png");
    shopSpr1->setPosition(ccp(bgSpr->getContentSize().width* .1f, bgSpr->getContentSize().height* .7f));
    bgSpr->addChild(shopSpr1, 10);
    CCSprite* shopSpr2 = CCSprite::create("res/pic/gashapon/gashapon_label2.png");
    shopSpr2->setAnchorPoint(ccp(.5f, 1));
    shopSpr2->setPosition(ccp(bgSpr->getContentSize().width* .1f, bgSpr->getContentSize().height* .7f - shopSpr1->getContentSize().width* .5f));
    bgSpr->addChild(shopSpr2, 10);
    
    
    myClothesTemp = (CCDictionary*)suitsArr->objectAtIndex(nowIndex);
    
    this->creat_Man();
    this->initClothes();
    
//    // 底1
//    CCSprite* diSpr1 = CCSprite::create("res/pic/gashapon/gashapon_di1.png");
//    diSpr1->setPosition(ccp(bgSpr->getContentSize().width* .5f, bgSpr->getContentSize().height* .2f));
////    diSpr1->setColor(ccYELLOW);
//    bgSpr->addChild(diSpr1, 20);
//    CCString* labelStr1 = CCString::createWithFormat("这些漂亮的饰品都可以随机抽到的呦~!快来抽吧.10连抽获得整件饰品的概率更大呦~!");
//    CCLabelTTF* label1 = CCLabelTTF::create(labelStr1->getCString(), DISPLAY->fangzhengFont(), 15, CCSizeMake(diSpr1->getContentSize().width* .95f, diSpr1->getContentSize().height* .7f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
//    label1->setPosition(ccp(diSpr1->getContentSize().width* .5f, diSpr1->getContentSize().height* .45f));
//    label1->setColor(ccc3(157, 108, 151));
//    diSpr1->addChild(label1);
    
    // 底3
    CCSprite* diSpr3 = CCSprite::create("res/pic/gashapon/gashapon_di3.png");
    diSpr3->setPosition(ccp(bgSpr->getContentSize().width* .5f, bgSpr->getContentSize().height* .17f));
    bgSpr->addChild(diSpr3, 10);
    CCString* labelStr2 = CCString::createWithFormat("%d/%d", nowIndex+1, suitsArr->count());
    label2 = CCLabelTTF::create(labelStr2->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(diSpr3->getContentSize().width* .95f, 18), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label2->setPosition(ccp(diSpr3->getContentSize().width* .5f, diSpr3->getContentSize().height* .5f));
    label2->setColor(ccc3(249, 128, 128));
    diSpr3->addChild(label2);
    
    
    // 碎片兑换
    CCSprite* spSpr1 = CCSprite::create("res/pic/gashapon/gashapon_suipian1.png");
    spSpr1->setScale(1.2f);
    CCSprite* spSpr2 = CCSprite::create("res/pic/gashapon/gashapon_suipian1.png");
    spSpr2->setScale(1.22f);
    CCMenuItem* suipianItem = CCMenuItemSprite::create(spSpr1, spSpr2, this, menu_selector(GashaponLayer::suipianCallBack));
    suipianItem->setPosition(ccp(bgSpr->getContentSize().width* .86f, bgSpr->getContentSize().height* .93f));
    CCSprite* suipianSpr = CCSprite::create("res/pic/gashapon/gashapon_suipian2.png");
    suipianSpr->setPosition(ccp(suipianItem->getContentSize().width* .29f, suipianItem->getContentSize().height* .63f));
    suipianSpr->setScale(1.3f);
    suipianItem->addChild(suipianSpr);
    
    // 1次
    CCSprite* oneSpr1 = CCSprite::create("res/pic/gashapon/gashapon_button1.png");
    CCSprite* oneSpr2 = CCSprite::create("res/pic/gashapon/gashapon_button1.png");
    oneSpr2->setScale(1.02f);
    oneItem = CCMenuItemSprite::create(oneSpr1, oneSpr2, this, menu_selector(GashaponLayer::oneCallBack));
    oneItem->setPosition(ccp(bgSpr->getContentSize().width* .3f, bgSpr->getContentSize().height* .12f));
    CCString* oneStr = CCString::createWithFormat("抽1次");
    oneLabel = CCLabelTTF::create(oneStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(oneSpr1->getContentSize().width* .95f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    oneLabel->setPosition(ccp(oneItem->getContentSize().width* .5f, oneItem->getContentSize().height* .5f));
    oneLabel->setColor(ccWHITE);
    oneItem->addChild(oneLabel);
    CCString* goldStr = CCString::createWithFormat("%d", DATA->getOperation()->getSignleCost());
    oneGoldLabel = CCLabelTTF::create(goldStr->getCString(), DISPLAY->fangzhengFont(), 14);
    oneGoldLabel->setPosition(ccp(oneItem->getContentSize().width* .65f, oneItem->getContentSize().height* .22f));
    oneGoldLabel->setColor(ccc3(255, 216, 228));
    oneItem->addChild(oneGoldLabel);
    
    CCSprite* goldSpr = CCSprite::create("res/pic/gashapon/gashapon_gold.png");
    goldSpr->setScale(1.2f);
    goldSpr->setPosition(ccp(oneGoldLabel->getContentSize().width + 5, oneGoldLabel->getContentSize().height* .53f));
    oneGoldLabel->addChild(goldSpr);
    // 倒计时
    // 底2
    this->initTime();
    diSpr2 = CCSprite::create("res/pic/gashapon/gashapon_di2.png");
    diSpr2->setPosition(ccp(oneItem->getContentSize().width* .5f, -13));
    oneItem->addChild(diSpr2);
    CCString* timeStr;
    if (_hour == 0 && _minute == 0 && _second == 0) {
        freeBool = true;
        diSpr2->setVisible(false);
        oneLabel->setString("免费");
        oneGoldLabel->setVisible(false);
        timeStr = CCString::createWithFormat("");
    }else{
        if (_hour > 0) {
            if (_minute >= 0 && _minute < 10) {
                timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_hour, _minute);
            }else{
                timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_hour, _minute);
            }
        }else{
            if (_minute >= 0 && _minute < 10) {
                if (_second >= 0 && _second < 10) {
                    timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_minute, _second);
                }else{
                    timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_minute, _second);
                }
            }else{
                if (_second >= 0 && _second < 10) {
                    timeStr = CCString::createWithFormat("免费倒计时%d:0%d",_minute, _second);
                }else{
                    timeStr = CCString::createWithFormat("免费倒计时%d:%d",_minute, _second);
                }
            }
        }
        this->schedule(SEL_SCHEDULE(&GashaponLayer::updateTime), 1.f);
    }
    timeLabel = CCLabelTTF::create(timeStr->getCString(), DISPLAY->fangzhengFont(), 12, CCSizeMake(diSpr2->getContentSize().width* .95f, 12), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    timeLabel->setPosition(ccp(diSpr2->getContentSize().width* .5f, diSpr2->getContentSize().height* .48f));
    timeLabel->setColor(ccWHITE);
    diSpr2->addChild(timeLabel);
    
    
    // 10次
    CCSprite* tenSpr1 = CCSprite::create("res/pic/gashapon/gashapon_button2.png");
    CCSprite* tenSpr2 = CCSprite::create("res/pic/gashapon/gashapon_button2.png");
    tenSpr2->setScale(1.02f);
    CCMenuItem* tenItem = CCMenuItemSprite::create(tenSpr1, tenSpr2, this, menu_selector(GashaponLayer::tenCallBack));
    tenItem->setPosition(ccp(bgSpr->getContentSize().width* .7f, bgSpr->getContentSize().height* .12f));
    CCString* tenStr = CCString::createWithFormat("10连抽");
    CCLabelTTF* tenLabel = CCLabelTTF::create(tenStr->getCString(), DISPLAY->fangzhengFont(), 20, CCSizeMake(tenSpr1->getContentSize().width* .95f, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    tenLabel->setPosition(ccp(tenItem->getContentSize().width* .5f, tenItem->getContentSize().height* .5f));
    tenLabel->setColor(ccWHITE);
    tenItem->addChild(tenLabel);
    this->creat_gold(tenItem, 10);
    
    CCMenu* menu = CCMenu::create(suipianItem, oneItem, tenItem, NULL);
    menu->setPosition(CCPointZero);
    bgSpr->addChild(menu, 30);
    
    this->schedule(SEL_SCHEDULE(&GashaponLayer::updataClothes1), 5.f);
}
void GashaponLayer::initTime(){
    long timeLong = DATA->getOperation()->getFreePoint();
    _hour = (int)timeLong/3600;//小时
    _minute = (int)(timeLong - _hour*3600)/60;//分钟
    _second = (int)(timeLong - _hour*3600 - _minute*60);//秒
    
//    _hour = 0;//小时
//    _minute = 0;//分钟
//    _second = 5;//秒
}
void GashaponLayer::updateTime(float dt){
    _second--;
    if (_hour == 0 && _minute == 0 && _second == 0) {
        freeBool = true;
        diSpr2->setVisible(false);
        oneLabel->setString("免费");
        oneGoldLabel->setVisible(false);
    }else{
        if (_second < 0) {
            _minute--;
            if (_minute < 0) {
                _hour--;
                if (_hour < 0) {
                    _hour = 0;
                    _minute = 0;
                    _second = 0;
                    this->unschedule(SEL_SCHEDULE(&GashaponLayer::updateTime));
                    
                    diSpr2->setVisible(false);
                    oneLabel->setString("免费");
                    oneGoldLabel->setVisible(false);
                    
                    return;
                }else{
                    _minute = 59;
                    _second = 59;
                }
            }else{
                _second = 59;
            }
        }
    }
    
    CCString* timeStr;
    if (_hour > 0) {
        if (_minute >= 0 && _minute < 10) {
            timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_hour, _minute);
        }else{
            timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_hour, _minute);
        }
    }else{
        if (_minute >= 0 && _minute < 10) {
            if (_second >= 0 && _second < 10) {
                timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_minute, _second);
            }else{
                timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_minute, _second);
            }
        }else{
            if (_second >= 0 && _second < 10) {
                timeStr = CCString::createWithFormat("免费倒计时%d:0%d",_minute, _second);
            }else{
                timeStr = CCString::createWithFormat("免费倒计时%d:%d",_minute, _second);
            }
        }
    }
    timeLabel->setString(timeStr->getCString());
    
    CCLog("%d:%d:%d",_hour, _minute, _second);
}
void GashaponLayer::creat_gold(CCMenuItem* item, int index){
    
    CCString* goldStr;
    if (index == 1) {
        goldStr = CCString::createWithFormat("10");
    }else if (index == 10){
        goldStr = CCString::createWithFormat("%d", DATA->getOperation()->getTenCost());
    }
    CCLabelTTF* label = CCLabelTTF::create(goldStr->getCString(), DISPLAY->fangzhengFont(), 14);
    label->setPosition(ccp(item->getContentSize().width* .65f, item->getContentSize().height* .22f));
    label->setColor(ccc3(255, 216, 228));
    label->setTag(index);
    item->addChild(label);
    
    CCSprite* goldSpr = CCSprite::create("res/pic/gashapon/gashapon_gold.png");
    goldSpr->setScale(1.2f);
    goldSpr->setPosition(ccp(label->getContentSize().width + 5, label->getContentSize().height* .53f));
    label->addChild(goldSpr);
}
void GashaponLayer::suipianCallBack(CCObject* pSender){
    this->removeFromParentAndCleanup(true);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("Creat_Exchange");
}
void GashaponLayer::oneCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->single_lottery_307();
}
void GashaponLayer::_307CallBack(CCObject* pSender){
    LOADING->remove();
    if (freeBool) {// 免费1次
        freeBool = false;
//        _hour = 8;//小时
//        _minute = 0;//分钟
//        _second = 0;//秒
        this->initTime();
        diSpr2->setVisible(true);
        oneLabel->setString("抽1次");
        oneGoldLabel->setVisible(true);
        
        CCString* timeStr;
        if (_hour > 0) {
            if (_minute >= 0 && _minute < 10) {
                timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_hour, _minute);
            }else{
                timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_hour, _minute);
            }
        }else{
            if (_minute >= 0 && _minute < 10) {
                if (_second >= 0 && _second < 10) {
                    timeStr = CCString::createWithFormat("免费倒计时0%d:0%d",_minute, _second);
                }else{
                    timeStr = CCString::createWithFormat("免费倒计时0%d:%d",_minute, _second);
                }
            }else{
                if (_second >= 0 && _second < 10) {
                    timeStr = CCString::createWithFormat("免费倒计时%d:0%d",_minute, _second);
                }else{
                    timeStr = CCString::createWithFormat("免费倒计时%d:%d",_minute, _second);
                }
            }
        }
        timeLabel->setString(timeStr->getCString());
        this->schedule(SEL_SCHEDULE(&GashaponLayer::updateTime), 1.f);
    }else{// 收费
        
    }
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    this->creat_Tishi((CCArray* )pSender);
}

void GashaponLayer::tenCallBack(CCObject* pSender){
    LOADING->show_loading();
    NET->multiply_lottery_309();
}
void GashaponLayer::_309CallBack(CCObject* pSender){
    LOADING->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    this->creat_Tishi(pSender);
}


void GashaponLayer::creat_Tishi(CCObject* arr){
    RewardLayer* layer = RewardLayer::create_with_index((CCArray* )arr);
    this->addChild(layer, 100);
    
//    this->removeFromParentAndCleanup(true);
//    CCNotificationCenter::sharedNotificationCenter()->postNotification("linshiMethod", arr);
}


void GashaponLayer::updataClothes1(){
    nowIndex++;
    if (nowIndex >= suitsArr->count()) {
        nowIndex = 0;
    }
    
    _ManSpr->removeAllChildren();
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    myClothesTemp = (CCDictionary*)suitsArr->objectAtIndex(nowIndex);
    
    CCString* labelStr2 = CCString::createWithFormat("%d/%d", nowIndex+1, suitsArr->count());
    label2->setString(labelStr2->getCString());
    
    this->creat_Man();
    this->initClothes();
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&GashaponLayer::updataClothes2));
    _ManSpr->runAction(CCSequence::create(CCDelayTime::create(.3f), callFunc, NULL));
}
void GashaponLayer::updataClothes2(){
    
    
}

void GashaponLayer::creat_Man(){
    float widthFolt = .63f;
    float heightFloat = .61f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setFlipX(flipxBool);
    _touSpr->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height * heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void GashaponLayer::initClothes(){//穿衣服
    float widthFolt = .63f;
    float heightFloat = .61f;
    float scaleFloat = .63f;
    bool flipxBool = false;
    int sub_part = 0;
    
    for (int i = Tag_QJ_TouFa; i <= Tag_QJ_ZhuangRong; i++) {
        if (i == Tag_QJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                _tfSpr1->setTag(Tag_QJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _tfSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _tfSpr1->setTag(Tag_QJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _tfSpr2->setTag(Tag_QJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _tfSpr3->setTag(Tag_QJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _wtSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wtSpr1->setTag(Tag_QJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wtSpr2->setTag(Tag_QJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wtSpr3->setTag(Tag_QJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
                _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _sySpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _sySpr1->setTag(Tag_QJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _sySpr2->setTag(Tag_QJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _sySpr3->setTag(Tag_QJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                    _kzSpr1->setTag(Tag_QJ_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _kzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _kzSpr1->setTag(Tag_QJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _kzSpr2->setTag(Tag_QJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _kzSpr3->setTag(Tag_QJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _wzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wzSpr1->setTag(Tag_QJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wzSpr2->setTag(Tag_QJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _wzSpr3->setTag(Tag_QJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _xzSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _xzSpr1->setTag(Tag_QJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _xzSpr2->setTag(Tag_QJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _xzSpr3->setTag(Tag_QJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_QJ_ShiPin){
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
                                _spSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _bSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _bSpr1->setTag(Tag_QJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _bSpr2->setTag(Tag_QJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
                            _bSpr3->setTag(Tag_QJ_Bao3);
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
                _zrSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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
                            _zrSpr1->setPosition(ccp(bgSpr->getContentSize().width* widthFolt, bgSpr->getContentSize().height* heightFloat));
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





