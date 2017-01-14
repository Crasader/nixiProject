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
#include "DataManager.h"
#include "ShowComp.h"
#include "AppUtil.h"

void CompetitionThemeInfo::config(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    this->setThemeName(json["name"].asString());
    this->setTags(AppUtil::array_with_json(json["tags"]));
    this->setRule(AppUtil::array_with_json(json["rule"]));
    this->setSearchFreeCount(json["search_free_count"].asInt());
    this->setStartFreeCount(json["start_free_count"].asInt());
    this->setStartLimit(json["start_limit"].asInt());
    
    this->setDay(json["D"].asInt());
    this->setHour(json["H"].asInt());
    this->setMinu(json["M"].asInt());
    this->setSecd(json["S"].asInt());
}

void CompetitionItem::config(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    this->setId(json["id"].asString());
    this->setNickname(json["nickname"].asString());
    this->setCollected(json["collected"].asInt());
    this->setLastRank(json["last_rank"].asInt());
    this->setOndress(AppUtil::dictionary_with_json(json["ondress"]));
    CCDictionary* dic = AppUtil::dictionary_with_json(json["ornaments"]);
    _ondress->setObject(dic, "7");
    
    this->setSearchTimes(json["daily_search"].asInt());
    this->setStartTimes(json["daily_start"].asInt());
    this->setCurOpponent(json["cur_opponent"].asString());
    this->setDailyOpponents(AppUtil::array_with_json(json["daily_opponents"]));
    
    this->setSearchCost(json["search_cost"].asInt());
    this->setStartCost(json["start_cost"].asInt());
    this->setScore(json["score"].asInt());
    
    this->setMatch(json["s_match"].asInt());
    this->setPuplar(json["s_popular"].asInt());
    this->setCharm(json["s_charm"].asInt());
    this->setLuck(json["s_luck"].asInt());
    
    this->setBuffId(json["buff"].asInt());
    this->setBuffDesc(json["buff_desc"].asString());
    this->setBuffedId(json["buffed"].asInt());
    this->setBuffedDesc(json["buffed_desc"].asString());
    
    this->setAdded(false);
}

bool CompetitionComp::hasInitRankInfo() {
    return _ranklist != NULL;
}


#pragma mark -

CompetitionComp::~CompetitionComp() {
    CC_SAFE_DELETE(_theme);
    CC_SAFE_DELETE(_self);
    CC_SAFE_DELETE(_opponent);
}

bool CompetitionComp::init() {
    _ranklist = NULL;
    _theme = NULL;
    _self = NULL;
    _opponent = NULL;
    
    return true;
}

void CompetitionComp::createRanklist(Value json) {
    if (json.type() == CSJson::nullValue) {
        return;
    }
    
    string myId = DATA->getShow()->getShowID();
    if (json.isArray()) {
        _selfRank = -1;
        CCArray* arr = CCArray::create();
        int count = json.size();
        for (int i = 0; i < count; i++) {
            CSJson::Value value = json[i];
            if (value != value.jsonNull) {
                CompetitionItem* item = CompetitionItem::create();
                item->config(value);
                arr->addObject(item);
                if (item->getId().compare(myId) == 0) {
                    _selfRank = i;
                }
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



