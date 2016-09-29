//
//  TempSignin.cpp
//  tiegao
//
//  Created by mac on 16/8/1.
//
//

#include "TempSignin.h"
//
//  TempSignin.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "TempSignin.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "DataManager.h"
#include "SigninComp.h"
#include "NetManager.h"
#include "Loading2.h"
#include "PromptLayer.h"

#pragma mark - Export

void TempSignin::show(CCNode* parent) {
    TempSignin* panel = TempSignin::create();
    parent->addChild(panel);
}


#pragma mark - Super

TempSignin::~TempSignin() {
}

bool TempSignin::init() {
    if (CCLayer::init()) {
        num_child = 0;
        
//        CCSprite* mask = CCSprite::create("res/pic/mask.png");
//        mask->setPosition(DISPLAY->center());
//        this->addChild(mask);
        
        _content = CCLayer::create();
        this->addChild(_content);
        
        _panel = CCSprite::create("pic/panel/temp_signin/tempsignin_bg.png");
        _panel->setPosition(DISPLAY->center());
        _content->addChild(_panel);
        
        CCSize panelSize = _panel->boundingBox().size;
        
        float leftX = panelSize.width * .16f;
        float rightX = panelSize.width * .42f;
        
        CCSprite* text_spr = CCSprite::create("pic/panel/temp_signin/tempsignin_text.png");
        text_spr->setPosition(ccp(panelSize.width* .5f, panelSize.height* .88f));
        _panel->addChild(text_spr);
        
        CCSprite* icon_1 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_1->setPosition(ccp(leftX, panelSize.height* .7f));
        icon_1->setTag(1);
        _panel->addChild(icon_1);
        
        CCSprite* icon_2 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_2->setPosition(ccp(rightX, panelSize.height* .7f));
        icon_2->setTag(2);
        _panel->addChild(icon_2);
        
        CCSprite* icon_3 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_3->setPosition(ccp(leftX, panelSize.height* .525f));
        icon_3->setTag(3);
        _panel->addChild(icon_3);
        
        CCSprite* icon_4 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_4->setPosition(ccp(rightX, panelSize.height* .525f));
        icon_4->setTag(4);
        _panel->addChild(icon_4);
        
        CCSprite* icon_5 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_5->setPosition(ccp(leftX, panelSize.height* .36f));
        icon_5->setTag(5);
        _panel->addChild(icon_5);
        
        CCSprite* icon_6 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_6->setPosition(ccp(rightX, panelSize.height* .36f));
        icon_6->setTag(6);
        _panel->addChild(icon_6);
        
        CCSprite* icon_7 = CCSprite::create("pic/panel/temp_signin/tempsignin_icon_plate.png");
        icon_7->setPosition(ccp(panelSize.width* .29f, panelSize.height* .195f));
        icon_7->setTag(7);
        _panel->addChild(icon_7);
        
        CCSprite* model = CCSprite::create("pic/panel/temp_signin/tempsignin_shower.png");
        model->setPosition(ccp(panelSize.width* .77f, panelSize.height * 0.41f));
        _panel->addChild(model);
        
//        CCSprite* model_bg = CCSprite::create("pic/panel/signin7/model_bg.png");
//        model_bg->setPosition(ccp(panelSize.width* .72f, panelSize.height * 0.46f));
//        _panel->addChild(model_bg);
//        
//        CCFadeOut* fo = CCFadeOut::create(0.8f);
//        CCScaleTo* st = CCScaleTo::create(0.8, 1.0f);
//        CCFadeIn* fi = CCFadeIn::create(0.5f);
//        CCScaleTo* st2 = CCScaleTo::create(0.5, 1.02f);
//        CCSequence* seq = CCSequence::create(CCSpawn::create(fi, st2, NULL), CCDelayTime::create(0.2f), CCScaleTo::create(0.5, 1.0f), CCSpawn::create(fi, st2, NULL), CCSpawn::create(fo, st, NULL), CCDelayTime::create(0.2f), NULL);
//        model_bg->runAction(CCRepeatForever::create(seq));
        
        this->config_siginInfo();
        
        
        return true;
    }
    else {
        return false;
    }
}

void TempSignin::onEnter() {
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&TempSignin::signin_callback_341), "HTTP_FINISHED_341", NULL);
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchSwallowEnabled(true);
    
    this->scheduleOnce(SEL_SCHEDULE(&TempSignin::keyBackStatus), .8f);
}

void TempSignin::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void TempSignin::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

