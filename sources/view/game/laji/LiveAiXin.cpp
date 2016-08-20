//
//  LiveAiXin.cpp
//  mm3c
//
//  Created by lakkey on 16-1-12.
//
//

#include "LiveAiXin.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "AppUtil.h"


Garbage * Garbage::create(int type)
{
    Garbage *pRet = new Garbage();
    if (pRet && pRet->initWithFile(type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool Garbage::initWithFile(int type)
{
    CCString* filename = CCString::createWithFormat("res/pic/game/laji/zjw_big_%d.png", type);
    if (!CCSprite::initWithFile(filename->getCString())) {
        return false;
    }
    
    if (type == 8) {
        result = -3;
    }
    else {
        result = 1;
    }
    
    return true;
}

int Garbage::getResult()
{
    return result;
}


// －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－


LiveAiXin::~LiveAiXin() {
    if (m_bombAction) {
        CC_SAFE_DELETE(m_bombAction);
        m_bombAction = NULL;
    }
}

bool LiveAiXin::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    
    m_time_going = false;
    canRun = true;
    animationBool = false;
    m_score = 0;
    ciIndex = 0;
    startFolat = .5f;
    timeFloat = 1.5f;
    
    
    //动画
    CCArray* animations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .1f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "renwuStr");
    
    
    // back ground
    this->setupView();
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    overBool = false;
    
    garbages = CCArray::create();
    garbages->retain();
    
    
    m_number = 0;
    
    m_Isbomb = false;
    //    this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::schedules), 5.f);
    //    setAccelerometerEnabled(true);//打开重力感应
    
    this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::openTouch), 1.f);
    
    return true;
}

void LiveAiXin::openTouch(float dt){
    this->setTouchEnabled(true);
    setAccelerometerEnabled(true);//打开重力感应
}

void LiveAiXin::schedules(float dt)
{
    this->schedule(SEL_SCHEDULE(&LiveAiXin::crecteGarbage), .2f);
    this->schedule(SEL_SCHEDULE(&LiveAiXin::updateTime), 1.f);
    this->scheduleUpdate();
}

void LiveAiXin::onEnter()
{
    CCLayer::onEnter();
    
//    MMAudioManager::get_instance()->play_music(kMusic_BG_Lives, true);
    
}
void LiveAiXin::onExit()
{
    this->unscheduleAllSelectors();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCLayer::onExit();
}

void LiveAiXin::setupView()
{
    
    // background
    CCSprite * back = CCSprite::create("res/pic/game/laji/zjw_bg.png");
    back->setPosition(CCPointMake(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(back, -2);
    
    m_box = CCSprite::create("res/pic/game/laji/zjw_xiangzi1.png");
    m_box->setPosition(CCPointMake(DISPLAY->ScreenWidth()* .5f - 214, DISPLAY->ScreenHeight() * .1f));
    this->addChild(m_box, 0);
    
    m_hidden_box = CCSprite::create("res/pic/game/laji/zjw_xiangzi.png");
    m_hidden_box->setPosition(ccp(100, 85));
//    m_hidden_box->setColor(ccRED);
    m_box->addChild(m_hidden_box);
    
    m_animationSprite = CCSprite::create();
    m_animationSprite->setPosition(ccp(m_box->getContentSize().width* .5f, m_box->getContentSize().height* .5f));
    m_box->addChild(m_animationSprite);
    
    {
        //时间框
        CCSprite* timeSpr = CCSprite::create("res/pic/game/color/color_scorepanel.png");
        timeSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .85f));
        this->addChild(timeSpr, 15);
        
        //本次得分
        label_num = CCLabelTTF::create("0", DISPLAY->fangzhengFont(), 25);
        label_num->setPosition(ccp(timeSpr->getContentSize().width* .85f, timeSpr->getContentSize().height* .28f));
        label_num->setColor(ccWHITE);
        timeSpr->addChild(label_num, 20);
        
        //最高分
        CCString* sco_str = CCString::createWithFormat("%d", 99);
        CCLabelTTF* label_score = CCLabelTTF::create(sco_str->getCString(), DISPLAY->fangzhengFont(), 25);
        label_score->setPosition(ccp(timeSpr->getContentSize().width* .85f, timeSpr->getContentSize().height* .7f));
        label_score->setColor(ccWHITE);
        timeSpr->addChild(label_score, 20);
    }
    
    {
        CCSprite * tishi = CCSprite::create("res/pic/game/color/guide_prompt_plane.png");
        tishi->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .54f));
        this->addChild(tishi, 20, 0x121);
        CCSprite * tishi1 = CCSprite::create("res/pic/game/laji/games_yd_housework.png");
        tishi1->setPosition(ccp(tishi->getContentSize().width* .5f, tishi->getContentSize().height* .5f));
        tishi->addChild(tishi1,2);
        
        
        CCSprite* kSpr = CCSprite::create("res/pic/game/color/guide_kuang2.png");
        kSpr->setPosition(ccp(tishi->getContentSize().width* .65f, 0));
        tishi->addChild(kSpr, 3);
        
        CCLabelTTF* label = CCLabelTTF::create("点击任意处继续", DISPLAY->fangzhengFont(), 35);
        label->setPosition(ccp(kSpr->getContentSize().width* .5f, kSpr->getContentSize().height* .5f));
        label->setColor(ccc3(24, 21, 70));
        kSpr->addChild(label, 4);
    }

    CCAnimation* anim = AppUtil::animationWithPics("res/pic/game/laji/zjw_xiangzi%d.png", 3, 2, .8f);
    m_bombAction = CCAnimate::create(anim);
    m_bombAction->retain();
}

