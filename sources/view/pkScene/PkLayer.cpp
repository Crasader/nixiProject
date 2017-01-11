//
//  PkLayer.cpp
//  tiegao
//
//  Created by mac on 17-1-9.
//
//

#include "PkLayer.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "MainScene.h"
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"



PkLayer::PkLayer(){
    
}
PkLayer::~PkLayer(){
    
}

bool PkLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    num_child = 0;
    selfScore = 0;
    opponentScore = 0;
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    themeInfo = DATA->getCompetition()->getTheme();
    selfItem = DATA->getCompetition()->getSelf();
    opponentItem = DATA->getCompetition()->getOpponent();
    
    this->creatAnimation();
    
    this->creat_view();
    this->creat_nameKuang();
    this->creat_zhufuKuang();
    // 对决
    this->creatDuijue();
    
    
    this->creat_Man1();
    this->creat_Man2();
    
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatScoreAnimation1), .5f);
    
    return true;
}
void PkLayer::creatAnimation(){
    CCArray* arrowAnimations = CCArray::createWithCapacity(10);
    char arrowStr[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr, "res/pic/pk/pk_quan%d.png", i);
        CCSpriteFrame* arrowFrame = CCSpriteFrame::create(arrowStr, CCRectMake(0, 0, 356, 356));
        arrowAnimations->addObject(arrowFrame);
    }
    CCAnimation* arrowAnimation = CCAnimation::createWithSpriteFrames(arrowAnimations, .4f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation, "quanStr");
}
CCScene* PkLayer::scene(){
    CCScene* scene = CCScene::create();
    PkLayer* layer = PkLayer::create();
    scene->addChild(layer);
    
    return scene;
}

void PkLayer::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    
//    nc->addObserver(this, SEL_CallFuncO(&pkScene::_823Callback), "HTTP_FINISHED_823", NULL);
    
    
//    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::keyBackStatus), .8f);
}
void PkLayer::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}
void PkLayer::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCLayer::onExit();
}
void PkLayer::keyBackClicked(){
    num_child++;
    CCLog("===== pkScene  children_num: %d", num_child);
    if (num_child> 1) {
        num_child = 0;
        return;
    }
    
    if (DATA->current_guide_step() == 0) {
        this->backCallBack(NULL);
    }
}

void PkLayer::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
    
}


void PkLayer::creat_view(){
    
    CCSprite* huangguanSpr = CCSprite::create("res/pic/pk/pk_huangguan4.png");
    huangguanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(huangguanSpr, 5);
    
    CCString* selfScoreStr = CCString::createWithFormat("%d", selfScore);
    scoreLabel1 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfScoreStr->getCString(), 0, .01f);
    scoreLabel1->setAnchorPoint(ccp(.5f, .5f));
    scoreLabel1->set_delegate(this); // 设置代理
    scoreLabel1->setPosition(ccp(huangguanSpr->getContentSize().width* .28f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel1, 1);
    
    CCString* opponentScoreStr = CCString::createWithFormat("%d", opponentScore);
    scoreLabel2 = FlashNumberLabel2::create_with_atlas2("res/pic/pk/pk_number1.png", opponentScoreStr->getCString(), 0, .01f);
    scoreLabel2->setAnchorPoint(ccp(.5f, .5f));
    scoreLabel2->set_delegate2(this); // 设置代理
    scoreLabel2->setPosition(ccp(huangguanSpr->getContentSize().width* .72f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel2, 1);
    
}
void PkLayer::creat_nameKuang(){
    CCSprite* nameKuangSpr1 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr1->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr1, 5);
    
    CCLabelTTF* nameLabel1 = CCLabelTTF::create("mingzimdms", DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .7f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel1->setPosition(ccp(nameKuangSpr1->getContentSize().width* .56f, nameKuangSpr1->getContentSize().height* .5f));
    nameLabel1->setColor(ccc3(191, 71, 99));
    nameKuangSpr1->addChild(nameLabel1);
    
    
    CCSprite* nameKuangSpr2 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr2->setFlipX(true);
    nameKuangSpr2->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .78f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr2, 5);
    
    CCLabelTTF* nameLabel2 = CCLabelTTF::create("mingzimdms", DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .7f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(nameKuangSpr2->getContentSize().width* .45f, nameKuangSpr2->getContentSize().height* .5f));
    nameLabel2->setColor(ccc3(191, 71, 99));
    nameKuangSpr2->addChild(nameLabel2);
}
void PkLayer::creat_zhufuKuang(){
    CCSprite* zhufuKuangSpr1 = CCSprite::create("res/pic/pk/pk_kuang6.png");
    zhufuKuangSpr1->setAnchorPoint(ccp(1.f, 0.f));
    zhufuKuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 3, 2.f));
    this->addChild(zhufuKuangSpr1, 5);
    
    CCLabelTTF* zhufuLabel1 = CCLabelTTF::create("魅力增加50%", DISPLAY->fangzhengFont(), 25, CCSizeMake(zhufuKuangSpr1->getContentSize().width* .9f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    zhufuLabel1->setPosition(ccp(zhufuKuangSpr1->getContentSize().width* .5f, zhufuKuangSpr1->getContentSize().height* .46f));
    zhufuLabel1->setColor(ccc3(231, 161, 127));
    zhufuKuangSpr1->addChild(zhufuLabel1);
    
    
    CCSprite* zhufuKuangSpr2 = CCSprite::create("res/pic/pk/pk_kuang6.png");
    zhufuKuangSpr2->setFlipX(true);
    zhufuKuangSpr2->setAnchorPoint(ccp(0.f, 0.f));
    zhufuKuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 3, 2.f));
    this->addChild(zhufuKuangSpr2, 5);
    
    CCLabelTTF* zhufuLabel2 = CCLabelTTF::create("魅力增加20%", DISPLAY->fangzhengFont(), 25, CCSizeMake(zhufuKuangSpr2->getContentSize().width* .9f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    zhufuLabel2->setPosition(ccp(zhufuKuangSpr2->getContentSize().width* .5f, zhufuKuangSpr2->getContentSize().height* .46f));
    zhufuLabel2->setColor(ccc3(231, 161, 127));
    zhufuKuangSpr2->addChild(zhufuLabel2);
}
void PkLayer::creatDuijue(){
    jiesuanSpr1 = CCSprite::create("res/pic/pk/pk_duijue1.png");
    jiesuanSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanSpr1, 15);
    jiesuanGuangSpr1 = CCSprite::create("res/pic/pk/pk_guang2.png");
    jiesuanGuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanGuangSpr1, 16);
    CCRotateBy* jiesuanGuangAction = CCRotateBy::create(6.f, 360);
    jiesuanGuangSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(jiesuanGuangAction)));
    
    jiesuanQuanSpr = CCSprite::create("res/pic/pk/pk_quan1.png");
    jiesuanQuanSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanQuanSpr, 17);
    CCAnimation* quanAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* quanAnimate = CCAnimate::create(quanAnimation);
    jiesuanQuanSpr->runAction(CCRepeatForever::create(quanAnimate));
    
    jiesuanSpr2 = CCSprite::create("res/pic/pk/jiesuan/pk_jiesuan.png");
    jiesuanSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .65f));
    this->addChild(jiesuanSpr2, 18);
    
