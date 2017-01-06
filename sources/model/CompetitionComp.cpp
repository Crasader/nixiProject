//
//  CompetitionComp.cpp
//  tiegao
//
//  Created by mac on 17/1/5.
//
//

#include "CompetitionComp.h"
//
//  CompetitionComp.cpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#include "CompetitionComp.h"
#include "ShowComp.h"
#include "AppUtil.h"

void CompetitionThemeInfo::config(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    this->setThemeName(json["name"].asString());
    this->setTags(AppUtil::array_with_json(json["tags"]));
    this->setRule(AppUtil::array_with_json(json["rule"]));
    this->setDay(json["D"].asInt());
    this->setHour(json["H"].asInt());
    this->setMinu(json["M"].asInt());
    this->setSecd(json["S"].asInt());
}

void CompetitionItem::config(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    this->setId(json["sid"].asString());
    this->setNickname(json["nickname"].asString());
    this->setCollected(json["collected"].asInt());
    this->setLastRank(json["last_rank"].asInt());
    this->setOndress(AppUtil::dictionary_with_json(json["ondress"]));
    this->setOrnaments(AppUtil::dictionary_with_json(json["ornaments"]));
    
    this->setDailyTimes(json["daily_times"].asInt());
    this->setBuffs(AppUtil::array_with_json(json["buffs"]));
    this->setDailyOpponents(AppUtil::array_with_json(json["daily_opponents"]));
    
    this->setMatch(json["s_match"].asInt());
    this->setPuplar(json["s_popular"].asInt());
    this->setCharm(json["s_charm"].asInt());
    this->setLuck(json["s_luck"].asInt());
}


#pragma mark -

CompetitionComp::~CompetitionComp() {
    CC_SAFE_DELETE(_theme);
    CC_SAFE_DELETE(_self);
    CC_SAFE_DELETE(_opponent);
}

bool CompetitionComp::init() {
    _theme = NULL;
    _self = NULL;
    _opponent = NULL;
    
    return true;
}

void CompetitionComp::createRanklist(Value json) {
    if (json.type() == CSJson::nullValue) {
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
        
        this->setRanklist(arr);
    }
}

void CompetitionComp::createThemeInfo(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }

    CompetitionThemeInfo* theme = CompetitionThemeInfo::create();
    theme->config(json);
    this->setTheme(theme);
}

void CompetitionComp::createSelfInfo(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CompetitionItem* info = CompetitionItem::create();
    info->config(json);
    this->setSelf(info);
}

void CompetitionComp::createOpponentInfo(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    CompetitionItem* info = CompetitionItem::create();
    info->config(json);
    this->setOpponent(info);
}



