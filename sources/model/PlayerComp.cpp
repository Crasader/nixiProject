//
//  PlayerComp.cpp
//  tiegao
//
//  Created by mac on 16/5/21.
//
//

#include "PlayerComp.h"
#include "AppUtil.h"

PlayerComp::~PlayerComp() {
    CC_SAFE_DELETE(rating);
}

bool PlayerComp::init() {
    rating = NULL;
    
    return true;
}

void PlayerComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("PlayerComp::init_with_json() json object error.");
        return;
    }
    
    update = json["update"].asInt64();
    energy = json["energy"].asInt();
    coin = json["coin"].asInt();
    gold = json["gold"].asInt();
    diam = json["diam"].asInt();
    mission = json["mission"].asInt();
    phase = json["phase"].asInt();
    left = json["left"].asInt();
    energyBuyTimes = json["eb_times"].asInt();
    
    CC_SAFE_RELEASE(rating);
    rating = AppUtil::dictionary_with_json(json["rating"]);
    rating->retain();
}

