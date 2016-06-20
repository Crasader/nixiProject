//
//  NetManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "NetManager.h"
#include "ConfigManager.h"
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

NetManager::~NetManager() {
    
}

NetManager* NetManager::Inst() {
    if (_instance == nullptr) {
        _instance = new NetManager();
    }
    
    return _instance;
}

string NetManager::generate_sign(int cid, const char* data) {
    LoginComp* login = DATA->getLogin();
    CCString* sign_origin = CCString::createWithFormat("%s%s%s", login->obtain_sid(), data, login->obtain_skey());
    IMD5 imd5;
    imd5.GenerateMD5((unsigned char*)sign_origin->getCString(), sign_origin->length());
    return imd5.ToString();
//    CCString* rtn = CCString::createWithFormat("%ld", DATA->cur_timestamp());
//    CCLOG("MD5=%s", result.c_str());
//    return result.c_str();
}

CCString* NetManager::obtain_login_url(const char* sid, int cid, string sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", CONFIG->login_addr.c_str(), sid, cid, sign.c_str());
    return rtn;
}

CCString* NetManager::obtain_game_url(const char* sid, int cid, string sign) {
    CCString* rtn = CCString::createWithFormat("%s?sid=%s&cid=%d&sign=%s", DATA->getLogin()->obtain_game_addr(), sid, cid, sign.c_str());
    return rtn;
}

void NetManager::post_data(int cid, string data)
{
    LoginComp* login = DATA->getLogin();
    CCString* url = NULL;
    if (900 == cid || 901 == cid || 903 == cid) {
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
    DATA->http_response_handle(resp_code, response);
}

void NetManager::requestFailed(CCHTTPRequest *request)
{
    CCLOG("NetManager::requestFailed() -\nError<%d>: %s", request->getErrorCode(), request->getErrorMessage().c_str());
    DATA->http_response_error(request->getErrorCode(), request->getErrorMessage());
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

void NetManager::account_login_901(const char *account, const char *password) {
    FastWriter writer;
    Value root;
    root["account"] = account;
    root["password"] = password;
    string data = writer.write(root);
    this->post_data(901, data);
}

void NetManager::login_game_server_902() {
    FastWriter writer;
    Value root;
    LoginComp* login = DATA->getLogin();
    root["skey"] = login->obtain_skey();
    string data = writer.write(root);
    this->post_data(902, data);
}

void NetManager::account_regist_903(const char *account, const char *password) {
    FastWriter writer;
    Value root;
    root["account"] = account;
    root["password"] = password;
    string data = writer.write(root);
    this->post_data(903, data);
}

void NetManager::save_nickname_904(const char *nickname) {
    FastWriter writer;
    Value root;
    root["nickname"] = nickname;
    string data = writer.write(root);
    this->post_data(904, data);
}


void NetManager::check_news_910() {
    this->post_data(910, string(""));
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


void NetManager::all_mails_700() {
    this->post_data(700, string(""));
}

void NetManager::response_mail_701(int id, int oper) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["oper"] = oper;
    string data = writer.write(root);
    this->post_data(701, data);
}


void NetManager::completed_mission_600() {
    this->post_data(600, string(""));
}

void NetManager::start_mission_601(int id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    string data = writer.write(root);
    this->post_data(601, data);
}

void NetManager::commit_mission_602() {
    this->post_data(602, string(""));
}


void NetManager::completed_story_500() {
    this->post_data(500, string(""));
}

void NetManager::start_story_501(int id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    string data = writer.write(root);
    this->post_data(501, data);
}

void NetManager::commit_story_503(int id, const char* flag) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["flag"] = flag;
    string data = writer.write(root);
    this->post_data(503, data);
}

void NetManager::owned_clothes_400() {
    this->post_data(400, string(""));
}

void NetManager::save_dressed_401(CCDictionary *dressed) {
    FastWriter writer;
    Value root;
    CCDictElement* pElem = NULL;
    CCDICT_FOREACH(dressed, pElem) {
        const char* key = pElem->getStrKey();
        root[key] = ((CCInteger* )dressed->objectForKey(key))->getValue();
    }
    string data = writer.write(root);
    this->post_data(401, data);
}


void NetManager::all_products_100() {
    this->post_data(100, string(""));
}