//    jiesuanGuangSpr2 = CCSprite::create("res/pic/pk/pk_guang3.png");
//    jiesuanGuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .65f));
//    this->addChild(jiesuanGuangSpr2, 19);
//    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.1f);
//    CCFadeOut* fadeOut1 = CCFadeOut::create(.2f);
//    CCScaleTo* scaleTo2 = CCScaleTo::create(.1f, 1.f);
//    CCFadeIn* fadeOut2 = CCFadeIn::create(.1f);
//    CCSpawn* jiesuanGuangSpawn = CCSpawn::create(scaleTo2, fadeOut2, NULL);
//    CCSequence* jiesuanGuangSeq = CCSequence::create(scaleTo1, CCDelayTime::create(.3f), fadeOut1, jiesuanGuangSpawn, NULL);
//    jiesuanGuangSpr2->runAction(CCRepeatForever::create(jiesuanGuangSeq));
}



void PkLayer::creatScoreAnimation1(){
    
    this->creatScoreKuang(1);
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatScoreAnimation2), 2.f);
}
void PkLayer::creatScoreAnimation2(){
    
    this->creatScoreKuang(2);
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatScoreAnimation3), 2.f);
}
void PkLayer::creatScoreAnimation3(){
    
    this->creatScoreKuang(3);
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatScoreAnimation4), 2.f);
}
void PkLayer::creatScoreAnimation4(){
    
    this->creatScoreKuang(4);
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatJiesuan), 2.f);
}
void PkLayer::creatScoreKuang(int type){
    if (this->getChildByTag(0x33333) != NULL) {
        CCNode* node = this->getChildByTag(0x33333);
        node->removeAllChildren();
        this->removeChildByTag(0x33333);
    }
    if (this->getChildByTag(0x44444) != NULL) {
        CCNode* node = this->getChildByTag(0x44444);
        node->removeAllChildren();
        this->removeChildByTag(0x44444);
    }
    
    CCSprite* dapeiSpr1_1;
    CCSprite* dapeiSpr1_2;
    CCSprite* dapeiSpr2_1;
    CCSprite* dapeiSpr2_2;
    bool actionBool = false;
    if (type == 1) {// 搭配
        selfIndex1 = 999;
        opponentIndex1 = 888;
        tempSelfScore = 999;
        tempOpponentScore = 888;
        if (tempSelfScore > tempOpponentScore) {
            actionBool = true;
        }else{
            actionBool = false;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_dapei2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_dapei1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_dapei2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_dapei1.png");
    }else if (type == 2){// 魅力
        selfIndex2 = 333;
        opponentIndex2 = 555;
        tempSelfScore = 333;
        tempOpponentScore = 555;
        if (tempSelfScore > tempOpponentScore) {
            actionBool = true;
        }else{
            actionBool = false;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_meili2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_meili1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_meili2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_meili1.png");
    }else if (type == 3){// 人气
        selfIndex3 = 222;
        opponentIndex3 = 115;
        tempSelfScore = 222;
        tempOpponentScore = 115;
        if (tempSelfScore > tempOpponentScore) {
            actionBool = true;
        }else{
            actionBool = false;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_renqi2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_renqi1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_renqi2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_renqi1.png");
    }else if (type == 4){// 幸运
        selfIndex4 = 123;
        opponentIndex4 = 122;
        tempSelfScore = 123;
        tempOpponentScore = 122;
        if (tempSelfScore > tempOpponentScore) {
            actionBool = true;
        }else{
            actionBool = false;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_xingyun2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_xingyun1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_xingyun2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_xingyun1.png");
    }
    // self
    dapeiSpr1_1->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, -300));
    dapeiSpr1_1->setTag(0x33333);
    this->addChild(dapeiSpr1_1, 50);
    dapeiSpr1_2->setPosition(ccp(dapeiSpr1_1->getContentSize().width* .5f, dapeiSpr1_1->getContentSize().height* .5f));
    dapeiSpr1_1->addChild(dapeiSpr1_2);
    CCRotateBy* dapeiAction1 = CCRotateBy::create(10.f, 360);
    dapeiSpr1_2->runAction(CCRepeatForever::create(CCRepeatForever::create(dapeiAction1)));
    CCString* label1 = CCString::createWithFormat(":%d", tempSelfScore);
    CCLabelAtlas* dapeiLabel1 = CCLabelAtlas::create(label1->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
    dapeiLabel1->setAnchorPoint(ccp(.5f, .5f));
    dapeiLabel1->setPosition(ccp(dapeiSpr1_1->getContentSize().width* .5f, dapeiSpr1_1->getContentSize().height* .35f));
    dapeiSpr1_1->addChild(dapeiLabel1);
    
    // opponent
    dapeiSpr2_1->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, -300));
    dapeiSpr2_1->setTag(0x44444);
    this->addChild(dapeiSpr2_1, 50);
    dapeiSpr2_2->setPosition(ccp(dapeiSpr2_1->getContentSize().width* .5f, dapeiSpr2_1->getContentSize().height* .5f));
    dapeiSpr2_1->addChild(dapeiSpr2_2);
    CCRotateBy* dapeiAction2 = CCRotateBy::create(10.f, 360);
    dapeiSpr2_2->runAction(CCRepeatForever::create(CCRepeatForever::create(dapeiAction2)));
    CCString* label2 = CCString::createWithFormat(":%d", tempOpponentScore);
    CCLabelAtlas* dapeiLabel2 = CCLabelAtlas::create(label2->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
    dapeiLabel2->setAnchorPoint(ccp(.5f, .5f));
    dapeiLabel2->setPosition(ccp(dapeiSpr2_1->getContentSize().width* .5f, dapeiSpr2_1->getContentSize().height* .35f));
    dapeiSpr2_1->addChild(dapeiLabel2);
    
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .35f + 15.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .35f));
    CCMoveTo* moveTo1_3 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .35f + 15.f));
    CCMoveTo* moveTo1_4 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .35f));
    CCSequence* seq1_1 = CCSequence::create(moveTo1_1, moveTo1_2, moveTo1_3, moveTo1_4, NULL);
    
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .35f + 15.f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .35f));
    CCMoveTo* moveTo2_3 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .35f + 15.f));
    CCMoveTo* moveTo2_4 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .35f));
    CCSequence* seq2_1 = CCSequence::create(moveTo2_1, moveTo2_2, moveTo2_3, moveTo2_4, NULL);
    
    if (actionBool) {
        //
        CCMoveTo* moveTo1 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .75f));
        CCFadeOut* fadeOut1_1 = CCFadeOut::create(.3f);
        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(PkLayer::updateScore));
        CCSequence* seq1 = CCSequence::create(CCDelayTime::create(.5f), callFuncN, NULL);
        CCSequence* seq2 = CCSequence::create(CCDelayTime::create(.4f), fadeOut1_1, NULL);
        CCSpawn* spawn1 = CCSpawn::create(moveTo1, seq1, seq2, NULL);
        CCSequence* seq3 = CCSequence::create(seq1_1, spawn1, NULL);
        dapeiSpr1_1->runAction(seq3);
        
        CCFadeOut* fadeOut1_2 = CCFadeOut::create(.3f);
        CCSequence* seq4 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut1_2, NULL);
        dapeiSpr1_2->runAction(seq4);
        CCFadeOut* fadeOut1_3 = CCFadeOut::create(.3f);
        CCSequence* seq5 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut1_3, NULL);
        dapeiLabel1->runAction(seq5);
        
        //
        CCMoveTo* moveTo2 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .5f));
        CCFadeOut* fadeOut2 = CCFadeOut::create(.3f);
        CCSequence* seq6 = CCSequence::create(CCDelayTime::create(.4f), fadeOut2, NULL);
        CCSpawn* spawn2 = CCSpawn::create(moveTo2, seq6, NULL);
        CCSequence* seq7 = CCSequence::create(seq2_1, spawn2, NULL);
        dapeiSpr2_1->runAction(seq7);
        
        CCFadeOut* fadeOut2_2 = CCFadeOut::create(.3f);
        CCSequence* seq8 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut2_2, NULL);
        dapeiSpr2_2->runAction(seq8);
        CCFadeOut* fadeOut2_3 = CCFadeOut::create(.3f);
        CCSequence* seq9 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut2_3, NULL);
        dapeiLabel2->runAction(seq9);
    }else{
        //
        CCMoveTo* moveTo1 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .5f));
        CCFadeOut* fadeOut1 = CCFadeOut::create(.3f);
        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(PkLayer::updateScore));
        CCSequence* seq1 = CCSequence::create(CCDelayTime::create(.5f), callFuncN, NULL);
        CCSequence* seq2 = CCSequence::create(CCDelayTime::create(.4f), fadeOut1, NULL);
        CCSpawn* spawn1 = CCSpawn::create(moveTo1, seq1, seq2, NULL);
        CCSequence* seq3 = CCSequence::create(seq1_1, spawn1, NULL);
        dapeiSpr1_1->runAction(seq3);
        
        CCFadeOut* fadeOut1_2 = CCFadeOut::create(.3f);
        CCSequence* seq4 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut1_2, NULL);
        dapeiSpr1_2->runAction(seq4);
        CCFadeOut* fadeOut1_3 = CCFadeOut::create(.3f);
        CCSequence* seq5 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut1_3, NULL);
        dapeiLabel1->runAction(seq5);
        
        //
        CCMoveTo* moveTo2 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .75f));
        CCFadeOut* fadeOut2 = CCFadeOut::create(.3f);
        CCSequence* seq6 = CCSequence::create(CCDelayTime::create(.4f), fadeOut2, NULL);
        CCSpawn* spawn2 = CCSpawn::create(moveTo2, seq6, NULL);
        CCSequence* seq7 = CCSequence::create(seq2_1, spawn2, NULL);
        dapeiSpr2_1->runAction(seq7);
        
        CCFadeOut* fadeOut2_2 = CCFadeOut::create(.3f);
        CCSequence* seq8 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut2_2, NULL);
        dapeiSpr2_2->runAction(seq8);
        CCFadeOut* fadeOut2_3 = CCFadeOut::create(.3f);
        CCSequence* seq9 = CCSequence::create(CCDelayTime::create(.9f), CCDelayTime::create(.4f), fadeOut2_3, NULL);
        dapeiLabel2->runAction(seq9);
    }
}
void PkLayer::updateScore(float dt){
    
    CCString* scoreStr1 = CCString::createWithFormat("%d", selfScore += tempSelfScore);
    scoreLabel1->set_new_number(scoreStr1->getCString());

    CCString* scoreStr2 = CCString::createWithFormat("%d", opponentScore += tempOpponentScore);
    scoreLabel2->set_new_number2(scoreStr2->getCString());
}

