//
//  StoryComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "StoryComp.h"
#include "AppUtil.h"

StoryComp::~StoryComp() {
    CC_SAFE_DELETE(_cmpleted_story);
}

bool StoryComp::init() {
    _cmpleted_story = NULL;

    return true;
}

void StoryComp::init_with_json(Value json) {
    if (json.type() == nullValue && !json.isObject()) {
        CCLOG("<ERROR> MissionComp::init_with_json() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_cmpleted_story);
    _cmpleted_story = AppUtil::dictionary_with_json(json);
    _cmpleted_story->retain();
}

CCDictionary* StoryComp::cmpleted_story() {
    return _cmpleted_story;
}

bool StoryComp::has_init_story() {
    return !(_cmpleted_story == NULL);
}

bool StoryComp::has_passed(const char *story_index) {
    bool rtn = false;
    CCObject* pObj = _cmpleted_story->objectForKey(story_index);
    if (pObj) {
        CCArray* arr = (CCArray* )pObj;
        CCObject* p = NULL;
        CCARRAY_FOREACH(arr, p) {
            CCString* str = (CCString* )p;
            if (str->compare("-1") == 0) {
                rtn = true;
            }
        }
    }
    
    return rtn;
}

CCArray* StoryComp::story_achievments(const char *story_index) {
    CCArray* rtn = NULL;
    CCObject* pObj = _cmpleted_story->objectForKey(story_index);
    if (pObj) {
        rtn = (CCArray* )pObj;
    }
    
    return rtn;
}