void LiveAiXin::starAnimation(){
    
    CCSprite* spr = CCSprite::create();
    spr->setTag(0x99999);
    this->addChild(spr, 100);
    
    CCSprite* renwuSpr1 = CCSprite::create("res/pic/game/color/a_star_s.png");
    renwuSpr1->setScale(.3f);
    renwuSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr1);
    
    CCSprite* renwuSpr2 = CCSprite::create("res/pic/game/color/a_star_t.png");
    renwuSpr2->setScale(.3f);
    renwuSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* .9f));
    spr->addChild(renwuSpr2, 1);
    
    CCSprite* renwuSpr3 = CCSprite::create("res/pic/game/color/a_star_a.png");
    renwuSpr3->setScale(.3f);
    renwuSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr3, 1);
    
    CCSprite* renwuSpr4 = CCSprite::create("res/pic/game/color/a_star_r.png");
    renwuSpr4->setScale(.3f);
    renwuSpr4->setPosition(ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* .9f));
    spr->addChild(renwuSpr4);
    
    CCSprite* renwuSpr5 = CCSprite::create("res/pic/game/color/a_star_t.png");
    renwuSpr5->setScale(.3f);
    renwuSpr5->setPosition(ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* .85f));
    spr->addChild(renwuSpr5);
    
    float heightFolat = .75f;
    
    CCMoveTo* moveTo1_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo2_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo3_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo4_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo5_1 = CCMoveTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* heightFolat));
    
    CCScaleTo* scaleTo1 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo2 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo3 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo4 = CCScaleTo::create(.2f, 1.f);
    CCScaleTo* scaleTo5 = CCScaleTo::create(.2f, 1.f);
    
    CCSpawn* spawn1 = CCSpawn::create(moveTo1_1, scaleTo1, NULL);
    CCSpawn* spawn2 = CCSpawn::create(moveTo2_1, scaleTo2, NULL);
    CCSpawn* spawn3 = CCSpawn::create(moveTo3_1, scaleTo3, NULL);
    CCSpawn* spawn4 = CCSpawn::create(moveTo4_1, scaleTo4, NULL);
    CCSpawn* spawn5 = CCSpawn::create(moveTo5_1, scaleTo5, NULL);
    
    CCMoveTo* moveTo1_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo2_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .45f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo3_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo4_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .55f, DISPLAY->ScreenHeight()* heightFolat));
    CCMoveTo* moveTo5_2 = CCMoveTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat));
    
    CCJumpTo* jumpTo1_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .3f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo1_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .2f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    CCJumpTo* jumpTo2_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .4f, DISPLAY->ScreenHeight()* heightFolat), 90.f, 2);
    CCJumpTo* jumpTo2_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .35f, DISPLAY->ScreenHeight()* heightFolat), 50.f, 1);
    CCJumpTo* jumpTo3_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat), 90.f, 2);
    CCJumpTo* jumpTo3_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* heightFolat), 50.f, 1);
    CCJumpTo* jumpTo4_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .6f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo4_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .65f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    CCJumpTo* jumpTo5_1 = CCJumpTo::create(.1f, ccp(DISPLAY->ScreenWidth()* .7f, DISPLAY->ScreenHeight()* heightFolat), 80.f, 1);
    CCJumpTo* jumpTo5_2 = CCJumpTo::create(.2f, ccp(DISPLAY->ScreenWidth()* .8f, DISPLAY->ScreenHeight()* heightFolat), 40.f, 2);
    
    CCFadeOut * out1 = CCFadeOut::create(1.f);
    CCFadeOut * out2 = CCFadeOut::create(1.f);
    CCFadeOut * out3 = CCFadeOut::create(1.f);
    CCFadeOut * out4 = CCFadeOut::create(1.f);
    CCFadeOut * out5 = CCFadeOut::create(1.f);
    CCCallFuncN *remove =  CCCallFuncN::create(this, callfuncN_selector(LiveAiXin::funCallback));
    
    CCSequence* seq1 = CCSequence::create(spawn1, CCDelayTime::create(.2f), moveTo1_2, CCDelayTime::create(.1f), jumpTo1_1, jumpTo1_2, out1, NULL);
    CCSequence* seq2 = CCSequence::create(spawn2, CCDelayTime::create(.2f), moveTo2_2, CCDelayTime::create(.1f), jumpTo2_1, jumpTo2_2, out2, NULL);
    CCSequence* seq3 = CCSequence::create(spawn3, CCDelayTime::create(.2f), moveTo3_2, CCDelayTime::create(.1f), jumpTo3_1, jumpTo3_2, out3, NULL);
    CCSequence* seq4 = CCSequence::create(spawn4, CCDelayTime::create(.2f), moveTo4_2, CCDelayTime::create(.1f), jumpTo4_1, jumpTo4_2, out4, NULL);
    CCSequence* seq5 = CCSequence::create(spawn5, CCDelayTime::create(.2f), moveTo5_2, CCDelayTime::create(.1f), jumpTo5_1, jumpTo5_2, out5, remove, NULL);
    
    renwuSpr1->runAction(seq1);
    renwuSpr2->runAction(seq2);
    renwuSpr3->runAction(seq3);
    renwuSpr4->runAction(seq4);
    renwuSpr5->runAction(seq5);
    
    // 播放倒计时
    if (!m_time_going) {
        m_time_going = true;
        //MMAudioManager::get_instance()->play_effect(kAudio_Read_Go, false);
    }
}