void PkLayer::creatJiesuan(){
    CCMoveTo* moveTo1 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
    jiesuanSpr1->runAction(moveTo1);
    
    CCMoveTo* moveTo2 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
    jiesuanGuangSpr1->runAction(moveTo2);
    
    CCMoveTo* moveTo3 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
    jiesuanQuanSpr->runAction(moveTo3);
    
    CCMoveTo* moveTo4 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
    jiesuanSpr2->runAction(moveTo4);
    
//    CCMoveTo* moveTo5 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
//    jiesuanGuangSpr2->runAction(moveTo5);
    
    jiesuanKuangSpr = CCSprite::create("res/pic/pk/jiesuan/pk_jiesuankuang.png");
    jiesuanKuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .49f));
    jiesuanKuangSpr->setVisible(false);
    jiesuanKuangSpr->setScale(.2f);
    this->addChild(jiesuanKuangSpr, 14);
    this->jiesuanAnimation(jiesuanKuangSpr, NULL, 1);
    
    
    // self
    CCString* selfStr1 = CCString::createWithFormat("%d", 999);
//    CCLabelAtlas* selfLabel1 = CCLabelAtlas::create(selfStr1->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    selfLabel1->setVisible(false);
//    selfLabel1->setScale(.2f);
//    selfLabel1->setAnchorPoint(ccp(.5f, .5f));
//    selfLabel1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .3f, jiesuanKuangSpr->getContentSize().height* .765f));
//    jiesuanKuangSpr->addChild(selfLabel1);
    FlashNumberLabel* selfLabel1 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfStr1->getCString(), 0, .01f);
    selfLabel1->setAnchorPoint(ccp(.5f, .5f));
    selfLabel1->setVisible(false);
    selfLabel1->setScale(.2f);
    selfLabel1->set_delegate(NULL); // 设置代理
    selfLabel1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .765f));
    selfLabel1->setTag(0x111);
    jiesuanKuangSpr->addChild(selfLabel1, 1);
    this->jiesuanAnimation(NULL, selfLabel1, 2);
    
    CCString* selfStr2 = CCString::createWithFormat("%d", 333);
