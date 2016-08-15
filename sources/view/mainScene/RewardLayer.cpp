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


RewardLayer::RewardLayer(){
    
}
RewardLayer::~RewardLayer(){
    
}

void RewardLayer::onEnter(){
    CCLayer::onEnter();
    
}
void RewardLayer::onExit(){
    
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
    bgSpr = CCSprite::create("res/pic/gashapon/gashapon_bg2.png");
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    for (int i = 0; i < arr->count(); i++) {
        CCString* str = (CCString* )arr->objectAtIndex(i);
        CCLog("%s", str->getCString());
    }
    if (arr->count() == 1) {
        kuangSpr = CCSprite::create("res/pic/gashapon/gashapon_kuang.png");
        kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        kuangSpr->setTag(0x99999);
        this->addChild(kuangSpr, 100);
        
        CCString* str = (CCString* )arr->objectAtIndex(0);
        std::string indexStr = str->getCString();
        int index = atoi(indexStr.c_str());
        CCString* iconStr;
        if (index >= 10000) {
            if (index >= 10000 && index < 20000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%s.png", str->getCString());
            }else if (index >= 20000 && index < 30000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%s.png", str->getCString());
            }else if (index >= 30000 && index < 40000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%s.png", str->getCString());
            }else if (index >= 40000 && index < 50000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%s.png", str->getCString());
            }else if (index >= 50000 && index < 60000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%s.png", str->getCString());
            }else if (index >= 60000 && index < 70000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%s.png", str->getCString());
            }else if (index >= 70000 && index < 80000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%s.png", str->getCString());
            }else if (index >= 80000 && index < 90000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%s.png", str->getCString());
            }else if (index >= 90000 && index < 100000){
                iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%s.png", str->getCString());
            }
            
        }else{
            iconStr = CCString::createWithFormat("res/pic/gashapon/gashapon_suipian.png");
        }
        CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
        iconSpr->setScale(.2f);
        iconSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .96f));
        kuangSpr->addChild(iconSpr, 70);
        
        if (index < 10000) {
            CCString* labelStr = CCString::createWithFormat("碎片 x%s", str->getCString());
            CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(kuangSpr->getContentSize().width* .5f, iconSpr->getContentSize().height* .18f));
            label->setColor(ccWHITE);
            kuangSpr->addChild(label);
        }else{
            
            CCString* labelStr = CCString::createWithFormat("哇,新衣服耶.");
            CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(kuangSpr->getContentSize().width* .8f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
            label->setPosition(ccp(kuangSpr->getContentSize().width* .5f, iconSpr->getContentSize().height* .15f));
            label->setColor(ccWHITE);
            kuangSpr->addChild(label);
        }
        
        CCMoveTo* moveTo = CCMoveTo::create(.6f, ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .5f));
        CCScaleTo* scaleTo = CCScaleTo::create(.6f, 1.f);
        CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
        CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
        iconSpr->runAction(CCSequence::create(spawn, callFunc, NULL));
    }else{// 十连抽
        kuangSpr = CCSprite::create("res/pic/gashapon/gashapon_kuang2.png");
        kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        kuangSpr->setTag(0x99999);
        this->addChild(kuangSpr, 100);
        
        for (int i = 0; i < arr->count(); i++) {
            CCString* str = (CCString* )arr->objectAtIndex(i);
            std::string indexStr = str->getCString();
            int index = atoi(indexStr.c_str());
            CCString* iconStr;
            if (i == 3) {
                index = 21051;
                str = NULL;
                str = CCString::createWithFormat("%d", index);
            }
            
            if (index >= 10000) {
                if (index >= 10000 && index < 20000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/1toufa/icon%s.png", str->getCString());
                }else if (index >= 20000 && index < 30000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/2waitao/icon%s.png", str->getCString());
                }else if (index >= 30000 && index < 40000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/3shangyi/icon%s.png", str->getCString());
                }else if (index >= 40000 && index < 50000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/4kuzi/icon%s.png", str->getCString());
                }else if (index >= 50000 && index < 60000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/5wazi/icon%s.png", str->getCString());
                }else if (index >= 60000 && index < 70000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/6xiezi/icon%s.png", str->getCString());
                }else if (index >= 70000 && index < 80000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/7shipin/icon%s.png", str->getCString());
                }else if (index >= 80000 && index < 90000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/8bao/icon%s.png", str->getCString());
                }else if (index >= 90000 && index < 100000){
                    iconStr = CCString::createWithFormat("res/pic/clothesScene/icon/9zhuangrong/icon%s.png", str->getCString());
                }
            }else{
                iconStr = CCString::createWithFormat("res/pic/gashapon/gashapon_suipian.png");
            }
            CCSprite* dikuangSpr = CCSprite::create("res/pic/gashapon/gashapon_dikuang.png");
            dikuangSpr->setScale(.2f);
            dikuangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .96f));
            kuangSpr->addChild(dikuangSpr, 70);
            
            CCSprite* iconSpr = CCSprite::create(iconStr->getCString());
            iconSpr->setScale(.7f);
            iconSpr->setPosition(ccp(dikuangSpr->getContentSize().width* .55f, dikuangSpr->getContentSize().height* .55f));
            dikuangSpr->addChild(iconSpr, 70);
            if (index < 10000) {
                CCString* labelStr = CCString::createWithFormat("x%s", str->getCString());
                CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), DISPLAY->fangzhengFont(), 18, CCSizeMake(iconSpr->getContentSize().width* .5f, 18), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
                label->setPosition(ccp(dikuangSpr->getContentSize().width* .9f, dikuangSpr->getContentSize().height* .9f));
                label->setColor(ccc3(181, 143, 145));
                dikuangSpr->addChild(label);
            }
            if (i < 5) {
                CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * i), kuangSpr->getContentSize().height* .63f));
                CCScaleTo* scaleTo = CCScaleTo::create(.5f, 1.f);
                CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
                dikuangSpr->runAction(CCSequence::create(CCDelayTime::create(0.2f*i), spawn, NULL));
            }else{
                CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(kuangSpr->getContentSize().width* (.12f + .19f * (i-5)), kuangSpr->getContentSize().height* .28f));
                CCScaleTo* scaleTo = CCScaleTo::create(.5f, 1.f);
                CCSpawn* spawn = CCSpawn::create(moveTo, scaleTo, NULL);
                CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&RewardLayer::openTouch));
                dikuangSpr->runAction(CCSequence::create(CCDelayTime::create(0.2f*i), spawn, callFunc, NULL));
            }
        }
    }
    
    CCSprite* titleSpr = CCSprite::create("res/pic/gashapon/gashapon_title.png");
    titleSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .99f));
    kuangSpr->addChild(titleSpr, 100);
    
    CCSprite* guangSpr = CCSprite::create("res/pic/gashapon/gashapon_1.png");
    guangSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.35f));
    kuangSpr->addChild(guangSpr, 50);
    CCRotateTo* rotateTo1 = CCRotateTo::create(.5f, -7);
    CCRotateTo* rotateTo2 = CCRotateTo::create(.5f, 0);
    CCRotateTo* rotateTo3 = CCRotateTo::create(.5f, 7);
    CCRotateTo* rotateTo4 = CCRotateTo::create(.5f, 0);
    CCSequence* seq = CCSequence::create(rotateTo1, rotateTo2, rotateTo3, rotateTo4, NULL);
    guangSpr->runAction(CCRepeatForever::create(seq));
    
    CCSprite* dianSpr = CCSprite::create("res/pic/gashapon/gashapon_2.png");
    dianSpr->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* 1.25f));
    kuangSpr->addChild(dianSpr, 60);
}
void RewardLayer::openTouch(){
    touchBool = true;
}

bool RewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    if (touchBool) {
        CCPoint location = pTouch->getLocation();
        if (! kuangSpr->boundingBox().containsPoint(location)) {
            this->removeFromParentAndCleanup(true);
        }
    }
    
    return true;
}