void LiveAiXin::callfuncN1(CCNode * sender)
{
    
    // 播放倒计时
    if (!m_time_going) {
        m_time_going = true;
        //MMAudioManager::get_instance()->play_effect(kAudio_Read_Go, false);
    }
    
}

void LiveAiXin::didAccelerate(CCAcceleration* pAccelerationValue)
{
    
    if (NULL != getChildByTag(26) || NULL != getChildByTag(25)) {
        return ;
    }
    
    CCDirector* pDir = CCDirector::sharedDirector();
    
    if ( m_box == NULL ) {
        return;
    }
    
    CCSize ballSize  = m_box->getContentSize();
    
    CCPoint ptNow  = m_box->getPosition();
    CCPoint ptTemp = pDir->convertToUI(ptNow);
    
    //9.8 重力加速度
    ptTemp.x += pAccelerationValue->x * 45.f;
    
    
    CCPoint ptNext = pDir->convertToGL(ptTemp);
    ptNext.x = MAX(MIN(DISPLAY->ScreenWidth() - ballSize.width / 2.0, ptNext.x), (0 + ballSize.width / 2.0));
    
    //    setplayerPositionX(ptNext.x);
    setplayerPositionX(pAccelerationValue->x * 120.f);
}

void LiveAiXin::setplayerPositionX(float x)
{
    //    m_box->setPositionX(x);
    
    m_box->stopAllActions();
    
    float posx = MAX(MIN(DISPLAY->ScreenWidth() - m_box->getContentSize().width / 2.0,m_box->getPositionX() + x), (0+m_box->getContentSize().width / 2.0));
    
    if (posx > m_box->getContentSize().width / 2.0 && posx < DISPLAY->ScreenWidth() - m_box->getContentSize().width / 2.0) {
        m_box->runAction(CCMoveBy::create(0.1f, ccp(x, 0)));
    }
}