//    CCLabelAtlas* selfLabel2 = CCLabelAtlas::create(selfStr2->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    selfLabel2->setVisible(false);
//    selfLabel2->setScale(.2f);
//    selfLabel2->setAnchorPoint(ccp(.5f, .5f));
//    selfLabel2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .3f, jiesuanKuangSpr->getContentSize().height* .595f));
//    jiesuanKuangSpr->addChild(selfLabel2);
    FlashNumberLabel* selfLabel2 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfStr2->getCString(), 0, .01f);
    selfLabel2->setAnchorPoint(ccp(.5f, .5f));
    selfLabel2->setVisible(false);
    selfLabel2->setScale(.2f);
    selfLabel2->set_delegate(NULL); // 设置代理
    selfLabel2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .595f));
    selfLabel2->setTag(0x222);
    jiesuanKuangSpr->addChild(selfLabel2, 1);
    this->jiesuanAnimation(NULL, selfLabel2, 2);
    
    CCString* selfStr3 = CCString::createWithFormat("%d", 222);
//    CCLabelAtlas* selfLabel3 = CCLabelAtlas::create(selfStr3->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    selfLabel3->setVisible(false);
//    selfLabel3->setScale(.2f);
//    selfLabel3->setAnchorPoint(ccp(.5f, .5f));
//    selfLabel3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .3f, jiesuanKuangSpr->getContentSize().height* .43f));
//    jiesuanKuangSpr->addChild(selfLabel3);
    FlashNumberLabel* selfLabel3 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfStr3->getCString(), 0, .01f);
    selfLabel3->setAnchorPoint(ccp(.5f, .5f));
    selfLabel3->setVisible(false);
    selfLabel3->setScale(.2f);
    selfLabel3->set_delegate(NULL); // 设置代理
    selfLabel3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .43f));
    selfLabel3->setTag(0x333);
    jiesuanKuangSpr->addChild(selfLabel3, 1);
    this->jiesuanAnimation(NULL, selfLabel3, 2);
    
    CCString* selfStr4 = CCString::createWithFormat("%d", 123);
