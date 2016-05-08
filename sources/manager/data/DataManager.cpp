//
//  DataManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "DataManager.h"
#include <sys/time.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "native/CCNative.h"
USING_NS_CC_EXTRA;
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

static DataManager* _instance = nullptr;

DataManager::~DataManager() {
    
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
    }
    
    _instance->_sid = "";
    
    return _instance;
}

void DataManager::config_UUID() {
    if (!_uuid.empty()) {
        CCLOG("UUID 只需获取一次，请确认是否正确使用???");
        return;
    }
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    _uuid = cocos2d::extra::CCNative::getOpenUDID();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    _uuid = "没有获得";
#endif
}

const char* DataManager::obtain_UUID() {
    return _uuid.c_str();
}

const char* DataManager::obtain_sid() {
    return _sid.c_str();
}

time_t DataManager::cur_timestamp() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec * 100000 + t.tv_usec;
    CCLOG("Now time = %ld", rtn);
    return rtn;
}