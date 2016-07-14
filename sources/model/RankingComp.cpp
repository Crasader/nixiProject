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
        
        this->createArray_with_dic();
    }
}

CCDictionary* RankingComp::ranking() {
    return _ranking;
}

void RankingComp::createArray_with_dic(){
    _rankArray = CCArray::create();
    
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(_ranking, pElem){
        const char* user_id = pElem->getStrKey();
        ShowComp* show = (ShowComp*)pElem->getObject();
        int ranking = show->ranking();
        if (_rankArray->count() == 0) {
            _rankArray->addObject(CCString::createWithFormat("%s", user_id));
        }else{
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(_rankArray, pObj){
                const char* id = ((CCString* )pObj)->getCString();
                ShowComp* show_2 = (ShowComp* )_ranking->objectForKey(id);
                int ranking_2 = show_2->ranking();
                
            }
        }
    }
    
}

CCArray* RankingComp::getRankArray(){
    return _rankArray;
}
