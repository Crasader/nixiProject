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
    
    if (json.isObject()) { // 对象
        CCDictionary* dic = CCDictionary::create();
        std::vector<std::string> keys = json.getMemberNames();
        for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
            std::string key = (std::string)* iterator;
            CSJson::Value value = json.get(key, value.jsonNull);
            if (value != value.jsonNull) {
                ShowComp* shower = ShowComp::create();
                shower->init_with_json(value);
                dic->setObject(shower, key);
            }
        }
        
        CC_SAFE_RELEASE(_ranking);
        _ranking = dic;
        _ranking->retain();
    }
}

CCDictionary* RankingComp::ranking() {
    return _ranking;
}
