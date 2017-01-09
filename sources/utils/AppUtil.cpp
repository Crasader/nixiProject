//
//  AppUtil.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "AppUtil.h"
#include "apptools/HelperFunc.h"

#define PATH_PIC        "res/pic"
#define PATH_PLIST      "plist"

void AppUtil::setNewObj(CCObject* old, CCObject* anew) {
    if (NULL == anew || nullptr == anew) {
        CC_SAFE_RELEASE(old);
        old = anew;
    }
    else if (old != anew) {
        CC_SAFE_RETAIN(anew);
        CC_SAFE_RELEASE(old);
        old = anew;
    }
}

char* AppUtil::read_xxtea_file(const char* filename) {
    unsigned long file_size = 0;
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
    char* result = (char*)CZHelperFunc::getFileData(fullPath.c_str(), "rb", &file_size);
    return result;
}

char* AppUtil::read_file(const char* filename) {
    CCLOG("AppUtil::reader_file:: %s", filename);
//    unsigned long file_size = 0;
//    char* file_data = (char*)CCFileUtils::sharedFileUtils()->getFileData(filename, "rb", &file_size);
//    return file_data;
    return AppUtil::read_xxtea_file(filename);
}

CSJson::Value AppUtil::read_json_file(const char* filename) {
    char* file_data = AppUtil::read_file(filename);
    CSJson::Reader reader;
    CSJson::Value root;
    if (! reader.parse(file_data, root, false)) {
        CCLOG("AppUtil::read_json_file(%s) error!~", filename);
        return CSJson::nullValue;
    }
    return root;
}

CCObject* AppUtil::object_with_json(CSJson::Value root) {
    if (root.isObject()) { // 对象
        CCDictionary* dic = CCDictionary::create();
        std::vector<std::string> keys = root.getMemberNames();
        for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
            std::string key = (std::string)* iterator;
            //            CCLOG("std::string key == %s", key.c_str());
            CSJson::Value value = root.get(key, value.jsonNull);
            if (value != value.jsonNull) {
                CCObject* pObj = AppUtil::object_with_json(value);
                if (pObj) {
                    dic->setObject(pObj, key);
                }
            }
        }
        
        return dic;
    }
    else if (root.isArray()) { // 数组
        CCArray* array = CCArray::create();
        for (CSJson::ValueIterator iterator = root.begin(); iterator != root.end(); iterator++) {
            CSJson::Value value = (CSJson::Value)* iterator;
            CCObject* pObj = AppUtil::object_with_json(value);
            if (pObj) {
                array->addObject(pObj);
            }
        }
        
        return array;
    }
    else if (root.isString()) {
        CCString* str = CCString::create(root.asString());
        return str;
    }
    else if (root.isBool()) {
        CCBool* value = CCBool::create(root.asBool());
        return value;
    }
    else if (root.isInt() || root.isUInt()) {
        CCInteger* value = CCInteger::create(root.asInt());
        return value;
    }
    else if (root.isDouble()) {
        CCDouble* value = CCDouble::create(root.asDouble());
        return value;
    }
    else {
        CCLOG("There is a NULL ... in AppUtil::object_with_json(CSJson::Value)");
        return NULL;
    }
}

CCArray* AppUtil::array_with_json(CSJson::Value root) {
    return (CCArray*)AppUtil::object_with_json(root);
}

CCDictionary* AppUtil::dictionary_with_json(CSJson::Value root) {
    return (CCDictionary*)AppUtil::object_with_json(root);
}

void AppUtil::sort_string_array(cocos2d::CCArray* arr) {
    int size = arr->count();
    for (int i = 0; i < size - 1; ++i) {
        CCString* pA = (CCString* )arr->objectAtIndex(i);
        for (int j = i + 1; j < size; ++j) {
            CCString* pB = (CCString* )arr->objectAtIndex(j);
            if (pA->compare(pB->getCString()) > 0) {
                arr->exchangeObjectAtIndex(i, j);
                
            }
        }
    }
}

