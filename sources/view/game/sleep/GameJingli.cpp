//
//  GameJingli.cpp
//  mm3c
//
//  Created by lakkey on 15-12-19.
//
//

#include "GameJingli.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "NetManager.h"
#include "ConfigManager.h"
#include "GhostSystem.h"
#include "GameCheckoutPanel.h"
#include "Loading2.h"

const int LOST_LIMIT = 5;
const char* GAME_ID = "3";

GameJingli::~GameJingli() {

}

bool GameJingli::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    CCSprite* bg = CCSprite::create("res/pic/game/sleep/sj_bg_sleep.png");
    bg->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
    this->addChild(bg);
    
    _gamestate = e_gamestate_idle;
//    _timeleft = def_GameTime;
    _create_ghost_interval = 0.f;
    
    return true;
}

void GameJingli::onEnter() {
    CCLayer::onEnter();
    
//    MMAudioManager::get_instance()->play_music(kMusic_BG_Lives, true);
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&GameJingli::nc_commit_game_707), "HTTP_FINISHED_707", NULL);
    
    this->init_UI();
    this->show_game_help();
    
    _ghostlayer = GhostSystem::create();
    this->addChild(_ghostlayer);
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    this->scheduleUpdate();
}

void GameJingli::onExit() {
    
    CCLayer::onExit();
}

void GameJingli::init_UI() {
    CCLayer* layer = CCLayer::create();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/game/sleep/sj_sleep_pack.plist");
    
    // 睡觉动画
    CCArray* arr_frame = CCArray::create();
    for (int i = 0; i < 4; ++i) {
        CCString* str = CCString::createWithFormat("sj_sleep_%d.png", i);
        CCSpriteFrame* fm = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(str->getCString());
        arr_frame->addObject(fm);
    }
    
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(arr_frame, .3f);
    CCActionInterval* act_up = CCAnimate::create(animation);
    CCActionInterval* act_down = act_up->reverse();
    CCSequence* seq = CCSequence::create(act_up, CCDelayTime::create(.3f), act_down, NULL);
    CCAction* action = CCRepeatForever::create(seq);
    CCSprite* spt_sleep = CCSprite::createWithSpriteFrameName("sj_sleep_0.png");
    spt_sleep->runAction(action);
    spt_sleep->setPosition(ccp(DISPLAY->ScreenWidth()* .5f + 273.f, DISPLAY->ScreenHeight()* .5f - 13.f));
    layer->addChild(spt_sleep);
    
    CCDictionary* scores = DATA->getHome()->getScores();
    
    //时间框
    CCSprite* timeSpr = CCSprite::create("res/pic/game/color/color_scorepanel.png");
    timeSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .85f));
    this->addChild(timeSpr, 1);
    
//    //倒计时
//    _lbl_time = CCLabelTTF::create("00:00", DefaultFont, 45);
//    _lbl_time->setColor(customColor);
//    _lbl_time->setPosition(ccp(timeSpr->getContentSize().width* .19f ,timeSpr->getContentSize().height* .3f));
//    timeSpr->addChild(_lbl_time);
    
    //本次得分
    _lbl_score = CCLabelTTF::create("0", DISPLAY->fangzhengFont(), 25);
    _lbl_score->setPosition(ccp(timeSpr->getContentSize().width* .85f ,timeSpr->getContentSize().height* .25f));
    _lbl_score->setColor(ccWHITE);
    timeSpr->addChild(_lbl_score, 20);
    
    // 历史最高分
    CCString* sco_str = CCString::createWithFormat("%d", ((CCInteger*)scores->objectForKey("3"))->getValue());
    CCLabelTTF* label_score = CCLabelTTF::create(sco_str->getCString(), DISPLAY->fangzhengFont(), 25);
    label_score->setColor(ccWHITE);
    label_score->setPosition(ccp(timeSpr->getContentSize().width* .85f ,timeSpr->getContentSize().height* .72f));
    timeSpr->addChild(label_score, 20);
    
    CCString* wrongStr = CCString::createWithFormat("%d/%d", 0, LOST_LIMIT);
    _Worng_word = CCLabelTTF::create(wrongStr->getCString(), DISPLAY->fangzhengFont(), 30);
    _Worng_word->setColor(ccWHITE);
    _Worng_word->setPosition(ccp(timeSpr->getContentSize().width* .2f ,timeSpr->getContentSize().height* .27f));
    timeSpr->addChild(_Worng_word, 20);
    
    this->addChild(layer);
}

