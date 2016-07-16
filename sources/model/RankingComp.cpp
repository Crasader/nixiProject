//
//  RankingComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "RankingComp.h"
#include "ShowComp.h"

RankingComp::~RankingComp() {
    CC_SAFE_DELETE(_ranking);
}

bool RankingComp::init() {
    _ranking = NULL;
    
    return true;
}

void RankingComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("RankingComp::init_with_json() json object error.");
        return;
    }
    
    if (json.isArray()) {
        CCArray* arr = CCArray::create();
        int count = json.size();
        for (int i = 0; i < count; i++) {
            CSJson::Value value = json[i];
            if (value != value.jsonNull) {
                ShowComp* shower = ShowComp::create();
                shower->init_with_json(value);
                arr->addObject(shower);
            }
        }
        
        CC_SAFE_RELEASE(_ranking);
        _ranking = arr;
        _ranking->retain();
    }
}

CCArray* RankingComp::ranking() {
    return _ranking;
}
