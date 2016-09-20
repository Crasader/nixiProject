//
//  Signin7Panel.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "Signin7Panel.h"
//
//  Signin7Panel.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "Signin7Panel.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "SigninComp.h"
#include "NetManager.h"
#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void Signin7Panel::show(CCNode* parent) {
    Signin7Panel* panel = Signin7Panel::create();
    parent->addChild(panel);
}


#pragma mark - Super

Signin7Panel::~Signin7Panel() {
}

bool Signin7Panel::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("pic/panel/signin7/signin7_panel.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        CCSprite* text_spr = CCSprite::create("pic/panel/signin7/text_reward.png");
        text_spr->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
        _panel->addChild(text_spr);
        
        CCSprite* icon_1 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_1->setPosition(ccp(panelSize.width* .25f, panelSize.height* .625f));
        icon_1->setTag(1);
        _panel->addChild(icon_1);
        
        CCSprite* icon_2 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_2->setPosition(ccp(panelSize.width* .47f, panelSize.height* .625f));
        icon_2->setTag(2);
        _panel->addChild(icon_2);
        
        CCSprite* icon_3 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_3->setPosition(ccp(panelSize.width* .25f, panelSize.height* .485f));
        icon_3->setTag(3);
        _panel->addChild(icon_3);
        
        CCSprite* icon_4 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_4->setPosition(ccp(panelSize.width* .47f, panelSize.height* .485f));
        icon_4->setTag(4);
        _panel->addChild(icon_4);
        
        CCSprite* icon_5 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_5->setPosition(ccp(panelSize.width* .25f, panelSize.height* .345f));
        icon_5->setTag(5);
        _panel->addChild(icon_5);
        
        CCSprite* icon_6 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_6->setPosition(ccp(panelSize.width* .47f, panelSize.height* .345f));
        icon_6->setTag(6);
        _panel->addChild(icon_6);
        
        CCSprite* icon_7 = CCSprite::create("pic/panel/signin7/icon_bg.png");
        icon_7->setPosition(ccp(panelSize.width* .36f, panelSize.height* .195f));
        icon_7->setTag(7);
        _panel->addChild(icon_7);
        
        CCSprite* model = CCSprite::create("pic/panel/signin7/si_model.png");
        model->setPosition(ccp(panelSize.width* .72f, panelSize.height * 0.46f));
        _panel->addChild(model);
        
        CCSprite* model_bg = CCSprite::create("pic/panel/signin7/model_bg.png");
        model_bg->setPosition(ccp(panelSize.width* .72f, panelSize.height * 0.46f));
        _panel->addChild(model_bg);
        
        CCFadeOut* fo = CCFadeOut::create(0.8f);
        CCScaleTo* st = CCScaleTo::create(0.8, 1.0f);
        CCFadeIn* fi = CCFadeIn::create(0.5f);
        CCScaleTo* st2 = CCScaleTo::create(0.5, 1.02f);
        CCSequence* seq = CCSequence::create(CCSpawn::create(fi, st2, NULL), CCDelayTime::create(0.2f), CCScaleTo::create(0.5, 1.0f), CCSpawn::create(fi, st2, NULL), CCSpawn::create(fo, st, NULL), CCDelayTime::create(0.2f), NULL);
        model_bg->runAction(CCRepeatForever::create(seq));
        
        this->config_siginInfo();
        
        
        return true;
    }
    else {
        return false;
    }
}

