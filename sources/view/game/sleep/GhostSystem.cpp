//
//  GhostSystem.cpp
//  mm3c
//
//  Created by lakkey on 15-12-19.
//
//

#include "GhostSystem.h"
#include "DisplayManager.h"
#include "DataManager.h"
#include "ConfigManager.h"
#include "AppUtil.h"
#include "AudioManager.h"


GhostSystem::~GhostSystem() {
    if (arr_ghost) {
        CC_SAFE_DELETE(arr_ghost);
        arr_ghost = NULL;
    }
    if (arr_sprite) {
        CC_SAFE_DELETE(arr_sprite);
        arr_sprite = NULL;
    }
}

bool GhostSystem::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    _total_score = 0;
    _lostCount = 0;
    
    arr_ghost = CCArray::create();
    arr_ghost->retain();
    arr_sprite = CCArray::create();
    arr_sprite->retain();
    // 丢弃前面4个值
    for (int i = 0; i < 4; i++) {
        this->rand_ghosttype();
    }
    
    return true;
}

void GhostSystem::create_ghost() {
    GHOSTTYPE type = this->rand_ghosttype();
    Ghost* ghost = this->_create_ghost(type);
    CCSprite* spt = this->create_ghost_sprite(ghost);
    this->addChild(spt, ghost->orderZ);
    if (type == e_ghosttype_flickering) {
        CCSequence* fade = CCSequence::create(CCDelayTime::create(0.8f), CCFadeOut::create(0.6), CCDelayTime::create(0.8f), CCFadeIn::create(0.6f), CCDelayTime::create(0.8f), CCFadeOut::create(0.6), CCDelayTime::create(0.8f), CCFadeIn::create(0.6f), NULL);
        spt->runAction(CCSpawn::createWithTwoActions(this->random_ghost_route(), fade));
    }
    else {
        spt->runAction(CCSpeed::create(this->random_ghost_route(), ghost->speed));
    }

    
    arr_ghost->addObject(ghost);
    arr_sprite->addObject(spt);
}


GHOSTTYPE GhostSystem::rand_ghosttype() {
    float rand_num = CCRANDOM_0_1();
    if (rand_num < 0.1) {
        return e_ghosttype_meatshield;
    }
    else if (rand_num < 0.6) {
        return e_ghosttype_normal;
    }
    else if (rand_num < 0.88) {
        return e_ghosttype_agile;
    }
    else {
        return e_ghosttype_flickering;
    }
}

Ghost* GhostSystem::_create_ghost(GHOSTTYPE type) {
    Ghost* rtn = Ghost::create();
    rtn->type = type;
    rtn->orderZ = type;
    
    if (type == e_ghosttype_meatshield) {
        rtn->speed = 0.2f;
        rtn->scale = 1.6f;
        rtn->health = 2;
        rtn->score = 3;
    }
    else if (type == e_ghosttype_flickering) {
        rtn->speed = 0.8f;
        rtn->scale = 0.8f;
        rtn->health = 1;
        rtn->score = 5;
    }
    else if (type == e_ghosttype_normal) {
        rtn->speed = 0.8f;
        rtn->scale = 0.8f;
        rtn->health = 1;
        rtn->score = 1;
    }
    else if (type == e_ghosttype_agile) {
        rtn->speed = 2.0f;
        rtn->scale = 0.4f;
        rtn->health = 1;
        rtn->score = 2;
    }
    
    return rtn;
}

CCSprite* GhostSystem::create_ghost_sprite(Ghost *ghost) {
    CCSprite* rtn = CCSprite::create("res/pic/game/sleep/sj_cat_flew.png");
    rtn->setPosition(ccp(-200, -200));
    rtn->setScale(ghost->scale);
    return rtn;
}

