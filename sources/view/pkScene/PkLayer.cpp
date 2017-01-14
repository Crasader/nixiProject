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
#include "ConfigManager.h"
#include "Loading2.h"
#include "NetManager.h"
#include "AppUtil.h"
#include "PromptLayer.h"
#include "AudioManager.h"
#include "JiesuanLayer.h"



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
    
    return true;
}
PkLayer* PkLayer::create_with_Layer(int selfIndex1, int selfIndex2, int opponentIndex1, int opponentIndex2){
    PkLayer* rtn = PkLayer::create();
    rtn->init_with_Layer(selfIndex1, selfIndex2, opponentIndex1, opponentIndex2);
    
    return rtn;
}
void PkLayer::init_with_Layer(int selfIndex1, int selfIndex2, int opponentIndex1, int opponentIndex2){
    
    temSelfIndex1 = selfIndex1;
    temSelfIndex2 = selfIndex2;
    temOpponentIndex1 = opponentIndex1;
    temOpponentIndex2 = opponentIndex2;
    
    this->creatAnimation();
    
    this->creat_view();
    this->creat_nameKuang();
    this->creat_zhufuKuang();
    // 对决
    this->creatDuijue();
    
    
    this->creat_Man1();
    this->creat_Man2();
    
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatScoreAnimation1), .5f);
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
    
    
    CCArray* arrowAnimations2 = CCArray::createWithCapacity(10);
    char arrowStr2[100] = {};
    for (int i = 1; i <= 6; i++) {
        
        sprintf(arrowStr2, "res/pic/pk/animation/pk_huangguan%d.png", i);
        CCSpriteFrame* arrowFrame2 = CCSpriteFrame::create(arrowStr2, CCRectMake(0, 0, 44, 42));
        arrowAnimations2->addObject(arrowFrame2);
    }
    CCAnimation* arrowAnimation2 = CCAnimation::createWithSpriteFrames(arrowAnimations2, .2f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation2, "huangguanStr");
    
    
    CCArray* arrowAnimations3 = CCArray::createWithCapacity(10);
    char arrowStr3[100] = {};
    for (int i = 1; i <= 5; i++) {
        
        sprintf(arrowStr3, "res/pic/pk/animation/pk_leftXin%d.png", i);
        CCSpriteFrame* arrowFrame3 = CCSpriteFrame::create(arrowStr3, CCRectMake(0, 0, 71, 73));
        arrowAnimations3->addObject(arrowFrame3);
    }
    CCAnimation* arrowAnimation3 = CCAnimation::createWithSpriteFrames(arrowAnimations3, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation3, "leftXinStr");
    
    
    CCArray* arrowAnimations4 = CCArray::createWithCapacity(10);
    char arrowStr4[100] = {};
    for (int i = 1; i <= 5; i++) {
        
        sprintf(arrowStr4, "res/pic/pk/animation/pk_rightXin%d.png", i);
        CCSpriteFrame* arrowFrame4 = CCSpriteFrame::create(arrowStr4, CCRectMake(0, 0, 76, 61));
        arrowAnimations4->addObject(arrowFrame4);
    }
    CCAnimation* arrowAnimation4 = CCAnimation::createWithSpriteFrames(arrowAnimations4, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation4, "rightXinStr");
    
    
    CCArray* arrowAnimations5 = CCArray::createWithCapacity(10);
    char arrowStr5[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr5, "res/pic/pk/animation/pk_leftSui%d.png", i);
        CCSpriteFrame* arrowFrame5 = CCSpriteFrame::create(arrowStr5, CCRectMake(0, 0, 74, 55));
        arrowAnimations5->addObject(arrowFrame5);
    }
    CCAnimation* arrowAnimation5 = CCAnimation::createWithSpriteFrames(arrowAnimations5, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation5, "leftSuiStr");
    
    
    CCArray* arrowAnimations6 = CCArray::createWithCapacity(10);
    char arrowStr6[100] = {};
    for (int i = 1; i <= 3; i++) {
        
        sprintf(arrowStr6, "res/pic/pk/animation/pk_rightSui%d.png", i);
        CCSpriteFrame* arrowFrame6 = CCSpriteFrame::create(arrowStr6, CCRectMake(0, 0, 74, 55));
        arrowAnimations6->addObject(arrowFrame6);
    }
    CCAnimation* arrowAnimation6 = CCAnimation::createWithSpriteFrames(arrowAnimations6, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(arrowAnimation6, "rightSuiStr");
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
    scoreLabel1->set_delegate(NULL); // 设置代理
    scoreLabel1->setPosition(ccp(huangguanSpr->getContentSize().width* .28f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel1, 1);
    
    CCString* opponentScoreStr = CCString::createWithFormat("%d", opponentScore);
    scoreLabel2 = FlashNumberLabel2::create_with_atlas2("res/pic/pk/pk_number1.png", opponentScoreStr->getCString(), 0, .01f);
    scoreLabel2->setAnchorPoint(ccp(.5f, .5f));
    scoreLabel2->set_delegate2(NULL); // 设置代理
    scoreLabel2->setPosition(ccp(huangguanSpr->getContentSize().width* .72f, huangguanSpr->getContentSize().height* .908f));
    huangguanSpr->addChild(scoreLabel2, 1);
    
}
void PkLayer::creat_nameKuang(){
    CCSprite* nameKuangSpr1 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr1->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr1, 5);
    
    CCString* nameStr1 = CCString::createWithFormat("%s", selfItem->getNickname().c_str());
    CCLabelTTF* nameLabel1 = CCLabelTTF::create(nameStr1->getCString(), DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .8f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel1->setPosition(ccp(nameKuangSpr1->getContentSize().width* .56f, nameKuangSpr1->getContentSize().height* .5f));
    nameLabel1->setColor(ccc3(191, 71, 99));
    nameKuangSpr1->addChild(nameLabel1);
    
    
    CCSprite* nameKuangSpr2 = CCSprite::create("res/pic/pk/pk_kuang7.png");
    nameKuangSpr2->setFlipX(true);
    nameKuangSpr2->setAnchorPoint(ccp(.5f, 1.f));
    nameKuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .78f, DISPLAY->ScreenHeight() - 15.f));
    this->addChild(nameKuangSpr2, 5);
    
    CCString* nameStr2 = CCString::createWithFormat("%s", opponentItem->getNickname().c_str());
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(nameStr2->getCString(), DISPLAY->fangzhengFont(), 27, CCSizeMake(nameKuangSpr1->getContentSize().width* .8f, 27), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(nameKuangSpr2->getContentSize().width* .45f, nameKuangSpr2->getContentSize().height* .5f));
    nameLabel2->setColor(ccc3(191, 71, 99));
    nameKuangSpr2->addChild(nameLabel2);
}
void PkLayer::creat_zhufuKuang(){
    CCSprite* zhufuKuangSpr1 = CCSprite::create("res/pic/pk/pk_kuang6.png");
    zhufuKuangSpr1->setAnchorPoint(ccp(1.f, 0.f));
    zhufuKuangSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f - 3, 2.f));
    this->addChild(zhufuKuangSpr1, 5);
    
    CCString* zhufuStr1;
    if (temSelfIndex1 == 1) {// 搭配
        zhufuStr1 = CCString::createWithFormat("搭配增加%d%%", temSelfIndex2);
    }else if (temSelfIndex1 == 2){// 人气
        zhufuStr1 = CCString::createWithFormat("人气增加%d%%", temSelfIndex2);
    }else if (temSelfIndex1 == 3){// 魅力
        zhufuStr1 = CCString::createWithFormat("魅力增加%d%%", temSelfIndex2);
    }else if (temSelfIndex1 == 4){// 运气
        zhufuStr1 = CCString::createWithFormat("运气增加%d%%", temSelfIndex2);
    }else if (temSelfIndex1 == 5){// 总分
        zhufuStr1 = CCString::createWithFormat("总分增加%d%%", temSelfIndex2);
    }else{
        zhufuStr1 = CCString::createWithFormat("无");
    }
    CCLabelTTF* zhufuLabel1 = CCLabelTTF::create(zhufuStr1->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(zhufuKuangSpr1->getContentSize().width* .9f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    zhufuLabel1->setPosition(ccp(zhufuKuangSpr1->getContentSize().width* .5f, zhufuKuangSpr1->getContentSize().height* .46f));
    zhufuLabel1->setColor(ccc3(231, 161, 127));
    zhufuKuangSpr1->addChild(zhufuLabel1);
    
    
    CCSprite* zhufuKuangSpr2 = CCSprite::create("res/pic/pk/pk_kuang6.png");
    zhufuKuangSpr2->setFlipX(true);
    zhufuKuangSpr2->setAnchorPoint(ccp(0.f, 0.f));
    zhufuKuangSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 3, 2.f));
    this->addChild(zhufuKuangSpr2, 5);
    
    CCString* zhufuStr2;
    if (temOpponentIndex1 == 1) {// 搭配
        zhufuStr2 = CCString::createWithFormat("搭配增加%d%%", temOpponentIndex2);
    }else if (temOpponentIndex1 == 2){// 人气
        zhufuStr2 = CCString::createWithFormat("人气增加%d%%", temOpponentIndex2);
    }else if (temOpponentIndex1 == 3){// 魅力
        zhufuStr2 = CCString::createWithFormat("魅力增加%d%%", temOpponentIndex2);
    }else if (temOpponentIndex1 == 4){// 运气
        zhufuStr2 = CCString::createWithFormat("运气增加%d%%", temOpponentIndex2);
    }else if (temOpponentIndex1 == 5){// 总分
        zhufuStr2 = CCString::createWithFormat("总分增加%d%%", temOpponentIndex2);
    }else{
        zhufuStr2 = CCString::createWithFormat("无");
    }
    CCLabelTTF* zhufuLabel2 = CCLabelTTF::create(zhufuStr2->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(zhufuKuangSpr2->getContentSize().width* .9f, 25), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
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
    int actionIndex = 0;
    if (type == 1) {// 搭配
        selfIndex1 = selfItem->getMatch();
        opponentIndex1 = opponentItem->getMatch();
        tempSelfScore = selfItem->getMatch();
        tempOpponentScore = opponentItem->getMatch();
        if (tempSelfScore > tempOpponentScore) {
            actionIndex = 1;
        }else if (tempSelfScore == tempOpponentScore){
            actionIndex = 2;
        }else{
            actionIndex = 0;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_dapei2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_dapei1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_dapei2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_dapei1.png");
    }else if (type == 2){// 魅力
        selfIndex2 = selfItem->getCharm();
        opponentIndex2 = opponentItem->getCharm();
        tempSelfScore = selfItem->getCharm();
        tempOpponentScore = opponentItem->getCharm();
        if (tempSelfScore > tempOpponentScore) {
            actionIndex = 1;
        }else if (tempSelfScore == tempOpponentScore){
            actionIndex = 2;
        }else{
            actionIndex = 0;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_meili2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_meili1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_meili2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_meili1.png");
    }else if (type == 3){// 人气
        selfIndex3 = selfItem->getPuplar();
        opponentIndex3 = opponentItem->getPuplar();
        tempSelfScore = selfItem->getPuplar();
        tempOpponentScore = opponentItem->getPuplar();
        if (tempSelfScore > tempOpponentScore) {
            actionIndex = 1;
        }else if (tempSelfScore == tempOpponentScore){
            actionIndex = 2;
        }else{
            actionIndex = 0;
        }
        dapeiSpr1_1 = CCSprite::create("res/pic/pk/pk_renqi2.png");
        dapeiSpr1_2 = CCSprite::create("res/pic/pk/pk_renqi1.png");
        dapeiSpr2_1 = CCSprite::create("res/pic/pk/pk_renqi2.png");
        dapeiSpr2_2 = CCSprite::create("res/pic/pk/pk_renqi1.png");
    }else if (type == 4){// 幸运
        selfIndex4 = selfItem->getLuck();
        opponentIndex4 = opponentItem->getLuck();
        tempSelfScore = selfItem->getLuck();
        tempOpponentScore = opponentItem->getLuck();
        if (tempSelfScore > tempOpponentScore) {
            actionIndex = 1;
        }else if (tempSelfScore == tempOpponentScore){
            actionIndex = 2;
        }else{
            actionIndex = 0;
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
    
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .4f + 15.f));
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .4f));
    CCMoveTo* moveTo1_3 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .4f + 15.f));
    CCMoveTo* moveTo1_4 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .4f));
    CCSequence* seq1_1 = CCSequence::create(moveTo1_1, moveTo1_2, moveTo1_3, moveTo1_4, NULL);
    
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .4f + 15.f));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .4f));
    CCMoveTo* moveTo2_3 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .4f + 15.f));
    CCMoveTo* moveTo2_4 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .4f));
    CCSequence* seq2_1 = CCSequence::create(moveTo2_1, moveTo2_2, moveTo2_3, moveTo2_4, NULL);
    
    if (actionIndex == 1) {
        //
        CCMoveTo* moveTo1 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .8f));
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
        CCMoveTo* moveTo2 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .55f));
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
    }else if (actionIndex == 2){
        //
        CCMoveTo* moveTo1 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .55f));
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
        CCMoveTo* moveTo2 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .55f));
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
    }else if (actionIndex == 0) {
        //
        CCMoveTo* moveTo1 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .25f, DISPLAY->ScreenHeight()* .55f));
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
        CCMoveTo* moveTo2 = CCMoveTo::create(.7f, ccp(DISPLAY->ScreenWidth()* .75f, DISPLAY->ScreenHeight()* .8f));
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
    