//    CCLabelAtlas* selfLabel4 = CCLabelAtlas::create(selfStr4->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    selfLabel4->setVisible(false);
//    selfLabel4->setScale(.2f);
//    selfLabel4->setAnchorPoint(ccp(.5f, .5f));
//    selfLabel4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .3f, jiesuanKuangSpr->getContentSize().height* .26f));
//    jiesuanKuangSpr->addChild(selfLabel4);
    FlashNumberLabel* selfLabel4 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", selfStr4->getCString(), 0, .01f);
    selfLabel4->setAnchorPoint(ccp(.5f, .5f));
    selfLabel4->setVisible(false);
    selfLabel4->setScale(.2f);
    selfLabel4->set_delegate(NULL); // 设置代理
    selfLabel4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .26f));
    selfLabel4->setTag(0x444);
    jiesuanKuangSpr->addChild(selfLabel4, 1);
    this->jiesuanAnimation(NULL, selfLabel4, 2);
    
    
    //opponent
    CCString* opponentStr1 = CCString::createWithFormat("%d", 888);
//    CCLabelAtlas* opponentLabel1 = CCLabelAtlas::create(opponentStr1->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    opponentLabel1->setVisible(false);
//    opponentLabel1->setScale(.2f);
//    opponentLabel1->setAnchorPoint(ccp(.5f, .5f));
//    opponentLabel1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .7f, jiesuanKuangSpr->getContentSize().height* .765f));
//    jiesuanKuangSpr->addChild(opponentLabel1);
    FlashNumberLabel* opponentLabel1 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", opponentStr1->getCString(), 0, .01f);
    opponentLabel1->setAnchorPoint(ccp(.5f, .5f));
    opponentLabel1->setVisible(false);
    opponentLabel1->setScale(.2f);
    opponentLabel1->set_delegate(NULL); // 设置代理
    opponentLabel1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .765f));
    opponentLabel1->setTag(0x555);
    jiesuanKuangSpr->addChild(opponentLabel1, 1);
    this->jiesuanAnimation(NULL, opponentLabel1, 2);
    
    CCString* opponentStr2 = CCString::createWithFormat("%d", 555);