CCSequence* GhostSystem::random_ghost_route()
{
    float start_x = DISPLAY->ScreenWidth() * (-0.3f);
    float start_y = DISPLAY->ScreenHeight() * (0.1 + 0.7 * CCRANDOM_0_1());
    
    float ctl_1_x = DISPLAY->ScreenWidth() * .33f;;
    float ctl_1_y = DISPLAY->ScreenHeight() * (0.1 + 0.7 * CCRANDOM_0_1());
    
    float ctl_2_x = DISPLAY->ScreenWidth() * .66f;
    float ctl_2_y = DISPLAY->ScreenHeight() * (0.1 + 0.7 * CCRANDOM_0_1());
    
    float end_x = DISPLAY->ScreenWidth() * 1.3f;
    float end_y = DISPLAY->ScreenHeight() * (0.1 + 0.7 * CCRANDOM_0_1());
    
    float f = CCRANDOM_0_1();
    
    CCCallFuncN* on_escape = CCCallFuncN::create(this, SEL_CallFuncN(&GhostSystem::on_ghost_escape));
    if (f < .5f) {
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(ctl_1_x, ctl_1_y);
        bezier.controlPoint_2 = ccp(ctl_2_x, ctl_2_y);
        bezier.endPosition = ccp(end_x, end_y);
        CCSequence* seq = CCSequence::create(CCHide::create(), CCFlipX::create(true), CCPlace::create(ccp(start_x, start_y)), CCShow::create(), CCBezierTo::create(4.f, bezier), on_escape, NULL);
        return seq;
    }
    else {
        ccBezierConfig bezier;
        bezier.controlPoint_1 = ccp(ctl_2_x, ctl_2_y);
        bezier.controlPoint_2 = ccp(ctl_1_x, ctl_1_y);
        bezier.endPosition = ccp(start_x, start_y);
        CCSequence* seq = CCSequence::create(CCHide::create(), CCPlace::create(ccp(end_x, end_y)), CCShow::create(), CCBezierTo::create(4.f, bezier), on_escape, NULL);
        return seq;
    }
}

void GhostSystem::on_hit(cocos2d::CCPoint pos) {
    CCLOG("hit on x=%f, y=%f", pos.x, pos.y);
    int count = arr_sprite->count();
    bool hit = false;
    for (int type = e_ghosttype_meatshield; type > 0; type--) {
        for (int i = count - 1; i >= 0; --i) {
            Ghost* ghost = (Ghost* )arr_ghost->objectAtIndex(i);
            CCSprite* spt = (CCSprite* )arr_sprite->objectAtIndex(i);
            CCRect box = spt->boundingBox();
            if (ghost->type == type && box.containsPoint(pos)) {
                hit = this->hit_ghost(i, ghost, spt);
                if (hit) {
                    break;
                }
            }
        }
        if (hit) {
            break;
        }
    }
}

bool GhostSystem::hit_ghost(int idx, Ghost* ghost, CCSprite* spt) {
    if (ghost->type == e_ghosttype_flickering and spt->getOpacity() < 250) {
        CCLOG("Not hitting ......");
        return false;
    }
    
    ghost->health -= 1;
    if (ghost->health > 0) {
        spt->runAction(CCBlink::create(0.1, 1));
    }
    else {
//        if (MMAudioManager::get_instance()->is_effect_on()) {
//            MMAudioManager::get_instance()->play_effect(kAudio_game_wrong, false);
//        }
        AUDIO->wrong_effect();
        
        CCSprite* spt_dying = CCSprite::create("res/pic/game/sleep/sj_cat_catched.png");
        spt_dying->setFlipX(spt->isFlipX());
        spt_dying->setScale(spt->getScale());
        spt_dying->setPosition(spt->getPosition());
        
        this->addChild(spt_dying);
        CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&GhostSystem::purge_after_dyinganimation));
        CCSequence* seq = CCSequence::create(CCFadeOut::create(.5f), purge, NULL);
        spt_dying->runAction(seq);
        
        // record score
        _total_score += ghost->score;
        
        // clean
        arr_ghost->removeObjectAtIndex(idx);
        
        spt->removeFromParentAndCleanup(true);
        spt->stopAllActions();
        arr_sprite->removeObjectAtIndex(idx);
    }
    
    return true;
}

void GhostSystem::purge_after_dyinganimation(CCNode* node) {
    CCSprite* spt = (CCSprite* )node;
    spt->removeFromParentAndCleanup(true);
}

void GhostSystem::on_ghost_escape(cocos2d::CCNode *node) {
//    int new_score = _total_score - 1;
//    _total_score = MAX(0, new_score);
    _lostCount++;
}
