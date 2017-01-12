//
//  DisplayManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "DisplayManager.h"
#include "AppUtil.h"

static DisplayManager* _instance = nullptr;

void DisplayManager::blink() {
    CCLOG("DisplayManager::blink() - checked~");
    if (_zrSpr1) {
        
        if (_curZRId != _savedZRId) {
            float interval = 0.1;
            float intervalTimes = 0;
            
            float rand1 = CCRANDOM_0_1();
            int delayTimes = rand1 < 0.33 ? 1 : (rand1 < 0.66 ? 2 : 3);
            _savedZRId = _curZRId;
            CCArray* arrFrame = CCArray::create();
            
            CCString* str1 = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d.png", _curZRId);
            CCSpriteFrame* frame1 = CCSpriteFrame::create(str1->getCString(), CCRectMake(0, 0, 900, 1136));
            arrFrame->addObject(frame1);
            intervalTimes++;
            
            CCString* str2 = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d_1.png", _curZRId);
            CCSpriteFrame* frame2 = CCSpriteFrame::create(str2->getCString(), CCRectMake(0, 0, 900, 1136));
            arrFrame->addObject(frame2);
            intervalTimes++;
            
            if (_curZRId == 90006 || _curZRId == 90007 || _curZRId == 90009 || _curZRId == 900010) {
                
                CCString* str3 = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d_2.png", _curZRId);
                CCSpriteFrame* frame3 = CCSpriteFrame::create(str3->getCString(), CCRectMake(0, 0, 900, 1136));
                arrFrame->addObject(frame3);
                intervalTimes++;
                
                for (int i = 0; i < delayTimes; i++) {
                    CCString* str4 = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d_3.png", _curZRId);
                    CCSpriteFrame* frame4 = CCSpriteFrame::create(str4->getCString(), CCRectMake(0, 0, 900, 1136));
                    arrFrame->addObject(frame4);
                    intervalTimes++;
                }
            }
            else {
                for (int i = 0; i < delayTimes; i++) {
                    CCString* str3 = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d_2.png", _curZRId);
                    CCSpriteFrame* frame3 = CCSpriteFrame::create(str3->getCString(), CCRectMake(0, 0, 900, 1136));
                    arrFrame->addObject(frame3);
                    intervalTimes++;
                }
            }
            
            CCString* strRenew = CCString::createWithFormat("pic/clothesScene/clothes/9zhuangrong/%d.png", _curZRId);
            CCSpriteFrame* frameRenew = CCSpriteFrame::create(strRenew->getCString(), CCRectMake(0, 0, 900, 1136));
            arrFrame->addObject(frameRenew);
            intervalTimes++;
            
            CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame, 0.1);
            this->setBlinkAnim(CCAnimate::create(animation));
            
            _nextInterval = intervalTimes * interval;
        }
        
        if (_blinkAnim) {
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DisplayManager::blink), this);
            _zrSpr1->stopAllActions();
            _zrSpr1->runAction(_blinkAnim);
            float nextDuration = _nextInterval + CCRANDOM_0_1() * 6;
            CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&DisplayManager::blink), this, nextDuration, kCCRepeatForever, nextDuration, false);
        }
    }
}

void DisplayManager::stopBlink() {
    if (_zrSpr1) {
        _zrSpr1->stopAllActions();
    }
    this->setZRSpr(NULL);
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DisplayManager::blink), this);
}

DisplayManager::~DisplayManager() {
    
}

DisplayManager* DisplayManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DisplayManager();
        _instance->init();
    }
    
    return _instance;
}

void DisplayManager::init() {
    CCSize winsize = CCDirector::sharedDirector()->getWinSize();
    _width = winsize.width;
    _height = winsize.height;
    // blink
    _curZRId = 90000;
    _savedZRId = 0;
}

const char* DisplayManager::fangzhengFont(){
    return "FZY4JW--GB1-0.ttf";
}

ccColor3B DisplayManager::defalutColor() {
    return ccc3(113, 89, 102);
}

ccColor3B DisplayManager::dullBlueColor() {
    return ccc3(102, 102, 154);
}

CCPoint DisplayManager::center() {
    return CCPointMake(_width * 0.5, _height * 0.5);
}

float DisplayManager::ScreenWidth(){
    return _width;
}

float DisplayManager::ScreenHeight(){
    return _height;
}

float DisplayManager::OriginWidth(){
    return (_width - 640) * 0.5f;
}

CCString* DisplayManager::GetOffTheNumber(int index){
    CCString* str;
    CSJson::Value data = AppUtil::read_json_file("res/story/storyNumber");
    CCDictionary* dic = AppUtil::dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());
    str = (CCString* )achievemArr->objectAtIndex(0);
    
    return str;
}

CCString* DisplayManager::GetOffTheName(int index){
    CCString* str;
    CSJson::Value data = AppUtil::read_json_file("res/story/storyNumber");
    CCDictionary* dic = AppUtil::dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());
    str = (CCString* )achievemArr->objectAtIndex(1);
    
    return str;
}

CCString* DisplayManager::GetOffTheNumber2(int index){
    CCString* str;
    CSJson::Value data = AppUtil::read_json_file("res/vipStory/vip_storyNumber");
    CCDictionary* dic = AppUtil::dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());
    str = (CCString* )achievemArr->objectAtIndex(0);
    
    return str;
}

CCString* DisplayManager::GetOffTheName2(int index){
    CCString* str;
    CSJson::Value data = AppUtil::read_json_file("res/vipStory/vip_storyNumber");
    CCDictionary* dic = AppUtil::dictionary_with_json(data);
    CCString* keyStr = CCString::createWithFormat("%d", index);
    CCArray* achievemArr = (CCArray* )dic->objectForKey(keyStr->getCString());
    str = (CCString* )achievemArr->objectAtIndex(1);
    
    return str;
}

float DisplayManager::W() {
    return _width;
}

float DisplayManager::H() {
    return _height;
}

float DisplayManager::halfW() {
    return _width * 0.5;
}

float DisplayManager::halfH() {
    return _height * 0.5;
}

float DisplayManager::btn_scale() {
    return 1.01f;
}

