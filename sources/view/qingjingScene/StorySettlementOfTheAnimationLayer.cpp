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
#include "VipQingjingScene.h"
#include "AppUtil.h"


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
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), DISPLAY->ScreenWidth(), DISPLAY->ScreenHeight()));
        
        theEndBool = false;
        tishiBool = false;
        
        this->setTouchSwallowEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);
        
        this->scheduleOnce(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::openTouch), 1.f);
        
        bRet = true;
    } while (0);
    
    return bRet;
}

StorySettlementOfTheAnimationLayer* StorySettlementOfTheAnimationLayer::create_with_index(int index, std::string ending, int goldIndex){
    StorySettlementOfTheAnimationLayer* rtn = StorySettlementOfTheAnimationLayer::create();
    rtn->init_with_index(index, ending, goldIndex);
    
    return rtn;
}
void StorySettlementOfTheAnimationLayer::init_with_index(int index, std::string ending, int goldIndex){
    
//    CCSprite* mask = CCSprite::create("res/pic/mask.png");
//    mask->setPosition(DISPLAY->center());
//    this->addChild(mask, 10);
    _goldIndex = goldIndex;
    
    if (index == 1) {
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/animation/qj_wan.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/animation/qj_cheng.png");
        CCSprite* spr3 = CCSprite::create("res/pic/qingjingScene/animation/qj_zhang.png");
        CCSprite* spr4 = CCSprite::create("res/pic/qingjingScene/animation/qj_jie.png");
        spr1->setScale(1.6f);
        spr1->setPosition(ccp(DISPLAY->ScreenWidth()* .41f, DISPLAY->ScreenHeight()* .8f));
        spr1->setVisible(false);
        this->addChild(spr1, 100);
        
        spr2->setScale(1.6f);
        spr2->setPosition(ccp(DISPLAY->ScreenWidth()* .47f, DISPLAY->ScreenHeight()* .8f));
        spr2->setVisible(false);
        this->addChild(spr2, 100);
        
        spr3->setScale(1.6f);
        spr3->setPosition(ccp(DISPLAY->ScreenWidth()* .53f, DISPLAY->ScreenHeight()* .8f));
        spr3->setVisible(false);
        this->addChild(spr3, 100);
        
        spr4->setScale(1.6f);
        spr4->setPosition(ccp(DISPLAY->ScreenWidth()* .59f, DISPLAY->ScreenHeight()* .8f));
        spr4->setVisible(false);
        this->addChild(spr4, 100);
        
        float sprFloat1 = (.05 + CCRANDOM_MINUS1_1()*.1f);
        float sprFloat2 = (.05 + CCRANDOM_MINUS1_1()*.1f);
        float sprFloat3 = (.05 + CCRANDOM_MINUS1_1()*.1f);
        float sprFloat4 = (.05 + CCRANDOM_MINUS1_1()*.1f);
        CCMoveTo* moveTo1_1 = CCMoveTo::create(.25f + sprFloat1, ccp(DISPLAY->ScreenWidth()* .26f, DISPLAY->ScreenHeight()* .65f));
        CCMoveTo* moveTo2_1 = CCMoveTo::create(.25f + sprFloat2, ccp(DISPLAY->ScreenWidth()* .42f, DISPLAY->ScreenHeight()* .67f));
        CCMoveTo* moveTo3_1 = CCMoveTo::create(.25f + sprFloat3, ccp(DISPLAY->ScreenWidth()* .58f, DISPLAY->ScreenHeight()* .67f));
        CCMoveTo* moveTo4_1 = CCMoveTo::create(.25f + sprFloat4, ccp(DISPLAY->ScreenWidth()* .74f, DISPLAY->ScreenHeight()* .65f));
        
        CCScaleTo* scaleTo1 = CCScaleTo::create(.25f + sprFloat1, 1.1f);
        CCScaleTo* scaleTo2 = CCScaleTo::create(.25f + sprFloat2, 1.1f);
        CCScaleTo* scaleTo3 = CCScaleTo::create(.25f + sprFloat3, 1.1f);
        CCScaleTo* scaleTo4 = CCScaleTo::create(.25f + sprFloat4, 1.1f);
        
        CCSpawn* spawn1 = CCSpawn::create(CCShow::create(), moveTo1_1, scaleTo1, NULL);
        CCSpawn* spawn2 = CCSpawn::create(CCShow::create(), moveTo2_1, scaleTo2, NULL);
        CCSpawn* spawn3 = CCSpawn::create(CCShow::create(), moveTo3_1, scaleTo3, NULL);
        CCSpawn* spawn4 = CCSpawn::create(CCShow::create(), moveTo4_1, scaleTo4, NULL);
        
        CCMoveTo* moveTo1_2 = CCMoveTo::create(.63f, ccp(DISPLAY->ScreenWidth()* 1.3f, DISPLAY->ScreenHeight()* .65f));
        CCMoveTo* moveTo2_2 = CCMoveTo::create(.61, ccp(DISPLAY->ScreenWidth()* 1.3f, DISPLAY->ScreenHeight()* .67f));
        CCMoveTo* moveTo3_2 = CCMoveTo::create(.59f, ccp(DISPLAY->ScreenWidth()* 1.3f, DISPLAY->ScreenHeight()* .67f));
        CCMoveTo* moveTo4_2 = CCMoveTo::create(.57, ccp(DISPLAY->ScreenWidth()* 1.3f, DISPLAY->ScreenHeight()* .65f));
        
        float sprFloat11 = 1.f - (.25f + sprFloat1);
        float sprFloat22 = 1.f - (.25f + sprFloat2);
        float sprFloat33 = 1.f - (.25f + sprFloat3);
        float sprFloat44 = 1.f - (.25f + sprFloat4);
        CCCallFunc* callFunc;
        CCSequence* seq1;
        if (DATA->getTheEndBool()) {// 有通关的章节，不显示第一次通关奖励
            theEndBool = false;
            callFunc = CCCallFunc::create(this, SEL_CallFunc(&StorySettlementOfTheAnimationLayer::closeCallBack));
            seq1 = CCSequence::create(spawn1, CCDelayTime::create(sprFloat11 + 1.f), moveTo1_2, CCDelayTime::create(.5f), callFunc, NULL);
        }else{// 没有通关的章节，显示第一次通关奖励
            if (_goldIndex != 0) {// 显示
                theEndBool = true;
                callFunc = CCCallFunc::create(this, SEL_CallFunc(&StorySettlementOfTheAnimationLayer::tishiCallBack));
                seq1 = CCSequence::create(spawn1, CCDelayTime::create(sprFloat11 + 1.f), moveTo1_2, CCDelayTime::create(.3f), callFunc, NULL);
            }else{// 不显示
                theEndBool = false;
                callFunc = CCCallFunc::create(this, SEL_CallFunc(&StorySettlementOfTheAnimationLayer::closeCallBack));
                seq1 = CCSequence::create(spawn1, CCDelayTime::create(sprFloat11 + 1.f), moveTo1_2, CCDelayTime::create(.5f), callFunc, NULL);
            }
        }
        
        CCSequence* seq2 = CCSequence::create(spawn2, CCDelayTime::create(sprFloat22 + 1.f), moveTo2_2, NULL);
        CCSequence* seq3 = CCSequence::create(spawn3, CCDelayTime::create(sprFloat33 + 1.f), moveTo3_2, NULL);
        CCSequence* seq4 = CCSequence::create(spawn4, CCDelayTime::create(sprFloat44 + 1.f), moveTo4_2, NULL);
        
        spr1->runAction(seq1);
        spr2->runAction(seq2);
        spr3->runAction(seq3);
        spr4->runAction(seq4);
        
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar1), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar2), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar3), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar4), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar5), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar6), .5f + CCRANDOM_MINUS1_1()*.2f);
        
        
    }else if (index == 2){
        
        CSJson::Value storyData = AppUtil::read_json_file("story/storyAchievement");
        CCDictionary* storyDic = AppUtil::dictionary_with_json(storyData);
        CCString* endingStr = (CCString* )storyDic->objectForKey(ending.c_str());
        
//        CCSprite* jjKuangSpr = CCSprite::create("res/pic/qingjingScene/animation/qj_jiejukuang.png");
//        jjKuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
//        this->addChild(jjKuangSpr, 100);
        CCSprite* jjKuangSpr = CCSprite::create("res/pic/qingjingScene/animation/qj_jiejukuang2.png");
        jjKuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .65f));
        this->addChild(jjKuangSpr, 100);
        
        
        CCLabelTTF* wanchengLabel = CCLabelTTF::create(endingStr->getCString(), DISPLAY->fangzhengFont(), 40);
        wanchengLabel->setPosition(ccp(jjKuangSpr->getContentSize().width* .5f, jjKuangSpr->getContentSize().height* .26f));
        wanchengLabel->setColor(ccc3(80, 63, 68));
        jjKuangSpr->addChild(wanchengLabel, 5);
        
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar1), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar2), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar3), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar4), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar5), .5f + CCRANDOM_MINUS1_1()*.2f);
        this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar6), .5f + CCRANDOM_MINUS1_1()*.2f);
    }
}
void StorySettlementOfTheAnimationLayer::closeCallBack(CCObject* pSender){
    this->removeFromParentAndCleanup(true);
    
    CCScene* scene = QingjingScene::scene();
    CCTransitionFade* trans = CCTransitionFade::create(0.6, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}
void StorySettlementOfTheAnimationLayer::tishiCallBack(CCObject* pSender){
    CCSprite* _panel = CCSprite::create("pic/building/reward/panel.png");
    _panel->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.75));
    _panel->setTag(0x22222);
    this->addChild(_panel, 100);
    
    CCLabelTTF* label1 = CCLabelTTF::create("首次通关奖励", DISPLAY->fangzhengFont(), 25);
    label1->setPosition(ccp(_panel->getContentSize().width* .5f, _panel->getContentSize().height* .71f));
    label1->setColor(ccc3(80, 63, 68));
    _panel->addChild(label1);
    
    CCLabelTTF* label2 = CCLabelTTF::create("点击任意处关闭", DISPLAY->fangzhengFont(), 25);
    label2->setPosition(ccp(_panel->getContentSize().width* .75f, 17));
    label2->setColor(ccWHITE);
    _panel->addChild(label2);
    
    CCSize panelSize = _panel->boundingBox().size;
    
    CCSprite* light = CCSprite::create("pic/building/reward/light.png");
    light->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(light);
    
    CCSprite* spots = CCSprite::create("pic/building/reward/spots.png");
    spots->setPosition(ccp(panelSize.width* .5f, panelSize.height * 1.15));
    _panel->addChild(spots);
    
    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.6), CCDelayTime::create(0.3), CCFadeOut::create(0.6), CCDelayTime::create(0.5), NULL);
    spots->runAction(CCRepeatForever::create(seq));
    
    CCSprite* title = CCSprite::create("pic/building/reward/title.png");
    title->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
    _panel->addChild(title);
    
    CCSprite* plate = CCSprite::create("pic/building/reward/plate.png");
    plate->setPosition(ccp(panelSize.width* .5f, panelSize.height * .45f));
    _panel->addChild(plate);
    
    CCSprite* diam = CCSprite::create("pic/common/diam2.png");
    diam->setPosition(ccp(panelSize.width* .5f, panelSize.height * .45f));
    _panel->addChild(diam);
    
    CCString* str = CCString::createWithFormat("%d 钻石", _goldIndex);
    CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 24);
    lbl->setColor(ccc3(107, 143, 190));
    lbl->setPosition(ccp(panelSize.width* .5f, panelSize.height * .25f));
    _panel->addChild(lbl);
    
    _panel->setScale(0.1);
    _panel->setVisible(true);
    
    float duration1 = 0.2f;
    CCMoveTo* moveto1 = CCMoveTo::create(duration1, ccp(DISPLAY->halfW(), DISPLAY->H() * 0.45));
    CCScaleTo* scaleto1 = CCScaleTo::create(duration1, 1.1);
    CCSpawn* spawn1 = CCSpawn::create(moveto1, scaleto1, NULL);
    
    float duration2 = 0.2f;
    CCMoveTo* moveto2 = CCMoveTo::create(duration2, DISPLAY->center());
    CCScaleTo* scaleto2 = CCScaleTo::create(duration2, 1);
    CCSpawn* spawn2 = CCSpawn::create(moveto2, scaleto2, NULL);
    
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&StorySettlementOfTheAnimationLayer::upTishiBool));
    
    _panel->runAction(CCEaseSineIn::create(CCSequence::create(spawn1, spawn2, callFunc, NULL)));
}

