//
//  NoticeComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "NoticeComp.h"

NoticeComp::~NoticeComp() {
}

bool NoticeComp::init() {
    
    return true;
}

void NoticeComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("NoticeComp::init_with_json() json object error.");
        return;
    }
    
}
