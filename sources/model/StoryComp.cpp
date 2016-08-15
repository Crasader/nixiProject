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
    _cmpleted_story = nullptr;

    return true;
}

void StoryComp::init_with_json(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> StoryComp::init_with_json() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_cmpleted_story);
    _cmpleted_story = AppUtil::dictionary_with_json(json);
    _cmpleted_story->retain();
}

void StoryComp::replace_story2_user(Value json) {
    if (json.type() == nullValue) {
        CCLOG("<ERROR> StoryComp::replace_story2_user() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_cmpleted_story2);
    _cmpleted_story2 = AppUtil::dictionary_with_json(json);
    _cmpleted_story2->retain();
}

CCDictionary* StoryComp::cmpleted_story() {
    CCAssert(_cmpleted_story != nullptr, "剧情信息尚未初始化~!");
    return _cmpleted_story;
}

bool StoryComp::has_init_story() {
    return !(_cmpleted_story == NULL);
}
bool StoryComp::has_init_story2(){
    return !(_cmpleted_story2 == NULL);
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

int StoryComp::story2_state(const char *story2_index) {
    CCInteger* state = (CCInteger*)_cmpleted_story2->objectForKey(story2_index);
    if (state == NULL) {
        return 0;
    }
    else {
        return state->getValue();
    }
}


