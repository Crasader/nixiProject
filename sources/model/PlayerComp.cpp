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
    _guide = json["guide"].asInt();
    
    mystery = json["mystery"].asInt();
    if (json["identity"] != CSJson::nullValue) {
        _identity = json["identity"].asInt();
    }
    else {
        _identity = 0;
    }
    
    
    CC_SAFE_RELEASE(rating);
    rating = AppUtil::dictionary_with_json(json["rating"]);
    rating->retain();
    
//    this->test();
}

void PlayerComp::init_custom(CSJson::Value json) {
    if (json.type() == nullValue) {
        return;
    }
    
    this->missionEnergyCost = json["mission_c"].asInt();
    this->mysteryEnergyCost = json["mystery_c"].asInt();
    this->trystEnergyCost = json["tryst_c"].asInt();
    this->storyEnergyCost = json["story_c"].asInt();
    this->vipStoryBuyCost = json["story_vip_b"].asInt();
    
    this->identityDiamReward = json["identity_r"].asInt();
}

bool PlayerComp::hasCommitIdentity() {
    return _identity == 2;
}

int PlayerComp::total_ratings() {
    int rtn = 0;
    if (rating) {
        CCDictElement* pElem = NULL;
        CCDICT_FOREACH(rating, pElem) {
            CCInteger* value = (CCInteger*)pElem->getObject();
            rtn += value->getValue();
        }
    }
    
    return rtn;
}

int PlayerComp::ratings(int phase) {
    int rtn = 0;
    if (rating) {
        CCInteger* v = (CCInteger*)rating->objectForKey(CCString::createWithFormat("%d", phase)->getCString());
        if (v != NULL) {
            rtn = v->getValue();
        }
    }
    
    return rtn;
}

#pragma - Inner API

void PlayerComp::test() {
    CCLOG("====== 所获总星数：%d", total_ratings());
    for (int i = 0; i < 8; i++) {
        CCLOG("公司阶段 %d 所获星数 %d", i, ratings(i));
    }
}



