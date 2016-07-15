//
//  SpecialManager.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "SpecialManager.h"
#include "AppUtil.h"

static SpecialManager* _instance = nullptr;

SpecialManager::~SpecialManager() {
    CC_SAFE_DELETE(_specials);
}

SpecialManager* SpecialManager::Inst() {
    if (_instance == nullptr) {
        _instance = new SpecialManager();
    }
    
    return _instance;
}

void SpecialManager::init() {
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/star.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/star2.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/petal.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/petal2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/spot.plist");
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("res/pic/special/flower.plist");
    
    _specials = CCDictionary::create();
    _specials->retain();
    
//    CCAnimation* star = AppUtil::animationWithFrame("star_%d.png", 30, 1, 0.1f);
//    _specials->setObject(star, "star");
    
    CCAnimation* star2 = AppUtil::animationWithFrame("star2_%d.png", 15, 1, 0.1f);
    _specials->setObject(star2, "star2");
    
//    CCAnimation* petal = AppUtil::animationWithFrame("petal_%d.png", 24, 1, 0.1f);
//    _specials->setObject(petal, "petal");
    
    CCAnimation* petal2 = AppUtil::animationWithFrame("petal2_%d.png", 52, 1, 0.1f);
    _specials->setObject(petal2, "petal2");
    
    CCAnimation* spot = AppUtil::animationWithFrame("spot_%d.png", 40, 1, 0.01f);
    _specials->setObject(spot, "spot");
    
    CCAnimation* flower = AppUtil::animationWithFrame("flower_%d.png", 10, 1, 0.07f);
    _specials->setObject(flower, "flower");
}

//void SpecialManager::showStarAt(CCNode* parent, CCPoint pos, int zOrder) {
//    CCSprite* star = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("star_1.png"));
//    star->setPosition(pos);
//    parent->addChild(star, zOrder);
//    
//    CCActionInterval* anim = CCAnimate::create(animationByName("star"));
//    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
//    CCSequence* act = CCSequence::create(anim, purge, NULL);
//    star->runAction(act);
//}

void SpecialManager::showStar2At(CCNode* parent, CCPoint pos, int zOrder) {
    CCSprite* star2 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("star2_1.png"));
    star2->setPosition(pos);
    star2->setScale(1.3f);
    parent->addChild(star2, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("star2"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    star2->runAction(act);
}

//void SpecialManager::showPetalAt(CCNode* parent, CCPoint pos, int zOrder) {
//    CCSprite* petal = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("petal_1.png"));
//    petal->setPosition(pos);
//    parent->addChild(petal, zOrder);
//    
//    CCActionInterval* anim = CCAnimate::create(animationByName("petal"));
//    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
//    CCSequence* act = CCSequence::create(anim, purge, NULL);
//    petal->runAction(act);
//}

void SpecialManager::showPetal2At(CCNode* parent, CCPoint pos, int zOrder) {
    CCSprite* petal2 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("petal2_1.png"));
    petal2->setPosition(pos);
    parent->addChild(petal2, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("petal2"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    petal2->runAction(act);
}

void SpecialManager::showSpotAt(CCNode *parent, CCPoint pos, int zOrder) {
    CCSprite* star = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("spot_1.png"));
    star->setScale(1.8f);
    star->setPosition(pos);
    parent->addChild(star, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("spot"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    star->runAction(act);
}

void SpecialManager::showFlowerAt(CCNode *parent, CCPoint pos, int zOrder) {
    {
    CCSprite* flower1 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("flower_1.png"));
    flower1->setPosition(pos + ccp(-50, 20));
    parent->addChild(flower1, zOrder);
    CCSequence* act1 = CCSequence::create(CCAnimate::create(animationByName("flower")), CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode)), NULL);
    flower1->runAction(act1);
    }{
    CCSprite* flower2 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("flower_1.png"));
    flower2->setPosition(pos + ccp(50, 20));
    flower2->setFlipX(true);
    parent->addChild(flower2, zOrder);
    CCSequence* act2 = CCSequence::create(CCAnimate::create(animationByName("flower")), CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode)), NULL);
    flower2->runAction(act2);
    }{
    CCSprite* flower3 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("flower_1.png"));
    flower3->setPosition(pos + ccp(-50, -20));
    flower3->setFlipY(true);
    parent->addChild(flower3, zOrder);
    CCSequence* act3 = CCSequence::create(CCAnimate::create(animationByName("flower")), CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode)), NULL);
    flower3->runAction(act3);
    }{
    CCSprite* flower4 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("flower_1.png"));
    flower4->setPosition(pos + ccp(50, -20));
    flower4->setFlipX(true);
    flower4->setFlipY(true);
    parent->addChild(flower4, zOrder);
    CCSequence* act4 = CCSequence::create(CCAnimate::create(animationByName("flower")), CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode)), NULL);
    flower4->runAction(act4);
    }
}

