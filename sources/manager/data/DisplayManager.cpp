//
//  DisplayManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "DisplayManager.h"

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

CCPoint DisplayManager::center() {
    return CCPointMake(_width * 0.5, _height * 0.5);
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
    return 1.02f;
}

