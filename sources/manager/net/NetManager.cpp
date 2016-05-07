//
//  NetManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "NetManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "network/CCNetwork.h"
USING_NS_CC_EXTRA;
#endif

static NetManager* _instance = nullptr;

const int CONNECT_TIMEOUT = 60;
const char* LOGIN_ADDR = "http://192.168.1.234:9765";

NetManager::~NetManager() {
    
}

NetManager* NetManager::Inst() {
    if (_instance == nullptr) {
        _instance = new NetManager();
    }
    
    return _instance;
}

CCString* NetManager::obtain_login_url(const char* sid, int cid, const char* sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", LOGIN_ADDR, sid, cid, sign);
    return rtn;
}

CCString* NetManager::obtain_game_url(const char* sid, int cid, const char* sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", LOGIN_ADDR, sid, cid, sign);
    return rtn;
}

void NetManager::post_data(const char* url, string data)
{
    CCLOG("NetManager::post_data() -\nurl >> %s\ndata >> %s", url, data.c_str());
    CCHTTPRequest* request = CCHTTPRequest::createWithUrl(this, url, kCCHTTPRequestMethodPOST);
    request->setPOSTData(data.c_str());
    request->setTimeout(CONNECT_TIMEOUT);
    request->start();
}

void NetManager::requestFinished(CCHTTPRequest *request)
{
    std::string response_string = request->getResponseString();
//    if (_delegate) {
//        _delegate->http_response_handle(response_string.c_str());
//    }
//    else {
//        this->response_handle(response_string.c_str());
//    }
}


void NetManager::requestFailed(CCHTTPRequest *request)
{
    CCLOG("NetManager::requestFailed() - error: %d", request->getErrorCode());
//    if (_delegate) {
//        _delegate->http_response_error(request->getErrorCode());
//    }
}

NetEnv NetManager::obtain_net_env() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return (NetEnv)CCNetwork::getInternetConnectionStatus();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return e_NetEnv_Unknown;
#endif
}

void NetManager::fast_login(const char *uuid) {
    CCString* url = this->obtain_login_url("", 9008, "");
}