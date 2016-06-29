//
//  StorySettlementOfTheAnimationLayer.cpp
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#include "StorySettlementOfTheAnimationLayer.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "QingjingScene.h"


StorySettlementOfTheAnimationLayer::StorySettlementOfTheAnimationLayer(){
    
}
StorySettlementOfTheAnimationLayer::~StorySettlementOfTheAnimationLayer(){
    
}

void StorySettlementOfTheAnimationLayer::onEnter(){
    CCLayer::onEnter();
    
}
void StorySettlementOfTheAnimationLayer::onExit(){
    
    setTouchEnabled(false);
    CCLayer::onExit();
}

bool StorySettlementOfTheAnimationLayer::init(){
    bool bRet = false;
    
    do {
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(250, 250, 250, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(false);
        
        this->scheduleOnce(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::openTouch), 1.f);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

StorySettlementOfTheAnimationLayer* StorySettlementOfTheAnimationLayer::create_with_index(int index){
    StorySettlementOfTheAnimationLayer* rtn = StorySettlementOfTheAnimationLayer::create();
    rtn->init_with_index(index);
    
    return rtn;
}
void StorySettlementOfTheAnimationLayer::init_with_index(int index){
    
    if (index == 1) {
        CCLabelTTF* wanchengLabel1 = CCLabelTTF::create("完成动画", DISPLAY->fangzhengFont(), 50.f);
        wanchengLabel1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
        wanchengLabel1->setColor(ccRED);
        this->addChild(wanchengLabel1, 5);
        
        CCLabelTTF* wanchengLabel2 = CCLabelTTF::create("完成动画", DISPLAY->fangzhengFont(), 50.f);
        wanchengLabel2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 1, DISPLAY->ScreenHeight()* .7f - 1));
        wanchengLabel2->setColor(ccRED);
        this->addChild(wanchengLabel2, 4);
        
    }else if (index == 2){
        CCLabelTTF* wanchengLabel1 = CCLabelTTF::create("结局动画", DISPLAY->fangzhengFont(), 50.f);
        wanchengLabel1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .7f));
        wanchengLabel1->setColor(ccRED);
        this->addChild(wanchengLabel1, 5);
        
        CCLabelTTF* wanchengLabel2 = CCLabelTTF::create("结局动画", DISPLAY->fangzhengFont(), 50.f);
        wanchengLabel2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 1, DISPLAY->ScreenHeight()* .7f - 1));
        wanchengLabel2->setColor(ccRED);
        this->addChild(wanchengLabel2, 4);
    }
}

bool StorySettlementOfTheAnimationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
    
    this->removeFromParentAndCleanup(true);
    CCScene* scene = QingjingScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
    
    return true;
}

void StorySettlementOfTheAnimationLayer::openTouch(float dt){
    this->setTouchEnabled(true);
}








