//
//  PlayerComp.cpp
//  tiegao
//
//  Created by mac on 16/5/21.
//
//

#include "PlayerComp.h"

PlayerComp::~PlayerComp() {
}

bool PlayerComp::init() {
    
    return true;
}

void PlayerComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("PlayerComp::init_with_json() json object error.");
        return;
    }

    _energy = json["energy"].asInt();
    _exp = json["exp"].asInt();
    _grade = json["grade"].asInt();
    _popularity = json["popularity"].asInt();
    _coin = json["coin"].asInt();
    _gold = json["gold"].asInt();
    _diam = json["diam"].asInt();
    _cur_mission = json["cur_mission"].asInt();
    _company_phase = json["company_phase"].asInt();
}