void LiveAiXin::crecteGarbage(float dt)
{
    int type = (int)(CCRANDOM_0_1() * 10) % 8 + 1;
    Garbage * garbage = Garbage::create(type);
    
    float x = CCRANDOM_0_1() * (DISPLAY->ScreenWidth() * 2 / 3) + DISPLAY->ScreenWidth() / 10;
    garbage->setPositionY(DISPLAY->ScreenHeight() + garbage->getContentSize().height);
    garbage->setPositionX(x);
    
    if (m_score != ciIndex) {
        if( m_score <= 2 ){
            ciIndex = m_score;
            timeFloat = 1.5f;
        }else if( m_score < 4 ){
            ciIndex = m_score;
            timeFloat -= .2f;
        }else if( m_score < 79 ){
            ciIndex = m_score;
            if( m_score % 20 == 0 )
            {
                timeFloat -= .1f;
            }
        }else if( m_score < 119 ){
            ciIndex = m_score;
            if( m_score % 20 == 0 )
            {
                timeFloat -= .1f;
            }
        }else{
            if (m_score >= 0) {
                ciIndex = m_score;
                if( m_score % 20 == 0 )
                {
                    if (timeFloat <= .5f) {
                        timeFloat = .5f;
                    }else{
                        timeFloat -= .05f;
                    }
                }
            }else{
                timeFloat = .5f;
            }
        }
    }
    
    
    
    float duration = CCRANDOM_0_1() * startFolat + timeFloat;
    
    garbage->runAction(CCMoveTo::create(duration, ccp(x, -garbage->getContentSize().height)));
    garbages->addObject(garbage);
    this->addChild(garbage, 10);
}

void LiveAiXin::addcome()
{
    // garbage come
    CCSprite * come = CCSprite::create("res/pic/game/laji/go.png");
    CCFadeIn * in = CCFadeIn::create(1.f);
    CCFadeOut * out = CCFadeOut::create(2.f);
    CCCallFuncN *remove =  CCCallFuncN::create(come, callfuncN_selector(LiveAiXin::funCallback) );
    come->setPosition(ccp(DISPLAY->ScreenWidth() / 2, DISPLAY->ScreenHeight() * 3 / 5));
    this->addChild(come, 26, 26);
    come->runAction(CCSequence::create(in, out, remove, NULL));
}

void LiveAiXin::removeGarbage(Garbage * garbage)
{
    garbage->stopAllActions();
    garbages->removeObject(garbage);
    this->removeChild(garbage, true);
}

bool LiveAiXin::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (this->getChildByTag(0x121)) { // 触摸开始游戏
        if (!animationBool) {
            animationBool = true;
            this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::starAnimation), .5f);
        }
    }
    
    if (NULL != getChildByTag(26)) {
        return false;
    }
    
//    CCPoint point = pTouch->getLocation();
//    if (m_box->boundingBox().containsPoint(point)) {
//        return true;
//    }
    
    return true;
}


void LiveAiXin::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if(m_Isbomb) {
        return;
    }
    
    CCPoint point = pTouch->getLocation();
//    if (point.x >= DISPLAY->ScreenWidth() / 6 && point.x <= DISPLAY->ScreenWidth() * 5 / 6) {
        m_box->setPositionX(point.x);
//    }
    
}

void LiveAiXin::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void LiveAiXin::funCallback(CCNode * sender)
{
    if(sender && sender->getParent()){
        sender->removeFromParentAndCleanup(true);
    }
    if(this->getChildByTag(0x121))
    {
        this->removeChildByTag(0x121, true);
    }
    this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::schedules), 1.f);
}

