//
//  NetManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "NetManager.h"
#include "DataManager.h"

#include "json_lib.h"
using namespace CSJson;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "network/CCNetwork.h"
USING_NS_CC_EXTRA;
#endif

static NetManager* _instance = nullptr;

const int CONNECT_TIMEOUT = 60;
const char* LOGIN_ADDR = "http://192.168.1.234:9765/account";

NetManager::~NetManager() {
    
}

NetManager* NetManager::Inst() {
    if (_instance == nullptr) {
        _instance = new NetManager();
    }
    
    return _instance;
}

const char* NetManager::generate_sign() {
    CCString* rtn = CCString::createWithFormat("%ld", DataManager::Inst()->cur_timestamp());
    return rtn->getCString();
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
    std::string response = request->getResponseString();
    CCLOG("NetManager::requestFinished() -\n%s", response.c_str());
//    if (_delegate) {
//        _delegate->http_response_handle(response_string.c_str());
//    }
//    else {
//        this->response_handle(response_string.c_str());
//    }
}

void NetManager::requestFailed(CCHTTPRequest *request)
{
    CCLOG("NetManager::requestFailed() -\nError: %d", request->getErrorCode());
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

void NetManager::fast_login_9008(const char* uuid) {
    DataManager* dm = DataManager::Inst();
    CCString* url = this->obtain_login_url(dm->obtain_sid(), 9007, this->generate_sign());
    
//    FastWriter writer;
//    Value root;
//    root["uuid"] = uuid;
//    string data = writer.write(root);
    string data = "give a cuowu";
    this->post_data(url->getCString(), data);
}