void SpecialManager::show_coin_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration) {
    int zOrder = 1000;
    // icon
    CCSprite* icon = CCSprite::create("pic/clothesScene/gj_coin.png");
    icon->setPosition(start);
    CCAnimate* flap = CCAnimate::create(AppUtil::animationWithPics("pic/common/fly_coin_%d.png", 2, 1, 0.3f));
    icon->runAction(CCRepeatForever::create(flap));
    parent->addChild(icon, zOrder);
    
    CCLabelAtlas* lbl = NULL;
    if (num > 0) {
        // num
        CCString* str_num = CCString::createWithFormat("%d", num);
        lbl = CCLabelAtlas::create(str_num->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        parent->addChild(lbl, zOrder);
    }
    
    CCCallFuncN* complete = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::coin_animation_completed));
    this->reward_display(icon, lbl, end, complete, duration);
}

void SpecialManager::show_gold_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration) {
    int zOrder = 1000;
    // gold
    CCSprite* icon = CCSprite::create("pic/clothesScene/gj_gold.png");
    icon->setPosition(start);
    CCAnimate* flap = CCAnimate::create(AppUtil::animationWithPics("pic/common/fly_gold_%d.png", 2, 1, 0.3f));
    icon->runAction(CCRepeatForever::create(flap));
    parent->addChild(icon, zOrder);
    
    CCLabelAtlas* lbl = NULL;
    if (num > 0) {
        // num
        CCString* str_num = CCString::createWithFormat("%d", num);
        CCLabelAtlas* lbl = CCLabelAtlas::create(str_num->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        parent->addChild(lbl, zOrder);
    }
    
    CCCallFuncN* complete = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::gold_animation_completed));
    this->reward_display(icon, lbl, end, complete, duration);
}

void SpecialManager::show_energy_reward(CCNode *parent, int num, CCPoint start, CCPoint end, float duration) {
    int zOrder = 1000;
    // piece
    CCSprite* energy = CCSprite::create("pic/clothesScene/gj_xin.png");
    energy->setPosition(start);
    CCAnimate* flap = CCAnimate::create(AppUtil::animationWithPics("pic/common/fly_energy_%d.png", 2, 1, 0.3f));
    energy->runAction(CCRepeatForever::create(flap));
    parent->addChild(energy, zOrder);
    
    CCLabelAtlas* lbl = NULL;
    if (num > 0) {
        // num
        CCString* str_num = CCString::createWithFormat("%d", num);
        CCLabelAtlas* lbl = CCLabelAtlas::create(str_num->getCString(), "pic/baseScene/base_number.png", 14, 20, '0');
        parent->addChild(lbl, zOrder);
    }
    
    CCCallFuncN* complete = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::energy_animation_completed));
    this->reward_display(energy, lbl, end, complete, duration);
}

#pragma - Inner

CCAnimation* SpecialManager::animationByName(const char* name) {
    return (CCAnimation*)_specials->objectForKey(name);
}

void SpecialManager::purgeActionNode(CCNode *node) {
    if (node) {
        node->removeFromParentAndCleanup(true);
    }
}