//    CCLabelAtlas* opponentLabel2 = CCLabelAtlas::create(opponentStr2->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    opponentLabel2->setVisible(false);
//    opponentLabel2->setScale(.2f);
//    opponentLabel2->setAnchorPoint(ccp(.5f, .5f));
//    opponentLabel2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .7f, jiesuanKuangSpr->getContentSize().height* .595f));
//    jiesuanKuangSpr->addChild(opponentLabel2);
    FlashNumberLabel* opponentLabel2 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", opponentStr2->getCString(), 0, .01f);
    opponentLabel2->setAnchorPoint(ccp(.5f, .5f));
    opponentLabel2->setVisible(false);
    opponentLabel2->setScale(.2f);
    opponentLabel2->set_delegate(NULL); // 设置代理
    opponentLabel2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .595f));
    opponentLabel2->setTag(0x666);
    jiesuanKuangSpr->addChild(opponentLabel2, 1);
    this->jiesuanAnimation(NULL, opponentLabel2, 2);
    
    CCString* opponentStr3 = CCString::createWithFormat("%d", 115);
//    CCLabelAtlas* opponentLabel3 = CCLabelAtlas::create(opponentStr3->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    opponentLabel3->setVisible(false);
//    opponentLabel3->setScale(.2f);
//    opponentLabel3->setAnchorPoint(ccp(.5f, .5f));
//    opponentLabel3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .7f, jiesuanKuangSpr->getContentSize().height* .43f));
//    jiesuanKuangSpr->addChild(opponentLabel3);
    FlashNumberLabel* opponentLabel3 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", opponentStr3->getCString(), 0, .01f);
    opponentLabel3->setAnchorPoint(ccp(.5f, .5f));
    opponentLabel3->setVisible(false);
    opponentLabel3->setScale(.2f);
    opponentLabel3->set_delegate(NULL); // 设置代理
    opponentLabel3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .43f));
    opponentLabel3->setTag(0x777);
    jiesuanKuangSpr->addChild(opponentLabel3, 1);
    this->jiesuanAnimation(NULL, opponentLabel3, 2);
    
    CCString* opponentStr4 = CCString::createWithFormat("%d", 122);
