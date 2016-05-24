//
//  StoryComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "StoryComp.h"

StoryComp::~StoryComp() {
}

bool StoryComp::init() {

    return true;
}

void StoryComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("StoryComp::init_with_json() json object error.");
        return;
    }
    
}