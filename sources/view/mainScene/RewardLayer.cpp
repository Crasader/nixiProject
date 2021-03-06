//
//  RewardLayer.cpp
//  tiegao
//
//  Created by mac on 16-8-12.
//
//

#include "RewardLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "ClothesScene.h"
#include "AppUtil.h"
#include "JNIController.h"
#include "Loading2.h"
#include "NetManager.h"


RewardLayer::RewardLayer(){
    
}
RewardLayer::~RewardLayer(){
    
}

void RewardLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&RewardLayer::_321CallBack), "HTTP_FINISHED_321", NULL);
}
void RewardLayer::onExit(){
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    unscheduleAllSelectors();
    
    setTouchEnabled(false);
    CCLayer::onExit();
}

bool RewardLayer::init(){
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(250, 250, 250, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        touchBool = false;
        tenIndex = 0;
        tenBool = false;
        rewardBool = false;
        iconBool = false;
        
        bRet = true;
    } while (0);
    
    return bRet;
}

RewardLayer* RewardLayer::create_with_index(CCArray* arr){
    RewardLayer* rtn = RewardLayer::create();
    rtn->init_with_index(arr);
    
    return rtn;
}
void RewardLayer::init_with_index(CCArray* arr){
    rewardArr = arr;
    rewardArr->retain();
    
    bgSpr = CCSprite::create("res/pic/mask.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr, 5);
    
//    for (int i = 0; i < arr->count(); i++) {
//        CCString* str = (CCString* )arr->objectAtIndex(i);
//        CCLog("%s", str->getCString());
//    }
    if (rewardArr->count() == 1) {
        this->drawOnce();
    }else{// 十连抽
        kuangSpr = CCSprite::create("res/pic/gashapon/gashapon_kuang2.png");
        kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        kuangSpr->setTag(0x99999);
        this->addChild(kuangSpr, 100);
        
        renyiSpr = CCSprite::create("res/pic/game/color/guide_kuang2.png");
        renyiSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, -20));
        kuangSpr->addChild(renyiSpr, 3);
        renyiSpr->setVisible(false);

        CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title2.png");
        titleSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.02f));
        kuangSpr->addChild(titleSpr, 100);
//        CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title.png");
//        titleSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .99f));
//        kuangSpr->addChild(titleSpr, 100);
        
        CCSprite* guangSpr = CCSprite::create("res/pic/gashapon/gashapon_1.png");
        guangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
        CCRotateBy* action = CCRotateBy::create(10.f, 360);
        guangSpr->runAction(CCRepeatForever::create(action));
        this->addChild(guangSpr, 4);
        
        CCSprite* dianSpr = CCSprite::create("res/pic/gashapon/gashapon_2.png");
        dianSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.25f));
        kuangSpr->addChild(dianSpr, 60);
        
        this->schedule(SEL_SCHEDULE(&RewardLayer::drawTen), .3f);
    }
}
void RewardLayer::openTouch(){
    touchBool = true;
}