//    CCLabelAtlas* opponentLabel4 = CCLabelAtlas::create(opponentStr4->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
//    opponentLabel4->setVisible(false);
//    opponentLabel4->setScale(.2f);
//    opponentLabel4->setAnchorPoint(ccp(.5f, .5f));
//    opponentLabel4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .7f, jiesuanKuangSpr->getContentSize().height* .26f));
//    jiesuanKuangSpr->addChild(opponentLabel4);
    FlashNumberLabel* opponentLabel4 = FlashNumberLabel::create_with_atlas("res/pic/pk/pk_number1.png", opponentStr4->getCString(), 0, .01f);
    opponentLabel4->setAnchorPoint(ccp(.5f, .5f));
    opponentLabel4->setVisible(false);
    opponentLabel4->setScale(.2f);
    opponentLabel4->set_delegate(NULL); // 设置代理
    opponentLabel4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .26f));
    opponentLabel4->setTag(0x888);
    jiesuanKuangSpr->addChild(opponentLabel4, 1);
    this->jiesuanAnimation(NULL, opponentLabel4, 2);
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatJiesuan2), .8f);
}
void PkLayer::jiesuanAnimation(CCSprite* spr, FlashNumberLabel* label, int type){
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.05f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(.3f, 1.f);
    CCSequence* seq1 = CCSequence::create(scaleTo1, scaleTo2, NULL);
    CCSpawn* spawn = CCSpawn::create(CCShow::create(), seq1, NULL);
    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(.5f), spawn, NULL);
    if (type == 1) {
        spr->runAction(seq2);
    }else if (type == 2){
        label->runAction(seq2);
    }
}
void PkLayer::creatJiesuan2(){
    int temSelfIndex1 = 5;
    int temSelfIndex2 = 10;
    int temOpponentIndex1 = 5;
    int temOpponentIndex2 = 10;
    
    if (temSelfIndex1 == 1) {
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x111);
        CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(selfIndex1 += selfIndex1*(temSelfIndex2* 0.01)));
        label1->set_new_number(scoreStr1->getCString());
        
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 1);
    }else if (temSelfIndex1 == 2){
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x222);
        CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(selfIndex2 += selfIndex2*(temSelfIndex2* 0.01)));
        label2->set_new_number(scoreStr2->getCString());
        
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 1);
    }else if (temSelfIndex1 == 3){
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x333);
        CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(selfIndex3 += selfIndex3*(temSelfIndex2* 0.01)));
        label3->set_new_number(scoreStr3->getCString());
        
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 1);
    }else if (temSelfIndex1 == 4){
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x444);
        CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(selfIndex4 += selfIndex4*(temSelfIndex2* 0.01)));
        label4->set_new_number(scoreStr4->getCString());
        
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 1);
    }else if (temSelfIndex1 == 5){
        //
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x111);
        CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(selfIndex1 += selfIndex1*(temSelfIndex2* 0.01)));
        label1->set_new_number(scoreStr1->getCString());
        
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 1);
        
        //
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x222);
        CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(selfIndex2 += selfIndex2*(temSelfIndex2* 0.01)));
        label2->set_new_number(scoreStr2->getCString());
        
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 1);
        
        //
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x333);
        CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(selfIndex3 += selfIndex3*(temSelfIndex2* 0.01)));
        label3->set_new_number(scoreStr3->getCString());
        
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 1);
        
        //
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x444);
        CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(selfIndex4 += selfIndex4*(temSelfIndex2* 0.01)));
        label4->set_new_number(scoreStr4->getCString());
        
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 1);
    }
    
    
    if (temOpponentIndex1 == 1) {
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x555);
        CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(opponentIndex1 += opponentIndex1*(temOpponentIndex2* 0.01)));
        label1->set_new_number(scoreStr1->getCString());
        
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 2);
    }else if (temOpponentIndex1 == 2){
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x666);
        CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(opponentIndex2 += opponentIndex2*(temOpponentIndex2* 0.01)));
        label2->set_new_number(scoreStr2->getCString());
        
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 2);
    }else if (temOpponentIndex1 == 3){
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x777);
        CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(opponentIndex3 += opponentIndex3*(temOpponentIndex2* 0.01)));
        label3->set_new_number(scoreStr3->getCString());
        
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 2);
    }else if (temOpponentIndex1 == 4){
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x888);
        CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(opponentIndex4 += opponentIndex4*(temOpponentIndex2* 0.01)));
        label4->set_new_number(scoreStr4->getCString());
        
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 2);
    }else if (temOpponentIndex1 == 5){
        //
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x555);
        CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(opponentIndex1 += opponentIndex1*(temOpponentIndex2* 0.01)));
        label1->set_new_number(scoreStr1->getCString());
        
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 2);
        
        //
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x666);
        CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(opponentIndex2 += opponentIndex2*(temOpponentIndex2* 0.01)));
        label2->set_new_number(scoreStr2->getCString());
        
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 2);
        
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x777);
        CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(opponentIndex3 += opponentIndex3*(temOpponentIndex2* 0.01)));
        label3->set_new_number(scoreStr3->getCString());
        
        //
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 2);
        
        //
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x888);
        CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(opponentIndex4 += opponentIndex4*(temOpponentIndex2* 0.01)));
        label4->set_new_number(scoreStr4->getCString());
        
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 2);
    }
    
}
void PkLayer::flashNumberAnimation(CCSprite* spr, int type){
    if (type == 1) {
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(spr->getPosition().x - 20, spr->getPosition().y));
        spr->runAction(moveTo);
    }else if (type == 2) {
        CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(spr->getPosition().x + 20, spr->getPosition().y));
        spr->runAction(moveTo);
    }
}



void PkLayer::creat_Man1(){
    float widthFolt = .4f;
    float heightFloat = .54f;
    float scaleFloat = 1.05f;
    bool flipxBool = false;
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban4.png");  //孔图
    holeStencil->setAnchorPoint(ccp(1.f, .5f));
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper);
    
    
    CCSprite* bgSpr1 = CCSprite::create("res/pic/house/house_1.png");
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr1, 2);
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/pk/pk_moban6.png");
    bgSpr2->setAnchorPoint(ccp(1.f, .5f));
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr2, 3);
    
    
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    
//    CCDictionary* clothesDic = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
//    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
}
void PkLayer::creat_Man2(){
    float widthFolt = .91f;
    float heightFloat = .54f;
    float scaleFloat = 1.05f;
    bool flipxBool = false;
    
    // 裁剪节点
    CCClippingNode *holesClipper = CCClippingNode::create();
    holesClipper->setInverted(false);
    holesClipper->setAlphaThreshold( 0.05f );
    holesClipper->setTag(0x88888);
    
    
    // 创建遮罩模板图片，并设置到裁剪节点
    CCSprite *holeStencil = CCSprite::create("res/pic/pk/pk_moban5.png");  //孔图
    holeStencil->setAnchorPoint(ccp(0, .5f));
    holeStencil->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->setStencil( holeStencil);
    // 裁剪节点加载到背景图
    this->addChild(holesClipper);
    
    
    int temIndex = rand()%100;
    CCString* bgStr;
    if (temIndex > 0 && temIndex <= 30) {
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 1);
    }else if (temIndex > 30 && temIndex <= 60){
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 2);
    }else if (temIndex > 60 && temIndex <= 90){
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 3);
    }else{
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 4);
    }
    CCSprite* bgSpr1 = CCSprite::create(bgStr->getCString());
    bgSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr1, 2);
    
    CCSprite* bgSpr2 = CCSprite::create("res/pic/pk/pk_moban6.png");
    bgSpr2->setAnchorPoint(ccp(0, .5f));
    bgSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    holesClipper->addChild(bgSpr2, 3);
    
    
    // 创建裁剪content图片，并加载到裁剪节点上
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setFlipX(flipxBool);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(manSpr, 200);
    
    CCSprite* touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    touSpr->setScale(scaleFloat);
    touSpr->setFlipX(flipxBool);
    touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat));
    holesClipper->addChild(touSpr, 210);
    
    