void GameJingli::show_game_help() {
    CCSprite * tishi = CCSprite::create("res/pic/game/color/guide_prompt_plane.png");
    tishi->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .54f));
    this->addChild(tishi, 20, 0x121);
    CCSprite * tishi1 = CCSprite::create("res/pic/game/sleep/sj_yd_sleep.png");
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

void GameJingli::starAnimation() {
    
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
    CCCallFuncN *remove =  CCCallFuncN::create(this, callfuncN_selector(GameJingli::funCallback));
    
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
}

void GameJingli::funCallback(CCNode* pSender) {
    if(pSender && pSender->getParent()){
        pSender->removeFromParentAndCleanup(true);
    }
    if(this->getChildByTag(0x121))
    {
        this->removeChildByTag(0x121, true);
    }
    
//    this->schedule(schedule_selector(GameJingli::updateTime), 1.f);
    _gamestate = e_gamestate_ongoing;
}

bool GameJingli::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (_gamestate == e_gamestate_idle) {
        _gamestate = e_gamestate_onstart;
    }
    else if (_gamestate == e_gamestate_ongoing) {
        _ghostlayer->on_hit(pTouch->getLocation());
    }
    
    return true;
}

void GameJingli::update(float dt) {
    if (_gamestate == e_gamestate_ongoing) {
        this->check_end();
    }
    
    if (_gamestate == e_gamestate_onstart) {
        _gamestate = e_gamestate_starting;
        this->starAnimation();
    }
    else if (_gamestate == e_gamestate_ongoing) {
        this->create_ghost(dt);
        this->update_UI();
    }
    else if (_gamestate == e_gamestate_onend) {
        this->update_UI();
        this->game_over();
    }
}

void GameJingli::check_end() {
    if (_ghostlayer->getLostCount() >= 5) {
        _gamestate = e_gamestate_onend;
    }
}

void GameJingli::game_over() {
    this->unscheduleAllSelectors();
    LOADING->show_loading();
    NET->commit_game_707("3", _ghostlayer->getTotal_score());
}


void GameJingli::create_ghost(float dt) {
//    _timeleft -= dt;
    int randCount = (int)(CCRANDOM_0_1() * 2);
    _create_ghost_interval += dt;

//    if(_timeleft >= 15)
//    {
//        if (_create_ghost_interval >= .8f) {
//            for (int i = randCount; i >= 0; i--) {
//                _ghostlayer->create_ghost();
//            }
//            
//            _create_ghost_interval = 0.f;
//        }
//    }
//    else if(_timeleft >= 10) {
//        if (_create_ghost_interval >= .6f && _create_ghost_interval < .8f) {
//            for (int i = randCount; i >= 0; i--) {
//                _ghostlayer->create_ghost();
//            }
//            
//            _create_ghost_interval = 0.f;
//        }
//    }
//    else if(_timeleft > 0) {
//        if (_create_ghost_interval >= .5f && _create_ghost_interval < .7f) {
//            for (int i = randCount; i >= 0; i--) {
//                _ghostlayer->create_ghost();
//            }
//            
//            _create_ghost_interval = 0.f;
//        }
//    }
    
    if(_ghostlayer->getTotal_score() >= 0)
    {
        if (_create_ghost_interval >= .8f) {
            for (int i = randCount; i >= 0; i--) {
                _ghostlayer->create_ghost();
            }
            
            _create_ghost_interval = 0.f;
        }
    }
    else if(_ghostlayer->getTotal_score() >= 10) {
        if (_create_ghost_interval >= .6f && _create_ghost_interval < .8f) {
            for (int i = randCount; i >= 0; i--) {
                _ghostlayer->create_ghost();
            }
            
            _create_ghost_interval = 0.f;
        }
    }
    else if(_ghostlayer->getTotal_score() > 25) {
        if (_create_ghost_interval >= .5f && _create_ghost_interval < .7f) {
            for (int i = randCount; i >= 0; i--) {
                _ghostlayer->create_ghost();
            }
            
            _create_ghost_interval = 0.f;
        }
    }
}

void GameJingli::update_UI() {
//    int left_time = (int)_timeleft;
    _lbl_score->setString(CCString::createWithFormat("%d", _ghostlayer->getTotal_score())->getCString());
    _Worng_word->setString(CCString::createWithFormat("%d/%d", _ghostlayer->getLostCount(), LOST_LIMIT)->getCString());
}

void GameJingli::nc_commit_game_707(CCObject *pObj) {
    LOADING->remove();
    CCDictionary* first = (CCDictionary*)pObj;
    GameCheckoutPanel::show(this->getScene(), "3", _ghostlayer->getTotal_score(), first);
}
