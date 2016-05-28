//
//  NetManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "NetManager.h"
#include "DataManager.h"
#include "IMD5.h"
#include "json_lib.h"
using namespace CSJson;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "network/CCNetwork.h"
USING_NS_CC_EXTRA;
#endif

static NetManager* _instance = nullptr;

const int CONNECT_TIMEOUT = 60;
//const char* LOGIN_ADDR = "http://192.168.1.234:9765/account";
const char* LOGIN_ADDR = "http://115.28.179.17:9765/account";

NetManager::~NetManager() {
    
}

NetManager* NetManager::Inst() {
    if (_instance == nullptr) {
        _instance = new NetManager();
    }
    
    return _instance;
}

string NetManager::generate_sign(int cid, const char* data) {
    LoginComp* login = DataManager::Inst()->getLogin();
    CCString* sign_origin = CCString::createWithFormat("%s%s%s", login->obtain_sid(), data, login->obtain_skey());
    IMD5 imd5;
    imd5.GenerateMD5((unsigned char*)sign_origin->getCString(), sign_origin->length());
    return imd5.ToString();
//    CCString* rtn = CCString::createWithFormat("%ld", DataManager::Inst()->cur_timestamp());
//    CCLOG("MD5=%s", result.c_str());
//    return result.c_str();
}

CCString* NetManager::obtain_login_url(const char* sid, int cid, string sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", LOGIN_ADDR, sid, cid, sign.c_str());
    return rtn;
}

CCString* NetManager::obtain_game_url(const char* sid, int cid, string sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", DataManager::Inst()->getLogin()->obtain_game_addr(), sid, cid, sign.c_str());
    return rtn;
}

void NetManager::post_data(int cid, string data)
{
    LoginComp* login = DataManager::Inst()->getLogin();
    CCString* url = NULL;
    if (900 == cid) {
        url = this->obtain_login_url(login->obtain_sid(), cid, this->generate_sign(cid, data.c_str()));
    }
    else {
        url = this->obtain_game_url(login->obtain_sid(), cid, this->generate_sign(cid, data.c_str()));
    }
    CCLOG("====== *** NetManager::post_data() *** ======\nurl >> %s\ndata >> %s", url->getCString(), data.c_str());
    CCHTTPRequest* request = CCHTTPRequest::createWithUrl(this, url->getCString(), kCCHTTPRequestMethodPOST);
    request->addRequestHeader("Web-Scope: mzplay");
    request->setPOSTData(data.c_str());
    request->setTimeout(CONNECT_TIMEOUT);
    request->start();
}

void NetManager::requestFinished(CCHTTPRequest *request)
{
    int resp_code = request->getResponseStatusCode();
    std::string response = request->getResponseString();
    CCLOG("NetManager::requestFinished(%d) -\n%s\n", resp_code, response.c_str());
    DataManager::Inst()->http_response_handle(resp_code, response);
}

void NetManager::requestFailed(CCHTTPRequest *request)
{
    CCLOG("NetManager::requestFailed() -\nError<%d>: %s", request->getErrorCode(), request->getErrorMessage().c_str());
    DataManager::Inst()->http_response_error(request->getErrorCode(), request->getErrorMessage());
}

NetEnv NetManager::obtain_net_env() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return (NetEnv)CCNetwork::getInternetConnectionStatus();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return e_NetEnv_Unknown;
#endif
}

void NetManager::fast_login_900(const char* uuid) {
    FastWriter writer;
    Value root;
    root["uuid"] = uuid;
    root["type"] = 1;
    string data = writer.write(root);
    this->post_data(900, data);
}

void NetManager::login_game_server_902() {
    FastWriter writer;
    Value root;
    LoginComp* login = DataManager::Inst()->getLogin();
    root["skey"] = login->obtain_skey();
    string data = writer.write(root);
    this->post_data(902, data);
}

void NetManager::recommend_stranger_802() {
    this->post_data(802, string(""));
}

void NetManager::send_message_803(const char *other_sid, int type) {
    FastWriter writer;
    Value root;
    root["other"] = other_sid;
    root["type"] = type;
    string data = writer.write(root);
    this->post_data(803, data);
}

void NetManager::all_messages_804() {
    this->post_data(804, string(""));
}

void NetManager::response_message_805(int id, int oper) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["oper"] = oper;
    string data = writer.write(root);
    this->post_data(805, data);
}

void NetManager::search_other_806(const char *other_sid) {
    FastWriter writer;
    Value root;
    root["other"] = other_sid;
    string data = writer.write(root);
    this->post_data(806, data);
}