void StorySettlementOfTheAnimationLayer::upTishiBool(float dt){
    tishiBool = true;
}

bool StorySettlementOfTheAnimationLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (theEndBool) {
        if (tishiBool) {
            tishiBool = false;
            theEndBool = false;
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
            
            this->closeCallBack(NULL);
        }
    }else{
        CCNotificationCenter::sharedNotificationCenter()->postNotification("UpdataMoney");
        
        this->closeCallBack(NULL);
    }
    
    return true;
}

void StorySettlementOfTheAnimationLayer::openTouch(float dt){
    this->setTouchEnabled(true);
}


void StorySettlementOfTheAnimationLayer::creatStar1(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar1));
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua1.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    
    
    CCSprite* xianSpr = CCSprite::create("res/pic/qingjingScene/animation/qj_xian11.png");
    this->addChild(xianSpr, 96);
    CCSequence* seq = CCSequence::create(CCPlace::create(ccp(0, DISPLAY->ScreenHeight() * (.6f + CCRANDOM_MINUS1_1()*.1f))), CCMoveBy::create(.6f + CCRANDOM_MINUS1_1()*.2f, ccp(DISPLAY->ScreenWidth()* 1.4f, 0)), NULL);
    xianSpr->runAction(seq);
    
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar1), 1.f + CCRANDOM_MINUS1_1()*.2f);
}
void StorySettlementOfTheAnimationLayer::creatStar2(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar2));
    
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua2.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    
    
    CCSprite* xianSpr2 = CCSprite::create("res/pic/qingjingScene/animation/qj_xian22.png");
    this->addChild(xianSpr2, 94);
    CCSequence* seq2 = CCSequence::create(CCPlace::create(ccp(0, DISPLAY->ScreenHeight() * (.6f + CCRANDOM_MINUS1_1()*.1f))), CCMoveBy::create(.6f + CCRANDOM_MINUS1_1()*.2f, ccp(DISPLAY->ScreenWidth()* 1.4f, 0)), NULL);
    xianSpr2->runAction(seq2);
    
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar2), 1.f + CCRANDOM_MINUS1_1()*.2f);
}
void StorySettlementOfTheAnimationLayer::creatStar3(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar3));
    
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua3.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    
    CCSprite* xianSpr3 = CCSprite::create("res/pic/qingjingScene/animation/qj_xian33.png");
    this->addChild(xianSpr3, 96);
    CCSequence* seq3 = CCSequence::create(CCPlace::create(ccp(0, DISPLAY->ScreenHeight() * (.6f + CCRANDOM_MINUS1_1()*.1f))), CCMoveBy::create(.6f + CCRANDOM_MINUS1_1()*.2f, ccp(DISPLAY->ScreenWidth()* 1.4f, 0)), NULL);
    xianSpr3->runAction(seq3);
    
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar3), 1.f + CCRANDOM_MINUS1_1()*.2f);
}
void StorySettlementOfTheAnimationLayer::creatStar4(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar4));
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua4.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar4), 1.f + CCRANDOM_MINUS1_1()*.2f);
}
void StorySettlementOfTheAnimationLayer::creatStar5(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar5));
    
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua5.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar5), 1.f + CCRANDOM_MINUS1_1()*.2f);
}
void StorySettlementOfTheAnimationLayer::creatStar6(float dt){
    this->unschedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar6));
    
    CCSprite* spr = CCSprite::create("res/pic/qingjingScene/animation/qj_hua6.png");
    spr->setScale(.25f);
    spr->setPosition(ccp(-200, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    spr->setScale(.25f + CCRANDOM_MINUS1_1()*.05f);
    this->addChild(spr, 90);
    
    CCMoveTo* moveTo = CCMoveTo::create(2.f + CCRANDOM_MINUS1_1()*.4f, ccp(DISPLAY->ScreenWidth() + 50, DISPLAY->ScreenHeight()* (.65f + CCRANDOM_MINUS1_1()*.1f)));
    CCRotateTo* rotateTo = CCRotateTo::create(2.f, 360*5);
    spr->runAction(CCSpawn::create(moveTo, rotateTo, NULL));
    
    this->schedule(SEL_SCHEDULE(&StorySettlementOfTheAnimationLayer::creatStar6), 1.f + CCRANDOM_MINUS1_1()*.2f);
}