//    CCDictionary* clothesDic = opponentItem->getOndress(); // 男宠衣着
//    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
}

void PkLayer::initClothes(CCClippingNode* _ManSpr, float widthFolt, float heightFloat, float scaleFloat, bool flipxBool, CCDictionary* myClothesTemp){//穿衣服
    
    int sub_part = 0;
    
    for (int i = Tag_PkLayer_TouFa; i <= Tag_PkLayer_ZhuangRong; i++) {
        if (i == Tag_PkLayer_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                CCSprite* _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr1->setTag(Tag_PkLayer_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                _tfSpr2->setTag(Tag_PkLayer_TouFa2);
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
                            CCSprite* _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr1->setTag(Tag_PkLayer_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr2->setTag(Tag_PkLayer_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _tfSpr3->setTag(Tag_PkLayer_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                CCSprite* _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wtSpr1->setTag(Tag_PkLayer_WaiTao1);
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
                            CCSprite* _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr1->setTag(Tag_PkLayer_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr2->setTag(Tag_PkLayer_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wtSpr3->setTag(Tag_PkLayer_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 300000);
                CCSprite* _sySpr1 = CCSprite::create(str->getCString());
                _sySpr1->setScale(scaleFloat);
                _sySpr1->setFlipX(flipxBool);
                _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                _sySpr1->setTag(Tag_PkLayer_ShangYi1);
                _ManSpr->addChild(_sySpr1, 350);
                
//                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr1->setFlipX(true);
//                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
//                _ManSpr->addChild(xingSpr1, 420);
//
//                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
//                xingSpr2->setFlipX(true);
//                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
//                _ManSpr->addChild(xingSpr2, 420);
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
                            CCSprite* _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr1->setTag(Tag_PkLayer_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr2->setTag(Tag_PkLayer_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(widthFolt, heightFloat));
                            _sySpr3->setTag(Tag_PkLayer_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                if (sub_part == 1) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_PkLayer_KuZi1);
                    _ManSpr->addChild(_kzSpr1, 290);
                }else{
//                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 400000);
                    CCSprite* _kzSpr1 = CCSprite::create(str->getCString());
                    _kzSpr1->setScale(scaleFloat);
                    _kzSpr1->setFlipX(flipxBool);
                    _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                    _kzSpr1->setTag(Tag_PkLayer_KuZi1);
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
                            CCSprite* _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr1->setTag(Tag_PkLayer_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr2->setTag(Tag_PkLayer_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _kzSpr3->setTag(Tag_PkLayer_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                CCSprite* _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _wzSpr1->setTag(Tag_PkLayer_WaZi1);
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
                            CCSprite* _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr1->setTag(Tag_PkLayer_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr2->setTag(Tag_PkLayer_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _wzSpr3->setTag(Tag_PkLayer_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                CCSprite* _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                _xzSpr1->setTag(Tag_PkLayer_XieZi1);
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
                            CCSprite* _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr1->setTag(Tag_PkLayer_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr2->setTag(Tag_PkLayer_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _xzSpr3->setTag(Tag_PkLayer_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(widthFolt, heightFloat));
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
                                _spSpr1->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(widthFolt, heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_PkLayer_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                CCSprite* _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                _bSpr1->setTag(Tag_PkLayer_Bao1);
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
                            CCSprite* _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr1->setTag(Tag_PkLayer_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            CCSprite* _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr2->setTag(Tag_PkLayer_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            CCSprite* _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(widthFolt, heightFloat));
                            _bSpr3->setTag(Tag_PkLayer_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_PkLayer_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                CCSprite* _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                _zrSpr1->setTag(Tag_PkLayer_ZhuangRong1);
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
                            CCSprite* _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(widthFolt, heightFloat));
                            _zrSpr1->setTag(Tag_PkLayer_ZhuangRong1);
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


void PkLayer::backCallBack(CCObject* pSender){
    
}













