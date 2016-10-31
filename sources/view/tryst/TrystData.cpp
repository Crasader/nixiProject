//
//  TrystData.cpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#include "TrystData.h"

// Export
TrystData::~TrystData() {
    CC_SAFE_DELETE_ARRAY(_dialogs);
}

TrystData* TrystData::create(CCArray* dialogs) {
    TrystData* rtn = new TrystData();
    if (rtn && rtn->init(dialogs)) {
        rtn->autorelease();
    }
    else {
        CC_SAFE_RELEASE_NULL(rtn);
    }
    
    return rtn;
}



// Inner

bool TrystData::init(CCArray* dialogs) {
    this->_dialogs = dialogs;
    this->_dialogs->retain();
    
    return  true;
}
