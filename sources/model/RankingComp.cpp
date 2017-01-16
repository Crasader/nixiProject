//
//  RankingComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "RankingComp.h"
#include "ShowComp.h"
#include "DataManager.h"

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
    
    string myId = DATA->getLogin()->obtain_sid();
    if (json.isArray()) {
        _selfRank = -1;
        CCArray* arr = CCArray::create();
        int count = json.size();
        for (int i = 0; i < count; i++) {
            CSJson::Value value = json[i];
            if (value != value.jsonNull) {
                ShowComp* shower = ShowComp::create();
                shower->init_with_json(value);
                arr->addObject(shower);
                if (shower->getShowID().compare(myId) == 0) {
                    _selfRank = i;
                }
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
