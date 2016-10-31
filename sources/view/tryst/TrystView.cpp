//
//  TrystView.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystView.h"

// Export
TrystView::~TrystView() {
    
}

TrystView* TrystView::create(const char *id) {
    TrystView* rtn = new TrystView();
    if (rtn && rtn->init(id)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}



// Inner

bool TrystView::init(const char *id) {
    if (CCScene::init()) {
        
        return  true;
    }
    else {
        return false;
    }
}

void TrystView::onEnter() {
    CCScene::onEnter();
    
}

void TrystView::onExit() {
    
    CCScene::onExit();
}