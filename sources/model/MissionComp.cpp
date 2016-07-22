//
//  MissionComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "MissionComp.h"
#include "AppUtil.h"

MissionComp::~MissionComp() {
    CC_SAFE_DELETE(_cmpleted_mission);
}

bool MissionComp::init() {
    _cmpleted_mission = nullptr;
    
    return true;
}

void MissionComp::init_with_json(Value json) {
    if (json.type() == nullValue && !json.isArray()) {
        CCLOG("<ERROR> MissionComp::init_with_json() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_cmpleted_mission);
    _cmpleted_mission = AppUtil::dictionary_with_json(json);
    _cmpleted_mission->retain();
}

CCDictionary* MissionComp::cmpleted_mission() {
    CCAssert(_cmpleted_mission != nullptr, "任务信息尚未初始化~!");
    return _cmpleted_mission;
}

int MissionComp::mission_rating(const char *mission_index) {
    CCObject* pObj = _cmpleted_mission->objectForKey(mission_index);
    if (pObj == NULL) {
        return 0;
    }
    else {
        return ((CCInteger* )pObj)->getValue();
    }
}