void SpecialManager::reward_display(CCNode* icon, CCNode* num, CCPoint end, CCCallFuncN* complete, float duration) {
    CCPoint start = icon->getPosition();
    
    CCActionInterval* icon_fly = this->fly_action(start, end, duration);
    CCCallFuncN* flower = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::show_flower));
    icon->runAction(CCSequence::create(icon_fly, flower, CCDelayTime::create(0.4f), complete, NULL));
    
    if (num != NULL) {
        num->setPosition(icon->getPosition() - ccp(50, 0));
        CCActionInterval* num_fly = this->fly_action(start, end * 0.8, duration);
        CCFadeOut* num_fadeout = CCFadeOut::create(duration);
        CCScaleTo* num_scale = CCScaleTo::create(duration, 1.6);
        num->runAction(CCSpawn::create(num_fly, num_fadeout, num_scale, NULL));
    }
}

CCActionInterval* SpecialManager::fly_action(CCPoint start, CCPoint end, float duration) {
    float delta_height = end.y - start.y;
    float delta_width = end.x - start.x;
    
    ccBezierConfig conf;
    conf.endPosition = end;
    if (delta_width > 0) {
        conf.controlPoint_1 = start + ccp(-80, delta_height * 0.2);
        conf.controlPoint_2 = start + ccp(-50, delta_height * 0.66);
    }
    else {
        conf.controlPoint_1 = start + ccp(80, delta_height * 0.2);
        conf.controlPoint_2 = start + ccp(50, delta_height * 0.66);
    }
    CCBezierTo* bezie = CCBezierTo::create(duration, conf);
    return bezie;
}

void SpecialManager::show_flower(CCNode* node) {
    showStarParticleEffect(1, node->getPosition(), node->getParent());
}

void SpecialManager::coin_animation_completed(CCNode* node) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("COIN_FLY_COMPLETED");
    //
    node->stopAllActions();
    node->removeFromParentAndCleanup(true);
}

void SpecialManager::gold_animation_completed(CCNode* node) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("GOLD_FLY_COMPLETED");
    //
    node->stopAllActions();
    node->removeFromParentAndCleanup(true);
}

void SpecialManager::energy_animation_completed(CCNode* node) {
    CCNotificationCenter::sharedNotificationCenter()->postNotification("ENERGY_FLY_COMPLETED");
    //
    node->stopAllActions();
    node->removeFromParentAndCleanup(true);
}

// 星星爆炸效果
void SpecialManager::showStarParticleEffect(int color, CCPoint position, CCNode* node){
    //	CCParticleExplosion* effect = CCParticleExplosion::create();
    CCParticleFlower* effect = CCParticleFlower::create();
    effect->setTexture(CCTextureCache::sharedTextureCache()->addImage("pic/common/ah_star.png"));
    effect->setTotalParticles(30);//最大粒子个数
    effect->setStartColor({0.5f,0.5f,0.5f,0.5f});//起始颜色
    effect->setStartColorVar({0.5f,0.5f,0.5f,1.0f});//起始颜色浮动值
    effect->setEndColor({0.1f,0.1f,0.1f,0.2f});//结束颜色
    effect->setEndColorVar({0.1f,0.1f,0.1f,0.2f}); //结束颜色浮动值
    effect->setStartSize(25.f);//起始大小
    effect->setStartSizeVar(15.f);
    effect->setGravity(CCPoint(0,-20));//动力加速
    effect->setDuration(.3f);//粒子发射器持续时间，-1为永久
    effect->setLife(.5f);//生存时间
    effect->setSpeed(100);//粒子速度
    effect->setSpeedVar(10);//粒子速度浮动值
    effect->setPosition(position);//发射器位置
    node->addChild(effect, 1000);
}

ccColor4F SpecialManager::getColor4F(int color){
    switch(color){
        case 1:
            return {144/255.0f,238/255.0f,229/255.0f,1};
        case 2:
            return {255/255.0f,251/255.0f,233/255.0f,1};
        case 3:
            return {254/255.0f,253/255.0f,144/255.0f,1};
        case 4:
            return {255/255.0f,188/255.0f,191/255.0f,1};
        case 5:
            return {224/255.0f,184/255.0f,234/255.0f,1};
    }
    return {1,1,1,0};
}