void Signin7Panel::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&Signin7Panel::signin_callback_303), "HTTP_FINISHED_303", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&Signin7Panel::keyBackStatus), .8f);
}
void Signin7Panel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void Signin7Panel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void Signin7Panel::config_siginInfo(){
    CCArray* arr = DATA->getSignin()->signin7_template();
    CCObject* obj = NULL;
    CCARRAY_FOREACH(arr, obj){
        CCSprite* icon_bg = (CCSprite*)_panel->getChildByTag(arr->indexOfObject(obj) + 1);
        CCDictionary* dic = (CCDictionary*)obj;
        CCString* cloth_id = (CCString*)dic->valueForKey("uri");
        CCSprite* icon = CCSprite::create(DATA->clothes_icon_path_with_id(cloth_id->intValue())->getCString());
        icon->setScale(0.6f);
        icon->setPosition(ccp(icon_bg->getContentSize().width* .5f, icon_bg->getContentSize().height* .45));
        icon_bg->addChild(icon);
        
        const char* id = dic->valueForKey("id")->getCString();
        SigninState cur_state = DATA->getSignin()->fetch_signin7_state(id);
        CCLOG("STATE = %d", cur_state);
        CCSprite* state_spr = NULL;
        switch (cur_state) {
            case e_SigninState_Locked:{
                // 锁定
                //                state_spr = CCSprite::create("res/pic/mask.png");
                //                state_spr->CCNode::setScale(icon_bg->getContentSize().width / state_spr->getContentSize().width, icon_bg->getContentSize().height / state_spr->getContentSize().height);
                //                state_spr->setPosition(ccp(icon_bg->getContentSize().width* .5f, icon_bg->getContentSize().height* .5f));
                //                icon_bg->addChild(state_spr);
                
                
            }
                
                
                break;
            case e_SigninState_Available:{
                // 可用
                state_spr = CCSprite::create("pic/panel/signin7/state_clickget.png");
                state_spr->setPosition(ccp(icon_bg->getContentSize().width* .5f, state_spr->getContentSize().height* .5f));
                state_spr->setTag(200);
                icon_bg->addChild(state_spr);
                
                CCSprite* stars = CCSprite::create("pic/panel/signin7/state_clickget.png");
                stars->setPosition(ccp(state_spr->getContentSize().width* .5f, state_spr->getContentSize().height* .5f));
                state_spr->addChild(stars);
                
                CCFadeOut* fo = CCFadeOut::create(0.8f);
                CCScaleTo* st = CCScaleTo::create(0.8, 1.0f);
                CCFadeIn* fi = CCFadeIn::create(0.5f);
                CCScaleTo* st2 = CCScaleTo::create(0.5, 1.02f);
                CCSequence* seq = CCSequence::create(CCSpawn::create(fi, st2, NULL), CCDelayTime::create(0.2f), CCScaleTo::create(0.5, 1.0f), CCSpawn::create(fi, st2, NULL), CCSpawn::create(fo, st, NULL), CCDelayTime::create(0.2f), NULL);
                stars->runAction(CCRepeatForever::create(seq));
            }
                break;
            case e_SigninState_Done:{
                // 已签
                state_spr = CCSprite::create("pic/panel/signin7/state_got.png");
                state_spr->setPosition(ccp(icon_bg->getContentSize().width - state_spr->getContentSize().width* .5f, state_spr->getContentSize().height* .5f));
                icon_bg->addChild(state_spr);
            }
                
                
                break;
            default:
                break;
        }
    }
    
}

bool Signin7Panel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

void Signin7Panel::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos_begin = _panel->convertToNodeSpace(pTouch->getStartLocation());
    CCPoint pos_end = _panel->convertToNodeSpace(pTouch->getLocation());
    for (int i = 1; i <= 7; i++) {
        CCSprite* icon_bg = (CCSprite*)_panel->getChildByTag(i);
        if (icon_bg->boundingBox().containsPoint(pos_begin) && icon_bg->boundingBox().containsPoint(pos_end)) {
            CCLOG("ICON %d IS TOUCHED", i);
            CCString* id_str = CCString::createWithFormat("%d", i);
            SigninState state = DATA->getSignin()->fetch_signin7_state(id_str->getCString());
            switch (state) {
                case e_SigninState_Locked:
                    
                    break;
                case e_SigninState_Available:
                    // 发送签到请求
                    LOADING->show_loading();
                    NET->perform_signin7_303(id_str->getCString());
                    
                    _signin_id = i;
                    break;
                case e_SigninState_Done:
                    
                    break;
                    
                default:
                    break;
            }
            
            break;
        }
    }
}

void Signin7Panel::signin_callback_303(){
    LOADING->remove();
    
    CCSprite* icon_bg = (CCSprite*)_panel->getChildByTag(_signin_id);
    if (icon_bg->getChildByTag(200)) {
        icon_bg->removeChildByTag(200, true);
        CCSprite* state = CCSprite::create("pic/panel/signin7/state_got.png");
        state->setPosition(ccp(icon_bg->getContentSize().width - state->getContentSize().width* .5f, state->getContentSize().height* .5f));
        icon_bg->addChild(state);
        
        CCArray* arr = DATA->getSignin()->signin7_template();
        CCObject* obj = NULL;
        CCString* cloth_id = NULL;
        CCARRAY_FOREACH(arr, obj){
            CCDictionary* dic = (CCDictionary*)obj;
            const char* id = dic->valueForKey("id")->getCString();
            if (CCString::createWithFormat("%d", _signin_id)->compare(id) == 0) {
                cloth_id = (CCString*)dic->valueForKey("uri");
            }
        }
        
        _clothesId = atoi(cloth_id->getCString());
        std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
        if (stdStr.empty()) {
            CCString* saveStr = CCString::createWithFormat("%d;", _clothesId);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }else{
            CCString* saveStr = CCString::createWithFormat("%d;", _clothesId);
            stdStr.append(saveStr->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
        }
        CCUserDefault::sharedUserDefault()->flush();
        
        
        PromptLayer* tip = PromptLayer::create();
        tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "领取成功");
    }
}

#pragma mark - Inner

void Signin7Panel::remove() {
    AUDIO->goback_effect();
    
    this->removeFromParentAndCleanup(true);
}

void Signin7Panel::keyBackClicked(){
    num_child++;
    CCLog("===== Signin7Panel  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}
