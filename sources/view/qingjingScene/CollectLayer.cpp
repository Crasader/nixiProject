//
//  CollectLayer.cpp
//  tiegao
//
//  Created by mac on 16-11-11.
//
//

#include "CollectLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "QingjingScene.h"
#include "AudioManager.h"
#include "CollectTableView.h"


CollectLayer::CollectLayer(){
    
}
CollectLayer::~CollectLayer(){
    
}

bool CollectLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    num_child = 0;
    
    creat_view();
    
    return true;
}
CCScene* CollectLayer::scene(){
    CCScene* scene = CCScene::create();
    CollectLayer* layer = CollectLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void CollectLayer::onEnter(){
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, menu_selector(CollectLayer::bgMethods), "CollectBgMethods", NULL);
    
    CCLayer::onEnter();
    
    this->scheduleOnce(SEL_SCHEDULE(&CollectLayer::keyBackStatus), .8f);
}
void CollectLayer::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}

void CollectLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void CollectLayer::keyBackClicked(){
    num_child++;
    CCLog("===== CollectLayer  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}
void CollectLayer::backCallBack(CCObject* pSender){
    AUDIO->goback_effect();
    
    num_child = 0;
    CCScene* scene = QingjingScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void CollectLayer::creat_view(){
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/common/btn_goback2.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/common/btn_goback2.png");
    backSpr2->setScale(1.02f);
    CCMenuItem* backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(CollectLayer::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    
    CCArray* collectArr = CCArray::create();
    for (int i = 0; i < 14; i++) {
        CCInteger* integer;
        if (i == 0) {
            if (CCUserDefault::sharedUserDefault()->getBoolForKey("101", false)) {
                integer = CCInteger::create(101);
                collectArr->addObject(integer);
            }
        }else{
            CCString* str = CCString::createWithFormat("%d", 1000 + (i + 1));
            if (CCUserDefault::sharedUserDefault()->getBoolForKey(str->getCString(), false)) {
                integer = CCInteger::create(1000 + (i + 1));
                collectArr->addObject(integer);
            }
        }
    }
    CCInteger* integer = CCInteger::create(2000);
    collectArr->addObject(integer);
    DATA->setCollectSource(collectArr);
    
    CCInteger* bgIndex = (CCInteger* )collectArr->objectAtIndex(0);
    CCString* bgStr = CCString::createWithFormat("res/pic/qingjingScene/collect/%d.png", bgIndex->getValue());
    bgSpr = CCSprite::create(bgStr->getCString());
    bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bgSpr);
    
    
    CollectTableView* tabLayer = CollectTableView::create();
    tabLayer->setPosition(ccp(DISPLAY->ScreenWidth()* .2f, 0));
    tabLayer->setTag(0x77777);
    this->addChild(tabLayer, 5);
}

void CollectLayer::bgMethods(CCObject* obj){
    
    CCArray* arr = DATA->getCollectSource();
    CCInteger* bgIndex = (CCInteger* )arr->objectAtIndex(DATA->getCollectIndex());
    CCString* bgFileName = CCString::createWithFormat("res/pic/qingjingScene/collect/%d.png", bgIndex->getValue());
    CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(bgFileName->getCString());
    CCRect rect = CCRectZero;
    rect.size = aTexture->getContentSize();
    bgSpr->setTexture(aTexture);
    bgSpr->setTextureRect(rect);
}



