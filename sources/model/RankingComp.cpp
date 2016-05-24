//
//  RankingComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "RankingComp.h"

RankingComp::~RankingComp() {
}

bool RankingComp::init() {
    
    return true;
}

void RankingComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("RankingComp::init_with_json() json object error.");
        return;
    }
    
}