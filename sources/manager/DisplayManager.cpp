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