void LiveAiXin::updateTime(float dt){
    
    
}

void LiveAiXin::funCallback1(CCNode * sender)
{
    if(sender && sender->getParent()){
        sender->removeFromParentAndCleanup(true);
    }
    
    this->addcome();
}

void LiveAiXin::update(float dt)
{
    CCObject * obj = NULL;
    
    CCARRAY_FOREACH(garbages, obj)
    {
        Garbage * garbage = (Garbage *)obj;
        if (!garbage) {
            continue;
        }
        if(garbage->getPositionY() < -1 * garbage->getContentSize().width/2)
        { this->removeGarbage(garbage);}
    }
    checkForCollision();
}

void LiveAiXin::checkForCollision()
{
    if (garbages->count() == 0) {
        return;
    }
    float boxWidth = m_hidden_box->getContentSize().width;
    Garbage * garbage =  (Garbage*)garbages->objectAtIndex(0);
    
    float garbageHeight =  garbage->getTexture()->getContentSize().height;
    
    float  boxCollisionRadians = boxWidth* .5f;
    float  garbageCollisionRandians = garbageHeight* .5f;
    float  MaxCollisionDistance = boxCollisionRadians + garbageCollisionRandians;
    
    
    CCObject * obj = NULL;
    CCARRAY_FOREACH(garbages, obj)
    {
        Garbage * garbage1 = (Garbage *)obj;
        if (!obj ) {
            continue;
        }
        
        CCPoint pot = m_box->convertToWorldSpace(m_hidden_box->getPosition());
        CCPoint boxPoint = this->convertToNodeSpace(pot) ;
        CCPoint garbagePoint = garbage1->getPosition();
        
        float actualDistance = ccpDistance(boxPoint, garbagePoint);
        if( actualDistance <= MaxCollisionDistance)
        {
            //执行动画
            //MZLog("canRun  %d",canRun);
            if(garbage1->getResult() != -3)
            {
//                if (MMAudioManager::get_instance()->is_effect_on()) {
//                    MMAudioManager::get_instance()->play_effect(kAudio_game_right, false);
//                }
                
                if (canRun) {
                    this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::runAnimate), 0.f);
                }
                m_score++;
                label_num->setString((CCString::createWithFormat("%d",m_score))->getCString());
            }
            if(garbage1->getResult() == -3)
            {
//                if (MMAudioManager::get_instance()->is_effect_on()) {
//                    MMAudioManager::get_instance()->play_effect(kAudio_game_wrong, false);
//                }
                
                m_animationSprite->stopAllActions();
                m_animationSprite->runAction(m_bombAction);
                
                m_score--;
                if (m_score < 0) {
                    m_score = 0;
                }
                label_num->setString((CCString::createWithFormat("%d",m_score))->getCString());
                
                m_Isbomb = true;
                setAccelerometerEnabled(false);
                this->scheduleOnce(SEL_SCHEDULE(&LiveAiXin::resume), 1.f);
            }
            
            
            m_number += garbage1->getResult();
            removeGarbage(garbage1);
        }
    }
}

void LiveAiXin::resume(float dt)
{
    m_Isbomb = false;
    setAccelerometerEnabled(true);
}

void LiveAiXin::gameOver()
{
    
    
    
}

void LiveAiXin::runAnimate(CCObject* pSender)
{
    CCCallFunc* close = CCCallFunc::create(this, SEL_CallFunc(&LiveAiXin::closeRun));
    CCCallFunc* open = CCCallFunc::create(this, SEL_CallFunc(&LiveAiXin::openRun));
    CCScaleTo* down = CCScaleTo::create(0.1, 1.1, 0.7);
    CCScaleTo* up = CCScaleTo::create(0.06, 1.f, 1.f);
    CCSequence* sequence = CCSequence::create(close, down, up, open, NULL);
    m_box->runAction(sequence);
}

void LiveAiXin::closeRun(){
    canRun = false;
}

void LiveAiXin::openRun(){
    canRun = true;
}
