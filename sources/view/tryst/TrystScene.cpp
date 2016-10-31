//
//  TrystScene.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystScene.h"

// Export
TrystScene::~TrystScene() {

}

TrystScene* TrystScene::create(const char *id) {
    TrystScene* rtn = new TrystScene();
    if (rtn && rtn->init(id)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}



// Inner

bool TrystScene::init(const char *id) {
    if (CCScene::init()) {
        
        return  true;
    }
    else {
        return false;
    }
}

void TrystScene::onEnter() {
    CCScene::onEnter();
    
}

void TrystScene::onExit() {

    CCScene::onExit();
}