void RewardLayer::drawOnce(){
    kuangSpr = CCSprite::create("res/pic/gashapon/gashapon_kuang.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    kuangSpr->setTag(0x99999);
    this->addChild(kuangSpr, 100);
    
//    CCString* str = (CCString* )arr->objectAtIndex(0);
//    std::string indexStr = str->getCString();
//    int index = atoi(indexStr.c_str());
    
    CCDictionary* dic = (CCDictionary*)rewardArr->objectAtIndex(0);
    const CCString* type = dic->valueForKey("type");
    int index = ((CCInteger* )dic->objectForKey("num"))->getValue();
    CCString* str = CCString::createWithFormat("%d", index);
    
    CCString* iconStr;
    int z_oder = 0;
    int clothesType = 0;
    if (index >= 10000) {
        std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
        if (stdStr.empty()) {
            CCString* saveStr = CCString::createWithFormat("%d;", index);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }else{
            CCString* saveStr = CCString::createWithFormat("%d;", index);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }
        CCUserDefault::sharedUserDefault()->flush();
        
        z_oder = 1;
        if (index >= 10000 && index < 20000){
            clothesType = Tag_CL_TouFa;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%s.png", str->getCString());
        }else if (index >= 20000 && index < 30000){
            clothesType = Tag_CL_WaiTao;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%s.png", str->getCString());
        }else if (index >= 30000 && index < 40000){
            clothesType = Tag_CL_ShangYi;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%s.png", str->getCString());
        }else if (index >= 40000 && index < 50000){
            clothesType = Tag_CL_KuZi;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%s.png", str->getCString());
        }else if (index >= 50000 && index < 60000){
            clothesType = Tag_CL_WaZi;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%s.png", str->getCString());
        }else if (index >= 60000 && index < 70000){
            clothesType = Tag_CL_XieZi;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%s.png", str->getCString());
        }else if (index >= 70000 && index < 80000){
            clothesType = Tag_CL_ShiPin;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%s.png", str->getCString());
        }else if (index >= 80000 && index < 90000){
            clothesType = Tag_CL_Bao;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%s.png", str->getCString());
        }else if (index >= 90000 && index < 100000){
            clothesType = Tag_CL_ZhuangRong;
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%s.png", str->getCString());
        }
        
    }else{
        z_oder = 0;
//        iconStr = CCString::createWithFormat("res/pic/gashapon/gashapon_suipian.png");
        iconStr = this->iconWithRewardType(type);
    }
    CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
    iconSpr->setScale(.2f);
    iconSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .96f));
    kuangSpr->addChild(iconSpr, 70 + z_oder);
    
    if (index < 10000) {
//        CCString* labelStr = CCString::createWithFormat("碎片 x%s", str->getCString());
        CCString* labelStr = this->descriptionWithRewardTypeOnce(type, index);
        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        label->setPosition(ccp(kuangSpr->getContentSize().width* .5f, iconSpr->getContentSize().height* .18f));
        label->setColor(ccWHITE);
        kuangSpr->addChild(label);
    }else{
        CCString* labelStr = CCString::createWithFormat("哇,新衣服耶.");
        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        label->setPosition(ccp(kuangSpr->getContentSize().width* .5f, iconSpr->getContentSize().height* .1f));
        label->setColor(ccWHITE);
        kuangSpr->addChild(label);
        
        
        int phase = 0;
        CCDictionary* allClothesDic = CONFIG->clothes();// 所有衣服
        CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(clothesType);// 获得当前类型所有衣服
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int id = clothDic->valueForKey("id")->intValue();
            if (id == index) {
                phase = clothDic->valueForKey("phase")->intValue();
                break;
            }
        }
        
        if (phase == 5) {
        
            CCSprite* shareSpr1;
            CCSprite* shareSpr2;
            if (DATA->getNews()->dailyShareCount == 0) {
                shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
                shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
                shareSpr2->setScale(1.02f);
            }else{
                shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
                shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
                shareSpr2->setScale(1.02f);
            }
            CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(RewardLayer::shareCallBack));
            shareItem->setAnchorPoint(ccp(0, .5f));
            shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
            CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
            shareMenu->setPosition(CCPointZero);
            shareMenu->setTag(0x334455);
            this->addChild(shareMenu, 120);
        }
    }
    
    CCMoveTo* moveTo = CCMoveTo::create(.6f, ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
    CCScaleTo* scaleTo = CCScaleTo::create(.6f, 1.f);
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
    CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
    iconSpr->runAction(CCSequence::create(spawn, callFunc, NULL));
    
    
    CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title2.png");
    titleSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .99f));
    kuangSpr->addChild(titleSpr, 100);