CCSprite* AppUtil::get_self_sprite(const char* file) {
    CCSprite* spt = CCSprite::create(file);
    CCSize size = spt->boundingBox().size;
    spt->setAnchorPoint(ccp(0.5f, 0.5f));
    spt->setPosition(ccp(size.width * 0.5f, size.height * 0.5f));
    return spt;
}

CCAnimation* AppUtil::animationWithFrame(const char* frameFormat, int frameCount,
                                         int startNum, float interval) {
    if ((frameCount - startNum) < 0) {
        return NULL;
    }
    
    char str[64] = {0};
    CCArray* arrFrame = CCArray::createWithCapacity(frameCount);
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    for (int i = startNum; i <= frameCount; i++) {
        sprintf(str, frameFormat, i);
        arrFrame->addObject(cache->spriteFrameByName(str));
    }
    //    MZLog("arrFrame count = %d", arrFrame->count());
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame, interval);
    return animation;
}

CCAnimation* AppUtil::animationWithPics(const char *picFormat, int picCount,
                                        int startNum, float interval) {
    if ((picCount - startNum) < 0) {
        return NULL;
    }
    
    CCString* str_pic = CCString::createWithFormat(picFormat, startNum);
    //    MZLog("res/pic = %s", str_pic->getCString());
    CCSprite* spt = CCSprite::create(str_pic->getCString());
    CCRect spt_rect = spt->getTextureRect();
    char str[64] = {0};
    CCArray* arrFrame = CCArray::createWithCapacity(picCount);
    for (int i = startNum; i <= picCount; i++) {
        sprintf(str, picFormat, i);
        CCSpriteFrame* frame = CCSpriteFrame::create(str, spt_rect);
        arrFrame->addObject(frame);
    }
    //    MZLog("arrFrame count = %d", arrFrame->count());
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(arrFrame, interval);
    return animation;
}

CCActionInterval* AppUtil::action_expand_fade_out(float duration, float interval, float scale_range) {
    CCSpawn* spawn = CCSpawn::create(CCScaleTo::create(duration, scale_range), CCFadeOut::create(duration), NULL);
    CCSequence* seq = CCSequence::create(CCScaleTo::create(0.f, 1.f), CCFadeTo::create(0.f, 0), spawn, CCDelayTime::create(interval), NULL);
    return seq;
}

CCRenderTexture* AppUtil::saveScreenAsRenderTexture() {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCRenderTexture* render = CCRenderTexture::create(winSize.width, winSize.height);
    
    render->begin();
    CCDirector::sharedDirector()->drawScene();
    render->end();
    
    return render;
}

void AppUtil::operate_all_scheduel_and_actions(CCNode* node, NodeOperateType type) {
    if (node->isRunning()) {
        switch (type) {
            case e_operate_pause: {
                node->pauseSchedulerAndActions();
            } break;
                
            case e_operate_resume: {
                node->resumeSchedulerAndActions();
            } break;
                
            case e_operate_stop: {
                node->stopAllActions();
                node->unscheduleAllSelectors();
            } break;
                
            default:
                break;
        }
    }
    
    CCArray* arr = node->getChildren();
    CCObject* iterator = NULL;
    CCARRAY_FOREACH(arr, iterator) {
        CCNode* child = (CCNode* )iterator;
        AppUtil::operate_all_scheduel_and_actions(child, type);
    }
}

void AppUtil::split(std::string src, const char* token, vector<string>& vect) {
    size_t nend=0;
    size_t nbegin=0;
    while(nend != std::string::npos)
    {
        nend = src.find(token, nbegin);
        if(nend == -1)
            vect.push_back(src.substr(nbegin, src.length()-nbegin));
        else
            vect.push_back(src.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen(token);
    }
}

size_t AppUtil::countOfSubstr(std::string src, const char *token) {
    //查找s 中flag 出现的所有位置。
    size_t pos = 0;
    size_t count = 1;
    while((pos = src.find_first_of(token, pos)) != string::npos)
    {
        pos++;
        count++;
    }
    return count;
}

