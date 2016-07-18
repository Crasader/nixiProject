//
//  PromptLayer.cpp
//  mm3c
//
//  Created by mac on 15-7-6.
//
//

#include "PromptLayer.h"
#include "DisplayManager.h"

#define PROMPTNAME    "res/pic/prompt.png"
#define TEXTCOLOR     ccBLACK

PromptLayer::PromptLayer(){
    
}

PromptLayer::~PromptLayer(){
    
}

bool PromptLayer::init(){
    if (!CCLayer::init()) {
        return false;
    }
    
    
    return true;
}

CCScene* PromptLayer::scene(){
    CCScene* scene = CCScene::create();
    PromptLayer* layer = PromptLayer::create();
    scene->addChild(layer);
    return scene;
}

void PromptLayer::promptBox(int idx){
    CCSprite* box = CCSprite::create(PROMPTNAME);
    CCSize box_size = box->boundingBox().size;
//    box->setScale(DISPLAY->ScreenWidth() / box_size.width) ;
    box->setPosition(DISPLAY->center());
    box->setTag(0x33333);
    this->addChild(box, 55);
    
    CCSequence* boxSeq = CCSequence::create(CCMoveBy::create(0.3f, ccp(0, 44.0f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    box->runAction(boxSeq);
    
    CCString* str;
    if (idx == 1) {
        str = CCString::create("");
    }

    CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 40.f);
    lbl->setAnchorPoint(ccp(.5f, .5f));
    lbl->setPosition(DISPLAY->center());
    lbl->setColor(ccc3(222, 222, 222));
    this->addChild(lbl, 56);
    
    CCSequence* lblSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    lbl->runAction(lblSeq);
}

void PromptLayer::promptBoxString(CCString* str){
    CCSprite* box = CCSprite::create(PROMPTNAME);
    CCSize box_size = box->boundingBox().size;
//    box->setScale(DISPLAY->ScreenWidth() / box_size.width) ;
    box->setPosition(DISPLAY->center());
    box->setTag(0x33333);
    this->addChild(box, 55);
    
    CCSequence* boxSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    box->runAction(boxSeq);
    
    CCLabelTTF* lbl = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 40.f);
    lbl->setAnchorPoint(ccp(.5f, .5f));
    lbl->setPosition(DISPLAY->center());
    lbl->setColor(TEXTCOLOR);
    this->addChild(lbl, 56);
    
    CCSequence* lblSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    lbl->runAction(lblSeq);
}

void PromptLayer::promptBoxSprite(){
    CCSprite* box = CCSprite::create(PROMPTNAME);
    CCSize box_size = box->boundingBox().size;
//    box->setScale(DISPLAY->ScreenWidth() / box_size.width) ;
    box->setPosition(DISPLAY->center());
    box->setTag(0x33333);
    this->addChild(box, 55);
    
    CCSequence* boxSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    box->runAction(boxSeq);
    
    CCSprite* xiuSpr = CCSprite::create(PROMPTNAME);
    xiuSpr->setScale(.85f);
    xiuSpr->setPosition(DISPLAY->center());
    this->addChild(xiuSpr, 56);
    
    CCSequence* xiuSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    xiuSpr->runAction(xiuSeq);
}

void PromptLayer::show_prompt(CCNode* parent, const char* str){
    CCSprite* box = CCSprite::create(PROMPTNAME);
    CCSize box_size = box->boundingBox().size;
//    box->setScale(DISPLAY->ScreenWidth() / box_size.width) ;
    box->setPosition(DISPLAY->center());
    box->setTag(0x33333);
    this->addChild(box, 55);
    
    CCSequence* boxSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), NULL);
    box->runAction(boxSeq);
    
    CCLabelTTF* lbl = CCLabelTTF::create(str, DISPLAY->fangzhengFont(), 40.f);
    lbl->setAnchorPoint(ccp(.5f, .5f));
    lbl->setPosition(DISPLAY->center());
    lbl->setColor(TEXTCOLOR);
    this->addChild(lbl, 56);
    
    parent->addChild(this);
    
    CCCallFunc* remove = CCCallFunc::create(this, SEL_CallFunc(&PromptLayer::removeself));
    CCSequence* lblSeq = CCSequence::create(CCMoveBy::create(.3f, ccp(0, 44.f)), CCShow::create(), CCFadeOut::create(2.f), remove, NULL);
    lbl->runAction(lblSeq);
}

void PromptLayer::removeself() {
    CCLOG("PromptLayer::removeself() ...");
    this->removeFromParentAndCleanup(true);
}