//    CCMoveTo* moveTo2 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
//    jiesuanGuangSpr1->runAction(moveTo2);
    jiesuanGuangSpr1->runAction(CCHide::create());
    
//    CCMoveTo* moveTo3 = CCMoveTo::create(.8f, ccp(DISPLAY->ScreenWidth()* .51f, DISPLAY->ScreenHeight()* .72f));
//    jiesuanQuanSpr->runAction(moveTo3);
    jiesuanQuanSpr->runAction(CCHide::create());
    
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
    CCString* selfStr1 = CCString::createWithFormat("%d", selfItem->getMatch());
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
    
    CCString* selfStr2 = CCString::createWithFormat("%d", selfItem->getPuplar());
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
    
    CCString* selfStr3 = CCString::createWithFormat("%d", selfItem->getCharm());
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
    
    CCString* selfStr4 = CCString::createWithFormat("%d", selfItem->getLuck());
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
    CCString* opponentStr1 = CCString::createWithFormat("%d", opponentItem->getMatch());
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
    
    CCString* opponentStr2 = CCString::createWithFormat("%d", opponentItem->getPuplar());
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
    
    CCString* opponentStr3 = CCString::createWithFormat("%d", opponentItem->getCharm());
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
    
    CCString* opponentStr4 = CCString::createWithFormat("%d", opponentItem->getLuck());
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
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatJiesuan2), 1.5f);
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
    
    if (temSelfIndex1 == 1) {
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x111);
        if (selfIndex1 > 0) {
            CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(selfIndex1 += selfIndex1*(temSelfIndex2* 0.01)));
            label1->set_new_number(scoreStr1->getCString());
        }
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 1);
        
        
        CCSprite* xingSpr1 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr1->setPosition(ccp(labSpr1->getContentSize().width* .9f, labSpr1->getContentSize().height* .9f));
        labSpr1->addChild(xingSpr1);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(fadeOut, CCDelayTime::create(.2f), fadeIn, CCDelayTime::create(.2f), NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* leftXinStr1 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .805f));
        jiesuanKuangSpr->addChild(leftXinStr1, 17);
        CCAnimation* quanAnimation1 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate1 = CCAnimate::create(quanAnimation1);
        leftXinStr1->runAction(CCRepeatForever::create(quanAnimate1));
    }else if (temSelfIndex1 == 2){
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x222);
        if (selfIndex2 > 0) {
            CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(selfIndex2 += selfIndex2*(temSelfIndex2* 0.01)));
            label2->set_new_number(scoreStr2->getCString());
        }
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 1);
        
        
        CCSprite* xingSpr2 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr2->setPosition(ccp(labSpr2->getContentSize().width* .9f, labSpr2->getContentSize().height* .9f));
        labSpr2->addChild(xingSpr2);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(fadeOut, CCDelayTime::create(.2f), fadeIn, CCDelayTime::create(.2f), NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr2->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* leftXinStr2 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .63f));
        jiesuanKuangSpr->addChild(leftXinStr2, 17);
        CCAnimation* quanAnimation2 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate2 = CCAnimate::create(quanAnimation2);
        leftXinStr2->runAction(CCRepeatForever::create(quanAnimate2));
    }else if (temSelfIndex1 == 3){
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x333);
        if (selfIndex3 > 0) {
            CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(selfIndex3 += selfIndex3*(temSelfIndex2* 0.01)));
            label3->set_new_number(scoreStr3->getCString());
        }
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 1);
        
        
        CCSprite* xingSpr3 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr3->setPosition(ccp(labSpr3->getContentSize().width* .9f, labSpr3->getContentSize().height* .9f));
        labSpr3->addChild(xingSpr3);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(fadeOut, CCDelayTime::create(.2f), fadeIn, CCDelayTime::create(.2f), NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr3->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* leftXinStr3 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .465f));
        jiesuanKuangSpr->addChild(leftXinStr3, 17);
        CCAnimation* quanAnimation3 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate3 = CCAnimate::create(quanAnimation3);
        leftXinStr3->runAction(CCRepeatForever::create(quanAnimate3));
    }else if (temSelfIndex1 == 4){
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x444);
        if (selfIndex4 > 0) {
            CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(selfIndex4 += selfIndex4*(temSelfIndex2* 0.01)));
            label4->set_new_number(scoreStr4->getCString());
        }
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 1);
        
        
        CCSprite* xingSpr4 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr4->setPosition(ccp(labSpr4->getContentSize().width* .9f, labSpr4->getContentSize().height* .9f));
        labSpr4->addChild(xingSpr4);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(fadeOut, CCDelayTime::create(.2f), fadeIn, CCDelayTime::create(.2f), NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr4->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* leftXinStr4 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .305f));
        jiesuanKuangSpr->addChild(leftXinStr4, 17);
        CCAnimation* quanAnimation4 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate4 = CCAnimate::create(quanAnimation4);
        leftXinStr4->runAction(CCRepeatForever::create(quanAnimate4));
    }else if (temSelfIndex1 == 5){
        //
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x111);
        if (selfIndex1 > 0) {
            CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(selfIndex1 += selfIndex1*(temSelfIndex2* 0.01)));
            label1->set_new_number(scoreStr1->getCString());
        }
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 1);
        
        //
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x222);
        if (selfIndex2 > 0) {
            CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(selfIndex2 += selfIndex2*(temSelfIndex2* 0.01)));
            label2->set_new_number(scoreStr2->getCString());
        }
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 1);
        
        //
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x333);
        if (selfIndex3 > 0) {
            CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(selfIndex3 += selfIndex3*(temSelfIndex2* 0.01)));
            label3->set_new_number(scoreStr3->getCString());
        }
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 1);
        
        //
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x444);
        if (selfIndex4 > 0) {
            CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(selfIndex4 += selfIndex4*(temSelfIndex2* 0.01)));
            label4->set_new_number(scoreStr4->getCString());
        }
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temSelfIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 1);
        
        
        CCSprite* leftXinStr1 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .805f));
        jiesuanKuangSpr->addChild(leftXinStr1, 17);
        CCAnimation* quanAnimation1 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate1 = CCAnimate::create(quanAnimation1);
        leftXinStr1->runAction(CCRepeatForever::create(quanAnimate1));
        
        CCSprite* leftXinStr2 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .63f));
        jiesuanKuangSpr->addChild(leftXinStr2, 17);
        CCAnimation* quanAnimation2 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate2 = CCAnimate::create(quanAnimation2);
        leftXinStr2->runAction(CCRepeatForever::create(quanAnimate2));
        
        CCSprite* leftXinStr3 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .465f));
        jiesuanKuangSpr->addChild(leftXinStr3, 17);
        CCAnimation* quanAnimation3 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate3 = CCAnimate::create(quanAnimation3);
        leftXinStr3->runAction(CCRepeatForever::create(quanAnimate3));
        
        CCSprite* leftXinStr4 = CCSprite::create("res/pic/pk/animation/pk_leftXin1.png");
        leftXinStr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .07f, jiesuanKuangSpr->getContentSize().height* .305f));
        jiesuanKuangSpr->addChild(leftXinStr4, 17);
        CCAnimation* quanAnimation4 = CCAnimationCache::sharedAnimationCache()->animationByName("leftXinStr");
        CCAnimate* quanAnimate4 = CCAnimate::create(quanAnimation4);
        leftXinStr4->runAction(CCRepeatForever::create(quanAnimate4));
    }
    
    
    if (temOpponentIndex1 == 1) {
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x555);
        if (opponentIndex1 > 0) {
            CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(opponentIndex1 += opponentIndex1*(temOpponentIndex2* 0.01)));
            label1->set_new_number(scoreStr1->getCString());
        }
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 2);
        
        
        CCSprite* xingSpr1 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr1->setPosition(ccp(labSpr1->getContentSize().width* .9f, labSpr1->getContentSize().height* .9f));
        labSpr1->addChild(xingSpr1);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(.2f), fadeOut, CCDelayTime::create(.2f), fadeIn, NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr1->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* rightXinStr1 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .795f));
        jiesuanKuangSpr->addChild(rightXinStr1, 17);
        CCAnimation* quanAnimation1 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate1 = CCAnimate::create(quanAnimation1);
        rightXinStr1->runAction(CCRepeatForever::create(quanAnimate1));
    }else if (temOpponentIndex1 == 2){
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x666);
        if (opponentIndex2 > 0) {
            CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(opponentIndex2 += opponentIndex2*(temOpponentIndex2* 0.01)));
            label2->set_new_number(scoreStr2->getCString());
        }
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 2);
        
        
        CCSprite* xingSpr2 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr2->setPosition(ccp(labSpr2->getContentSize().width* .9f, labSpr2->getContentSize().height* .9f));
        labSpr2->addChild(xingSpr2);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(.2f), fadeOut, CCDelayTime::create(.2f), fadeIn, NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr2->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* rightXinStr2 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .62f));
        jiesuanKuangSpr->addChild(rightXinStr2, 17);
        CCAnimation* quanAnimation2 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate2 = CCAnimate::create(quanAnimation2);
        rightXinStr2->runAction(CCRepeatForever::create(quanAnimate2));
    }else if (temOpponentIndex1 == 3){
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x777);
        if (opponentIndex3 > 0) {
            CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(opponentIndex3 += opponentIndex3*(temOpponentIndex2* 0.01)));
            label3->set_new_number(scoreStr3->getCString());
        }
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 2);
        
        
        CCSprite* xingSpr3 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr3->setPosition(ccp(labSpr3->getContentSize().width* .9f, labSpr3->getContentSize().height* .9f));
        labSpr3->addChild(xingSpr3);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(.2f), fadeOut, CCDelayTime::create(.2f), fadeIn, NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr3->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* rightXinStr3 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .46f));
        jiesuanKuangSpr->addChild(rightXinStr3, 17);
        CCAnimation* quanAnimation3 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate3 = CCAnimate::create(quanAnimation3);
        rightXinStr3->runAction(CCRepeatForever::create(quanAnimate3));
    }else if (temOpponentIndex1 == 4){
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x888);
        if (opponentIndex4 > 0) {
            CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(opponentIndex4 += opponentIndex4*(temOpponentIndex2* 0.01)));
            label4->set_new_number(scoreStr4->getCString());
        }
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 2);
        
        
        CCSprite* xingSpr4 = CCSprite::create("res/pic/pk/pk_guang4.png");
        xingSpr4->setPosition(ccp(labSpr4->getContentSize().width* .9f, labSpr4->getContentSize().height* .9f));
        labSpr4->addChild(xingSpr4);
        CCRotateBy* action = CCRotateBy::create(1.f, 50);
        CCFadeOut* fadeOut = CCFadeOut::create(.3f);
        CCFadeIn* fadeIn = CCFadeIn::create(.3f);
        CCSequence* seq = CCSequence::create(CCDelayTime::create(.2f), fadeOut, CCDelayTime::create(.2f), fadeIn, NULL);
        CCSpawn* spawn = CCSpawn::create(action, seq, NULL);
        xingSpr4->runAction(CCRepeatForever::create(CCRepeatForever::create(spawn)));
        
        
        CCSprite* rightXinStr4 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .295f));
        jiesuanKuangSpr->addChild(rightXinStr4, 17);
        CCAnimation* quanAnimation4 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate4 = CCAnimate::create(quanAnimation4);
        rightXinStr4->runAction(CCRepeatForever::create(quanAnimate4));
    }else if (temOpponentIndex1 == 5){
        //
        FlashNumberLabel* label1 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x555);
        if (opponentIndex1 > 0) {
            CCString* scoreStr1 = CCString::createWithFormat("%d", (int)(opponentIndex1 += opponentIndex1*(temOpponentIndex2* 0.01)));
            label1->set_new_number(scoreStr1->getCString());
        }
        CCString* labStr1 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr1 = CCSprite::create(labStr1->getCString());
        labSpr1->setScale(.8f);
        labSpr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .855f));
        jiesuanKuangSpr->addChild(labSpr1);
        this->flashNumberAnimation(labSpr1, 2);
        
        
        //
        FlashNumberLabel* label2 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x666);
        if (opponentIndex2 > 0) {
            CCString* scoreStr2 = CCString::createWithFormat("%d", (int)(opponentIndex2 += opponentIndex2*(temOpponentIndex2* 0.01)));
            label2->set_new_number(scoreStr2->getCString());
        }
        CCString* labStr2 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr2 = CCSprite::create(labStr2->getCString());
        labSpr2->setScale(.8f);
        labSpr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .685f));
        jiesuanKuangSpr->addChild(labSpr2);
        this->flashNumberAnimation(labSpr2, 2);
        
        
        //
        FlashNumberLabel* label3 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x777);
        if (opponentIndex3 > 0) {
            CCString* scoreStr3 = CCString::createWithFormat("%d", (int)(opponentIndex3 += opponentIndex3*(temOpponentIndex2* 0.01)));
            label3->set_new_number(scoreStr3->getCString());
        }
        CCString* labStr3 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr3 = CCSprite::create(labStr3->getCString());
        labSpr3->setScale(.8f);
        labSpr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .515f));
        jiesuanKuangSpr->addChild(labSpr3);
        this->flashNumberAnimation(labSpr3, 2);
        
        
        //
        FlashNumberLabel* label4 = (FlashNumberLabel* )jiesuanKuangSpr->getChildByTag(0x888);
        if (opponentIndex4 > 0) {
            CCString* scoreStr4 = CCString::createWithFormat("%d", (int)(opponentIndex4 += opponentIndex4*(temOpponentIndex2* 0.01)));
            label4->set_new_number(scoreStr4->getCString());
        }
        CCString* labStr4 = CCString::createWithFormat("res/pic/pk/pk_%d.png", temOpponentIndex2);
        CCSprite* labSpr4 = CCSprite::create(labStr4->getCString());
        labSpr4->setScale(.8f);
        labSpr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .35f));
        jiesuanKuangSpr->addChild(labSpr4);
        this->flashNumberAnimation(labSpr4, 2);
        
        
        
        CCSprite* rightXinStr1 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .795f));
        jiesuanKuangSpr->addChild(rightXinStr1, 17);
        CCAnimation* quanAnimation1 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate1 = CCAnimate::create(quanAnimation1);
        rightXinStr1->runAction(CCRepeatForever::create(quanAnimate1));
        
        CCSprite* rightXinStr2 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .62f));
        jiesuanKuangSpr->addChild(rightXinStr2, 17);
        CCAnimation* quanAnimation2 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate2 = CCAnimate::create(quanAnimation2);
        rightXinStr2->runAction(CCRepeatForever::create(quanAnimate2));
        
        CCSprite* rightXinStr3 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .46f));
        jiesuanKuangSpr->addChild(rightXinStr3, 17);
        CCAnimation* quanAnimation3 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate3 = CCAnimate::create(quanAnimation3);
        rightXinStr3->runAction(CCRepeatForever::create(quanAnimate3));
        
        CCSprite* rightXinStr4 = CCSprite::create("res/pic/pk/animation/pk_rightXin1.png");
        rightXinStr4->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .93f, jiesuanKuangSpr->getContentSize().height* .295f));
        jiesuanKuangSpr->addChild(rightXinStr4, 17);
        CCAnimation* quanAnimation4 = CCAnimationCache::sharedAnimationCache()->animationByName("rightXinStr");
        CCAnimate* quanAnimate4 = CCAnimate::create(quanAnimation4);
        rightXinStr4->runAction(CCRepeatForever::create(quanAnimate4));
    }
    
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::creatJiesuan3), 1.f);
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
void PkLayer::creatJiesuan3(){
    CCLabelAtlas* jiesuanScoreLabel1;
    CCLabelAtlas* jiesuanScoreLabel2;
    CCScaleTo* scaleTo1,* scaleTo2,* scaleTo3,* scaleTo4;
    
    selfScore = selfIndex1 + selfIndex2 + selfIndex3 + selfIndex4;
    opponentScore = opponentIndex1 + opponentIndex2 + opponentIndex3 + opponentIndex4;
    
    CCString* scoreStr1 = CCString::createWithFormat("%d", selfIndex1 + selfIndex2 + selfIndex3 + selfIndex4);
    CCString* scoreStr2 = CCString::createWithFormat("%d", opponentIndex1 + opponentIndex2 + opponentIndex3 + opponentIndex4);
    if (selfScore > opponentScore) {
        jiesuanScoreLabel1 = CCLabelAtlas::create(scoreStr1->getCString(), "res/pic/pk/jiesuan/pk_number.png", 26, 35, '0');
        jiesuanScoreLabel2 = CCLabelAtlas::create(scoreStr2->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
        scaleTo1 = CCScaleTo::create(.5f, 1.2f);
        scaleTo2 = CCScaleTo::create(.3f, 1.1f);
        scaleTo3 = CCScaleTo::create(.5f, 1.05f);
        scaleTo4 = CCScaleTo::create(.3f, 1.f);
    }else if (selfScore == opponentScore){
        jiesuanScoreLabel1 = CCLabelAtlas::create(scoreStr1->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
        jiesuanScoreLabel2 = CCLabelAtlas::create(scoreStr2->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
        scaleTo1 = CCScaleTo::create(.5f, 1.05f);
        scaleTo2 = CCScaleTo::create(.3f, 1.f);
        scaleTo3 = CCScaleTo::create(.5f, 1.05f);
        scaleTo4 = CCScaleTo::create(.3f, 1.1f);
    }else if (selfScore < opponentScore) {
        jiesuanScoreLabel1 = CCLabelAtlas::create(scoreStr1->getCString(), "res/pic/pk/pk_number2.png", 21, 27, '0');
        jiesuanScoreLabel2 = CCLabelAtlas::create(scoreStr2->getCString(), "res/pic/pk/jiesuan/pk_number.png", 26, 35, '0');
        scaleTo1 = CCScaleTo::create(.5f, 1.05f);
        scaleTo2 = CCScaleTo::create(.3f, 1.f);
        scaleTo3 = CCScaleTo::create(.5f, 1.2f);
        scaleTo4 = CCScaleTo::create(.3f, 1.1f);
    }
    
    jiesuanScoreLabel1->setVisible(false);
    jiesuanScoreLabel1->setScale(.2f);
    jiesuanScoreLabel1->setAnchorPoint(ccp(.5f, .5f));
    jiesuanScoreLabel1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
    jiesuanKuangSpr->addChild(jiesuanScoreLabel1);
    
    CCSequence* seq1 = CCSequence::create(scaleTo1, scaleTo2, NULL);
    CCSpawn* spawn1 = CCSpawn::create(CCShow::create(), seq1, NULL);
    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(.5f), spawn1, NULL);
    jiesuanScoreLabel1->runAction(seq2);
    
    
    jiesuanScoreLabel2->setVisible(false);
    jiesuanScoreLabel2->setScale(.2f);
    jiesuanScoreLabel2->setAnchorPoint(ccp(.5f, .5f));
    jiesuanScoreLabel2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
    jiesuanKuangSpr->addChild(jiesuanScoreLabel2);
    
    CCSequence* seq3 = CCSequence::create(scaleTo3, scaleTo4, NULL);
    CCSpawn* spawn2 = CCSpawn::create(CCShow::create(), seq3, NULL);
    CCSequence* seq4;
    if (selfScore > opponentScore) {
        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(PkLayer::creatJiesuan4));
        seq4 = CCSequence::create(CCDelayTime::create(.5f), spawn2, callFuncN, NULL);
    }else if (selfScore == opponentScore){
        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(PkLayer::nextLayer1));
        seq4 = CCSequence::create(CCDelayTime::create(.5f), spawn2, CCDelayTime::create(.8f), callFuncN, NULL);
    }else if (selfScore < opponentScore) {
        CCCallFuncN* callFuncN = CCCallFuncN::create(this, callfuncN_selector(PkLayer::creatJiesuan4));
        seq4 = CCSequence::create(CCDelayTime::create(.5f), spawn2, callFuncN, NULL);
    }
    jiesuanScoreLabel2->runAction(seq4);
    
    
    if (selfScore > opponentScore) {
        xingSpr1_1 = CCSprite::create("res/pic/pk/animation/pk_xingxing1.png");
        xingSpr1_1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_1 = 50+ rand()%205;
        if (xingIndex1_1 > 50 && xingIndex1_1< 255) {
            xingBool1_1 = false;
        }else{
            xingBool1_1 = true;
        }
        xingSpr1_1->setOpacity(xingIndex1_1);
        jiesuanKuangSpr->addChild(xingSpr1_1, 12);
        
        xingSpr1_2 = CCSprite::create("res/pic/pk/animation/pk_xingxing2.png");
        xingSpr1_2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_2 = 50+ rand()%205;
        if (xingIndex1_2 > 50 && xingIndex1_2< 255) {
            xingBool1_2 = false;
        }else{
            xingBool1_2 = true;
        }
        xingSpr1_2->setOpacity(xingIndex1_2);
        jiesuanKuangSpr->addChild(xingSpr1_2, 12);
        
        xingSpr1_3 = CCSprite::create("res/pic/pk/animation/pk_xingxing3.png");
        xingSpr1_3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_3 = 50+ rand()%205;
        if (xingIndex1_3 > 50 && xingIndex1_3< 255) {
            xingBool1_3 = false;
        }else{
            xingBool1_3 = true;
        }
        xingSpr1_3->setOpacity(xingIndex1_3);
        jiesuanKuangSpr->addChild(xingSpr1_3, 12);
        
        CCString* selfScoreStr1 = CCString::createWithFormat("%d", selfScore);
        scoreLabel1->set_new_number(selfScoreStr1->getCString());
        
        CCString* selfScoreStr2 = CCString::createWithFormat("%d", opponentScore);
        scoreLabel2->set_new_number2(selfScoreStr2->getCString());
        
        this->schedule(SEL_SCHEDULE(&PkLayer::updateOpacity), .1f);
    }else if (selfScore < opponentScore){
        xingSpr1_1 = CCSprite::create("res/pic/pk/animation/pk_xingxing1.png");
        xingSpr1_1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_1 = 50+ rand()%205;
        if (xingIndex1_1 > 50 && xingIndex1_1< 255) {
            xingBool1_1 = false;
        }else{
            xingBool1_1 = true;
        }
        xingSpr1_1->setOpacity(xingIndex1_1);
        jiesuanKuangSpr->addChild(xingSpr1_1, 12);
        
        xingSpr1_2 = CCSprite::create("res/pic/pk/animation/pk_xingxing2.png");
        xingSpr1_2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_2 = 50+ rand()%205;
        if (xingIndex1_2 > 50 && xingIndex1_2< 255) {
            xingBool1_2 = false;
        }else{
            xingBool1_2 = true;
        }
        xingSpr1_2->setOpacity(xingIndex1_2);
        jiesuanKuangSpr->addChild(xingSpr1_2, 12);
        
        xingSpr1_3 = CCSprite::create("res/pic/pk/animation/pk_xingxing3.png");
        xingSpr1_3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
        xingIndex1_3 = 50+ rand()%205;
        if (xingIndex1_3 > 50 && xingIndex1_3< 255) {
            xingBool1_3 = false;
        }else{
            xingBool1_3 = true;
        }
        xingSpr1_3->setOpacity(xingIndex1_3);
        jiesuanKuangSpr->addChild(xingSpr1_3, 12);
        
        CCString* selfScoreStr1 = CCString::createWithFormat("%d", selfScore);
        scoreLabel1->set_new_number(selfScoreStr1->getCString());
        
        CCString* selfScoreStr2 = CCString::createWithFormat("%d", opponentScore);
        scoreLabel2->set_new_number2(selfScoreStr2->getCString());
        
        this->schedule(SEL_SCHEDULE(&PkLayer::updateOpacity), .1f);
    }
    
//    xingSpr1_1 = CCSprite::create("res/pic/pk/animation/pk_xingxing1.png");
//    if (selfScore > opponentScore) {
//        xingSpr1_1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }else{
//        xingSpr1_1->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }
//    xingIndex1_1 = 50+ rand()%205;
//    if (xingIndex1_1 > 50 && xingIndex1_1< 255) {
//        xingBool1_1 = false;
//    }else{
//        xingBool1_1 = true;
//    }
//    xingSpr1_1->setOpacity(xingIndex1_1);
//    jiesuanKuangSpr->addChild(xingSpr1_1, 12);
//    
//    xingSpr1_2 = CCSprite::create("res/pic/pk/animation/pk_xingxing2.png");
//    if (selfScore > opponentScore) {
//        xingSpr1_2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }else{
//        xingSpr1_2->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }
//    xingIndex1_2 = 50+ rand()%205;
//    if (xingIndex1_2 > 50 && xingIndex1_2< 255) {
//        xingBool1_2 = false;
//    }else{
//        xingBool1_2 = true;
//    }
//    xingSpr1_2->setOpacity(xingIndex1_2);
//    jiesuanKuangSpr->addChild(xingSpr1_2, 12);
//    
//    xingSpr1_3 = CCSprite::create("res/pic/pk/animation/pk_xingxing3.png");
//    if (selfScore > opponentScore) {
//        xingSpr1_3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }else{
//        xingSpr1_3->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .07f));
//    }
//    xingIndex1_3 = 50+ rand()%205;
//    if (xingIndex1_3 > 50 && xingIndex1_3< 255) {
//        xingBool1_3 = false;
//    }else{
//        xingBool1_3 = true;
//    }
//    xingSpr1_3->setOpacity(xingIndex1_3);
//    jiesuanKuangSpr->addChild(xingSpr1_3, 12);
//    
//    
//    //
//    CCString* selfScoreStr1 = CCString::createWithFormat("%d", selfScore);
//    scoreLabel1->set_new_number(selfScoreStr1->getCString());
//    
//    CCString* selfScoreStr2 = CCString::createWithFormat("%d", opponentScore);
//    scoreLabel2->set_new_number2(selfScoreStr2->getCString());
//    
//    this->schedule(SEL_SCHEDULE(&PkLayer::updateOpacity), .1f);
}
void PkLayer::updateOpacity(float dt){
    if (xingBool1_1) {
        if (xingIndex1_1 > 50 && xingIndex1_1 <= 255) {
            xingIndex1_1 -= 20;
        }else{
            xingBool1_1 = false;
            xingIndex1_1 += 20;
            if (xingIndex1_1 >= 255) {
                xingIndex1_1 = 255;
            }
        }
    }else{
        if (xingIndex1_1 > 50 && xingIndex1_1 < 255) {
            xingIndex1_1 += 20;
            if (xingIndex1_1 >= 255) {
                xingIndex1_1 = 255;
            }
        }else{
            xingBool1_1 = true;
            xingIndex1_1 -= 20;
        }
    }
    xingSpr1_1->setOpacity(xingIndex1_1);
    
    if (xingBool1_2) {
        if (xingIndex1_2 > 50 && xingIndex1_2 <= 255) {
            xingIndex1_2 -= 20;
        }else{
            xingBool1_2 = false;
            xingIndex1_2 += 20;
            if (xingIndex1_2 >= 255) {
                xingIndex1_2 = 255;
            }
        }
    }else{
        if (xingIndex1_2 > 50 && xingIndex1_2 < 255) {
            xingIndex1_2 += 20;
            if (xingIndex1_2 >= 255) {
                xingIndex1_2 = 255;
            }
        }else{
            xingBool1_2 = true;
            xingIndex1_2 -= 20;
        }
    }
    xingSpr1_2->setOpacity(xingIndex1_2);
    
    if (xingBool1_3) {
        if (xingIndex1_3 > 50 && xingIndex1_3 <= 255) {
            xingIndex1_3 -= 20;
        }else{
            xingBool1_3 = false;
            xingIndex1_3 += 20;
            if (xingIndex1_3 >= 255) {
                xingIndex1_3 = 255;
            }
        }
    }else{
        if (xingIndex1_3 > 50 && xingIndex1_3 < 255) {
            xingIndex1_3 += 20;
            if (xingIndex1_3 >= 255) {
                xingIndex1_3 = 255;
            }
        }else{
            xingBool1_3 = true;
            xingIndex1_3 -= 20;
        }
    }
    xingSpr1_3->setOpacity(xingIndex1_3);
}
void PkLayer::creatJiesuan4(){
    CCSprite* huangguanSpr = CCSprite::create("res/pic/pk/animation/pk_huangguan1.png");
    if (selfScore > opponentScore) {
        huangguanSpr->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .25f, jiesuanKuangSpr->getContentSize().height* .17f));
    }else{
        huangguanSpr->setPosition(ccp(jiesuanKuangSpr->getContentSize().width* .75f, jiesuanKuangSpr->getContentSize().height* .17f));
    }
    jiesuanKuangSpr->addChild(huangguanSpr);
    
    CCAnimation* quanAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("huangguanStr");
    CCAnimate* quanAnimate = CCAnimate::create(quanAnimation);
    huangguanSpr->runAction(quanAnimate);
    
    
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::nextLayer1), 1.2f);
}
void PkLayer::nextLayer1(){
    LOADING->show_loading();
    this->scheduleOnce(SEL_SCHEDULE(&PkLayer::nextLayer2), .5f);
}
void PkLayer::nextLayer2(){
    LOADING->remove();
    
    CCScene* pScene = CCScene::create();
    JiesuanLayer* layer = JiesuanLayer::create_with_Layer(selfScore, opponentScore, bgIndex);
    pScene->addChild(layer);
    CCTransitionMoveInB* trans = CCTransitionMoveInB::create(.3f, pScene);
    CCDirector::sharedDirector()->replaceScene(trans);
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
    
    
    CCDictionary* clothesDic = DATA->getCompetition()->getSelf()->getOndress(); //DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
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
    
    
    bgIndex = rand()%100;
    CCString* bgStr;
    if (bgIndex > 0 && bgIndex <= 30) {
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 1);
    }else if (bgIndex > 30 && bgIndex <= 60){
        bgStr = CCString::createWithFormat("res/pic/house/house_%d.png", 2);
    }else if (bgIndex > 60 && bgIndex <= 90){
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
    
    
    CCDictionary* clothesDic = opponentItem->getOndress(); // 男宠衣着
    this->initClothes(holesClipper, DISPLAY->ScreenWidth()* widthFolt, bgSpr1->getContentSize().height* heightFloat, scaleFloat, flipxBool, clothesDic);
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