//    CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title.png");
//    titleSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .99f));
//    kuangSpr->addChild(titleSpr, 100);
    
    CCSprite* guangSpr = CCSprite::create("res/pic/gashapon/gashapon_1.png");
    guangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
    CCRotateBy* action = CCRotateBy::create(10.f, 360);
    guangSpr->runAction(CCRepeatForever::create(action));
    this->addChild(guangSpr, 4);
    
    CCSprite* dianSpr = CCSprite::create("res/pic/gashapon/gashapon_2.png");
    dianSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.25f));
    kuangSpr->addChild(dianSpr, 60);
}
void RewardLayer::shareCallBack(CCObject* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("/share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    rt->saveToFile(path.c_str());
    
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCRenderTexture* rt = AppUtil::saveScreenAsRenderTexture();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path.append("share.png");
    
    CCLog("图片 === %s", path.c_str());
    
    JNIController::setShareImage(path.c_str());
    rt->saveToFile(path.c_str());
    
    JNIController::showShare(3, 0);
    
    this->schedule(SEL_SCHEDULE(&RewardLayer::shareStatus), .1f);
#endif
}
void RewardLayer::shareStatus(float dt){
    if (JNIController::getShareStatus() == 1) {
        JNIController::shareText();
        JNIController::setShareStatus(0);
        
        LOADING->show_loading();
        NET->daily_share_321();
        
        this->unschedule(SEL_SCHEDULE(&RewardLayer::shareStatus));
    }else if (JNIController::getShareStatus() == 2 || JNIController::getShareStatus() == 3){
        JNIController::shareText();
        JNIController::setShareStatus(0);
        this->unschedule(SEL_SCHEDULE(&RewardLayer::shareStatus));
    }
}
void RewardLayer::_321CallBack(CCObject* pSender){
    LOADING->remove();
    
    if (this->getChildByTag(0x334455) != NULL) {
        this->removeChildByTag(0x334455);
    }
    CCSprite* shareSpr1;
    CCSprite* shareSpr2;
    if (DATA->getNews()->dailyShareCount == 0) {
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share1.png");
        shareSpr2->setScale(1.02f);
    }else{
        shareSpr1 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2 = CCSprite::create("res/pic/haoyoupaihang/share2.png");
        shareSpr2->setScale(1.02f);
    }
    CCMenuItem* shareItem = CCMenuItemSprite::create(shareSpr1, shareSpr2, this, menu_selector(RewardLayer::shareCallBack));
    shareItem->setAnchorPoint(ccp(0, .5f));
    shareItem->setPosition(ccp(5, DISPLAY->ScreenHeight()* .75f));
    CCMenu* shareMenu = CCMenu::create(shareItem, NULL);
    shareMenu->setPosition(CCPointZero);
    shareMenu->setTag(0x334455);
    this->addChild(shareMenu, 20);
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
}

CCString* RewardLayer::iconWithRewardType(const CCString* type) {
    CCString* rtn = NULL;
    if (type->compare("piece") == 0) {
        rtn = ccs("pic/gashapon/gashapon_suipian.png");
    }
    else if (type->compare("coin") == 0) {
        rtn = ccs("pic/common/coin2.png");
    }
    else if (type->compare("diam") == 0) {
        rtn = ccs("pic/common/diam2.png");
    }
    
    return rtn;
}

CCString* RewardLayer::descriptionWithRewardTypeOnce(const CCString* type, int num) {
    CCString* rtn = NULL;
    if (type->compare("piece") == 0) {
        rtn = CCString::createWithFormat("水晶碎片x%d", num);
    }
    else if (type->compare("coin") == 0) {
        rtn = CCString::createWithFormat("金币x%d", num);
    }
    else if (type->compare("diam") == 0) {
        rtn = CCString::createWithFormat("钻石x%d", num);
    }
    
    return rtn;
}

CCString* RewardLayer::descriptionWithRewardType(const CCString* type) {
    CCString* rtn = NULL;
    if (type->compare("piece") == 0) {
        rtn = ccs("水晶碎片");
    }
    else if (type->compare("coin") == 0) {
        rtn = ccs("金币");
    }
    else if (type->compare("diam") == 0) {
        rtn = ccs("钻石");
    }
    
    return rtn;
}


void RewardLayer::drawTen(){
    if (tenIndex == rewardArr->count() - 1) {
        this->unschedule(SEL_SCHEDULE(&RewardLayer::drawTen));
    }
    
    CCDictionary* dic = (CCDictionary*)rewardArr->objectAtIndex(tenIndex);
    const CCString* type = dic->valueForKey("type");
    
    int index = ((CCInteger* )dic->objectForKey("num"))->getValue();
    CCString* str = CCString::createWithFormat("%d", index);
    
    CCString* iconStr;
    const CCString* nameStr;

//    if (i == 3) {
//        index = 31201;
//        str = NULL;
//        str = CCString::createWithFormat("%d", index);
//    }
    if (index >= 10000) {
        this->unschedule(SEL_SCHEDULE(&RewardLayer::drawTen));
        
        std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
        if (stdStr.empty()) {
            CCString* saveStr = CCString::createWithFormat("%d;", index);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }else{
            CCString* saveStr = CCString::createWithFormat("%d;", index);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }
        CCUserDefault::sharedUserDefault()->flush();
        
        
        
        
        iconBool = true;
        rewardBool = true;
        CCDictionary* dic = CONFIG->clothes();// 所有衣服
        CCArray* clothesArr;

        if (index >= 10000 && index < 20000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_TouFa);// 获得当前类型所有衣服
        }else if (index >= 20000 && index < 30000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_WaiTao);// 获得当前类型所有衣服
        }else if (index >= 30000 && index < 40000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_ShangYi);// 获得当前类型所有衣服
        }else if (index >= 40000 && index < 50000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_KuZi);// 获得当前类型所有衣服
        }else if (index >= 50000 && index < 60000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_WaZi);// 获得当前类型所有衣服
        }else if (index >= 60000 && index < 70000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_XieZi);// 获得当前类型所有衣服
        }else if (index >= 70000 && index < 80000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_ShiPin);// 获得当前类型所有衣服
        }else if (index >= 80000 && index < 90000){
            iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%s.png", str->getCString());
            clothesArr = (CCArray* )dic->objectForKey(Tag_CL_Bao);// 获得当前类型所有衣服
        }
        for (int i = 0; i < clothesArr->count(); i++) {
            CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(i);
            int id = clothDic->valueForKey("id")->intValue();
            if (id == index) {
                nameStr = clothDic->valueForKey("name");
                break;
            }
        }
    }else{
        iconBool = false;
        
        iconStr = this->iconWithRewardType(type);
        nameStr = this->descriptionWithRewardType(type);
    }
    CCSprite* dikuangSpr = CCSprite::create("res/pic/gashapon/gashapon_dikuang.png");
    dikuangSpr->setScale(.2f);
    dikuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .97f));
    dikuangSpr->setTag(tenIndex);
    kuangSpr->addChild(dikuangSpr, 70 + tenIndex);
    
    CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
    CCLabelTTF* nameLabel;
    if (iconBool) {
        iconSpr->setScale(.4f);
        iconSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .55f));
        
        nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 14, CCSizeMake(kuangSpr->getContentSize().width* .9f, 14), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
        
        renyiSpr->setVisible(true);
    }else {
        iconSpr->setScale(.7f);
        iconSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .55f, dikuangSpr->getContentSize().height* .55f));
        nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 14, CCSizeMake(dikuangSpr->getContentSize().width* .9f, 14), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    }
    dikuangSpr->addChild(iconSpr, 70);
    nameLabel->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .1f));
    nameLabel->setColor(ccc3(154, 114, 146));
    dikuangSpr->addChild(nameLabel);
    
    if (index < 10000) {
        CCString* labelStr = CCString::createWithFormat("x%s", str->getCString());
//        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(iconSpr->getContentSize().width* .5f, 18), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 18);
        label->setAnchorPoint(ccp(0.5, 0.5));
        label->setPosition(ccp(dikuangSpr->getContentSize().width* .84f, dikuangSpr->getContentSize().height* .9f));
        label->setColor(ccc3(181, 143, 145));
        dikuangSpr->addChild(label);
    }
    if (tenIndex < 5) {
        if (iconBool) {
            rewardBool = true;
            tenBool = true;
            
            CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
            CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
            CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 2.5f);
            CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
            CCSequence* seq = CCSequence::create(spawn1, CCDelayTime::create(.2f), callFunc, NULL);
            dikuangSpr->runAction(seq);
        }else{
            tenBool = false;
            
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * tenIndex), kuangSpr->getContentSize().height* .63f));
            CCScaleTo* scaleTo = CCScaleTo::create(.5f, 1.f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
            dikuangSpr->runAction(CCSequence::create(CCDelayTime::create(0.2f), spawn, NULL));
            tenIndex++;
        }
    }else {
        if (iconBool) {
            rewardBool = true;
            tenBool = true;
            this->unschedule(SEL_SCHEDULE(&RewardLayer::drawTen));
            
            CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
            CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 2.5f);
            CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
            CCSequence* seq;
            CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
            if (tenIndex == rewardArr->count() - 1) {
                seq = CCSequence::create(spawn1, CCDelayTime::create(.2f), callFunc, NULL);
            }else{
                seq = CCSequence::create(spawn1, CCDelayTime::create(.2f), callFunc, NULL);
            }
            dikuangSpr->runAction(seq);
        }else{
            tenBool = false;
            
            CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * (tenIndex - 5)), kuangSpr->getContentSize().height* .28f));
            CCScaleTo* scaleTo = CCScaleTo::create(.5f, 1.f);
            CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
            if (tenIndex == rewardArr->count() - 1) {
                CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::drawTen3));
                dikuangSpr->runAction(CCSequence::create(CCDelayTime::create(0.2f), spawn, callFunc, NULL));
            }else{
                dikuangSpr->runAction(CCSequence::create(CCDelayTime::create(0.2f), spawn, NULL));
                tenIndex++;
            }
            
        }
    }
}
void RewardLayer::drawTen2(){
    CCSprite* dikuangSpr;
    if (kuangSpr->getChildByTag(tenIndex) != NULL) {
        dikuangSpr = (CCSprite* )kuangSpr->getChildByTag(tenIndex);
    }
    
    if (tenIndex < 5) {
        if (iconBool) {
            CCCallFunc* callFunc2 = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openDraw));
            CCMoveTo* moveTo2 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * tenIndex), kuangSpr->getContentSize().height* .63f));
            CCScaleTo* scaleTo2 = CCScaleTo::create(.5f, 1.f);
            CCSpawn* spawn2 = CCSpawn::create(moveTo2, scaleTo2, NULL);
            CCSequence* seq = CCSequence::create(CCDelayTime::create(.5f), spawn2, callFunc2, NULL);
            tenIndex++;
            dikuangSpr->runAction(seq);
        }
    }else {
        if (iconBool) {
            CCCallFunc* callFunc2 = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openDraw));
            CCMoveTo* moveTo2 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * (tenIndex - 5)), kuangSpr->getContentSize().height* .28f));
            CCScaleTo* scaleTo2 = CCScaleTo::create(.5f, 1.f);
            CCSpawn* spawn2 = CCSpawn::create(moveTo2, scaleTo2, NULL);
            CCSequence* seq;
            if (tenIndex == rewardArr->count() - 1) {
                CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
                seq = CCSequence::create(CCDelayTime::create(.5f), spawn2, callFunc, NULL);
            }else{
                tenIndex++;
                seq = CCSequence::create(CCDelayTime::create(.5f), spawn2, callFunc2, NULL);
            }
            dikuangSpr->runAction(seq);
        }
    }
}
void RewardLayer::drawTen3(){
//    if (!rewardBool) {
//        CCSprite* reward = CCSprite::create("res/pic/gashapon/gashapon_reward.png");
//        reward->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.1f));
//        reward->setScale(.1f);
//        kuangSpr->addChild(reward, 80);
//        
//        CCSprite* rewardGuangSpr = CCSprite::create("res/pic/gashapon/gashapon_3.png");
//        rewardGuangSpr->setPosition(ccp(reward->getContentSize().width* .5f, reward->getContentSize().height* .42f));
//        CCRotateBy* action = CCRotateBy::create(4.f, 360);
//        rewardGuangSpr->runAction(CCRepeatForever::create(action));
//        reward->addChild(rewardGuangSpr);
//        
//        CCSprite* dikuangSpr = CCSprite::create("res/pic/gashapon/gashapon_dikuang.png");
//        dikuangSpr->setPosition(ccp(reward->getContentSize().width* .5f, reward->getContentSize().height* .39f));
//        reward->addChild(dikuangSpr, 5);
//        
//        CCString* iconStr = CCString::createWithFormat("res/pic/gashapon/gashapon_suipian.png");
//        CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
//        iconSpr->setScale(.7f);
//        iconSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .55f, dikuangSpr->getContentSize().height* .55f));
//        dikuangSpr->addChild(iconSpr, 70);
//        
//        CCString* nameStr = CCString::createWithFormat("水晶碎片");
//        CCLabelTTF* nameLabel = CCLabelTTF::create(nameStr->getCString(), DISPLAY->fangzhengFont(), 14, CCSizeMake(dikuangSpr->getContentSize().width* .9f, 14), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
//        nameLabel->setPosition(ccp(dikuangSpr->getContentSize().width* .5f, dikuangSpr->getContentSize().height* .1f));
//        nameLabel->setColor(ccc3(154, 114, 146));
//        dikuangSpr->addChild(nameLabel);
//        
//        CCString* labelStr = CCString::createWithFormat("x%d", 30);
//        CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(iconSpr->getContentSize().width* .5f, 18), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
//        label->setPosition(ccp(dikuangSpr->getContentSize().width* .83f, dikuangSpr->getContentSize().height* .9f));
//        label->setColor(ccc3(181, 143, 145));
//        dikuangSpr->addChild(label);
//        
//        CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .2f));
//        CCMoveTo* moveTo2 = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* .5f, - kuangSpr->getContentSize().height* .4f));
//        CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 2.f);
//        CCScaleTo* scaleTo2 = CCScaleTo::create(.5f, 1.f);
//        CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
//        CCSpawn* spawn2 = CCSpawn::create(moveTo2, scaleTo2, NULL);
//        CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
//        
//        CCSequence* seq = CCSequence::create(CCDelayTime::create(1.6f), spawn1, CCDelayTime::create(.6f), spawn2, callFunc, NULL);
//        reward->runAction(seq);
//    }else{
//        this->openTouch();
//    }
    
    this->openTouch();
}
void RewardLayer::openDraw(){
    this->schedule(SEL_SCHEDULE(&RewardLayer::drawTen), .3f);
}

bool RewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (touchBool) {
        if (tenBool) {
            tenBool = false;
            touchBool = false;
            renyiSpr->setVisible(false);
            this->drawTen2();
        }else{
            CCPoint location = pTouch->getLocation();
            if (! kuangSpr->boundingBox().containsPoint(location)) {
                CCNotificationCenter::sharedNotificationCenter()->postNotification("openUpdataClothes");
                this->removeFromParentAndCleanup(true);
            }
        }
    }
    
    return true;
}