void TempSignin::config_siginInfo(){
    // ----
    CCArray* arr = DATA->getTempSigninTemplate();
    
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
        // ----
        SigninState cur_state = DATA->fetch_tempsignin_state(DATA->getTempSigninUserdata(), id);
        
        CCLOG("STATE = %d", cur_state);
        CCSprite* state_spr = NULL;
        switch (cur_state) {
            case e_SigninState_Locked:{ // 没到时间
                int index = arr->indexOfObject(obj);
                CCString* name = CCString::createWithFormat("pic/panel/temp_signin/date_10_%d.png", 1+index);
                state_spr = CCSprite::create(name->getCString());
                if (state_spr) {
                    state_spr->setPosition(ccp(icon_bg->getContentSize().width - state_spr->getContentSize().width* .5f, icon_bg->getContentSize().height* 0.15f));
                    icon_bg->addChild(state_spr);
                }
            }  break;
            case e_SigninState_Available:{ // 可用
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
            } break;
            case e_SigninState_Done:{ // 已签
                state_spr = CCSprite::create("pic/panel/signin7/state_got.png");
                state_spr->setPosition(ccp(icon_bg->getContentSize().width - state_spr->getContentSize().width* .5f, state_spr->getContentSize().height* .5f));
                icon_bg->addChild(state_spr);
            }  break;
            case e_SigninState_Expired:{ // 过期
                state_spr = CCSprite::create("pic/panel/temp_signin/tempsignin_expired.png");
                state_spr->setPosition(ccp(icon_bg->getContentSize().width - state_spr->getContentSize().width* .5f, state_spr->getContentSize().height* .5f));
                icon_bg->addChild(state_spr);
            }  break;
            default:
                break;
        }
    }
    
}

bool TempSignin::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _panel->boundingBox().containsPoint(location)) {
        this->removeFromParentAndCleanup(true);
    }
    
    return true;
}

void TempSignin::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    CCPoint pos_begin = _panel->convertToNodeSpace(pTouch->getStartLocation());
    CCPoint pos_end = _panel->convertToNodeSpace(pTouch->getLocation());
    
    CCArray* temp = DATA->getTempSigninTemplate();
    
    for (int i = 1; i <= 7; i++) {
        CCSprite* icon_bg = (CCSprite*)_panel->getChildByTag(i);
        if (icon_bg->boundingBox().containsPoint(pos_begin) && icon_bg->boundingBox().containsPoint(pos_end)) {
            CCLOG("ICON %d IS TOUCHED", i);
//            CCString* id_str = CCString::createWithFormat("%d", i);
            CCDictionary* item = (CCDictionary*)temp->objectAtIndex(i - 1);
            CCString* id_str = (CCString*)item->objectForKey("id");
            SigninState state = DATA->fetch_tempsignin_state(DATA->getTempSigninUserdata(), id_str->getCString());
            switch (state) {
                case e_SigninState_Locked:
                    
                    break;
                case e_SigninState_Available:
                    // 发送签到请求
                    LOADING->show_loading();
                    NET->perform_temp_signin_341(id_str->getCString());
                    
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

void TempSignin::signin_callback_341(){
    LOADING->remove();
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification("UPDATE_NEWS_STATUS");
    
    CCSprite* icon_bg = (CCSprite*)_panel->getChildByTag(_signin_id);
    if (icon_bg->getChildByTag(200)) {
        icon_bg->removeChildByTag(200, true);
        CCSprite* state = CCSprite::create("pic/panel/signin7/state_got.png");
        state->setPosition(ccp(icon_bg->getContentSize().width - state->getContentSize().width* .5f, state->getContentSize().height* .5f));
        icon_bg->addChild(state);
        
//        CCArray* arr = DATA->getSignin()->signin7_template();
//        CCObject* obj = NULL;
//        CCString* cloth_id = NULL;
//        CCARRAY_FOREACH(arr, obj){
//            CCDictionary* dic = (CCDictionary*)obj;
//            const char* id = dic->valueForKey("id")->getCString();
//            if (CCString::createWithFormat("%d", _signin_id)->compare(id) == 0) {
//                cloth_id = (CCString*)dic->valueForKey("uri");
//                break;
//            }
//        }
//        
//        _clothesId = atoi(cloth_id->getCString());
//        std::string stdStr = CCUserDefault::sharedUserDefault()->getStringForKey("SaveClothes", "");
//        if (stdStr.empty()) {
//            CCString* saveStr = CCString::createWithFormat("%d;", _clothesId);
//            stdStr.append(saveStr->getCString());
//            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
//        }else{
//            CCString* saveStr = CCString::createWithFormat("%d;", _clothesId);
//            stdStr.append(saveStr->getCString());
//            CCUserDefault::sharedUserDefault()->setStringForKey("SaveClothes", stdStr.c_str());
//        }
//        CCUserDefault::sharedUserDefault()->flush();
        
        
        PromptLayer* tip = PromptLayer::create();
        tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "领取成功");
    }
}

#pragma mark - Inner

void TempSignin::remove() {
    AUDIO->goback_effect();
    
    this->removeFromParentAndCleanup(true);
}

void TempSignin::keyBackClicked(){
    num_child++;
    CCLog("===== TempSignin  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->remove();
}
