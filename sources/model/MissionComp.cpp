//
//  MissionComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MissionComp.h"

MissionComp::~MissionComp() {
}

bool MissionComp::init() {
    
    return true;
}

void MissionComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("MissionComp::init_with_json() json object error.");
        return;
    }
    
}