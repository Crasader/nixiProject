//
//  DataManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "DataManager.h"
#include <sys/time.h>

static DataManager* _instance = nullptr;

DataManager::~DataManager() {
    
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init_data();
    }
    
    return _instance;
}

void DataManager::init_data() {
    this->setLogin(LoginComp::create());
}

time_t DataManager::cur_timestamp() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec * 100000 + t.tv_usec;
    CCLOG("Now time = %ld", rtn);
    return rtn;
}

void DataManager::http_response_error(int code, string msg) {

}

void DataManager::http_response_handle(string response) {
    Reader reader;
    Value root;
    if (!reader.parse(response, root, false)) {
        CCLOG("DataManager::http_response_handle() json reader error.");
        return;
    }
    
    int code = root["code"].asInt();
    if (0 == code) {
        CCString* cid = ccs(root["cid"].asString());
        Value content = root["content"];
        this->handle_protocol(cid->intValue(), content);
    }
    else {
//        string msg = root["content"].asString();
    }
}

void DataManager::handle_protocol(int cid, Value content) {
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    const char* notif_format = "HTTP_FINISHED_%d";
    switch (cid) {
        case 900: {
            _login->init_with_json(content);
        } break;
            
        default:
            break;
    }
    
    nc->postNotification(CCString::createWithFormat(notif_format, cid)->getCString());
}



