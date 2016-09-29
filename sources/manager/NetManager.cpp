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

void NetManager::fast_login_900(const char* uuid, int channel) {
    FastWriter writer;
    Value root;
    root["uuid"] = uuid;
    root["type"] = 1;
    root["channel"] = channel;
    root["ver"] = "10300";
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

void NetManager::account_regist_903(const char *account, const char *password, int channel) {
    FastWriter writer;
    Value root;
    root["account"] = account;
    root["password"] = password;
    root["channel"] = channel;
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

void NetManager::update_guide_905(int guide) {
    FastWriter writer;
    Value root;
    root["guide"] = guide;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(905, data);
}

void NetManager::reset_nickname_907(const char *nickname) {
    FastWriter writer;
    Value root;
    root["nickname"] = nickname;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(907, data);
}

void NetManager::yijie_login_909() {
    FastWriter writer;
    Value root;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(909, data);
}

void NetManager::check_news_910() {
    this->post_data(910, string(""));
}


void NetManager::social_info_800() {
    this->post_data(800, string(""));
}

void NetManager::search_other_801(const char *other_sid) {
    FastWriter writer;
    Value root;
    root["other"] = other_sid;
    string data = writer.write(root);
    this->post_data(801, data);
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

void NetManager::all_friends_806() {
    this->post_data(806, string(""));
}

void NetManager::take_energy_807() {
    FastWriter writer;
    Value root;
    root["param"] = 1;
    string data = writer.write(root);
    this->post_data(807, data);
}

void NetManager::all_paper_808() {
    this->post_data(808, string(""));
}

void NetManager::send_papar_809(const char *other_sid, const char *contents) {
    FastWriter writer;
    Value root;
    root["other"] = other_sid;
    root["contents"] = contents;
    string data = writer.write(root);
    this->post_data(809, data);
}

void NetManager::delete_paper_811(int id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    string data = writer.write(root);
    this->post_data(811, data);
}

void NetManager::friend_break_813(const char *other_sid) {
    FastWriter writer;
    Value root;
    root["other"] = other_sid;
    string data = writer.write(root);
    this->post_data(813, data);
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

void NetManager::home_info_704(bool full) {
    FastWriter writer;
    Value root;
    root["full"] = full;
    string data = writer.write(root);
    this->post_data(704, data);
}

void NetManager::change_house_705(string id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(705, data);
}

void NetManager::commit_game_707(string gameId, int score) {
    FastWriter writer;
    Value root;
    root["id"] = gameId;
    root["score"] = score;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(707, data);
}


void NetManager::completed_mission_600() {
    this->post_data(600, string(""));
}

//void NetManager::start_mission_601(int id) {
//    FastWriter writer;
//    Value root;
//    root["id"] = id;
//    string data = writer.write(root);
//    this->post_data(601, data);
//}

void NetManager::commit_mission_603(int id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    string data = writer.write(root);
    this->post_data(603, data);
}

void NetManager::commit_extra_mission_605(int id, int type, int flag) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["type"] = type;
    root["flag"] = flag;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(605, data);
}


void NetManager::completed_story_500() {
    this->post_data(500, string(""));
}

void NetManager::start_story_501(const char* id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    string data = writer.write(root);
    this->post_data(501, data);
}

void NetManager::commit_story_503(const char* id, const char* flag) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["flag"] = flag;
    string data = writer.write(root);
    this->post_data(503, data);
}

void NetManager::completed_story2_504() {
    this->post_data(504, string(""));
}

void NetManager::buy_story2_505(const char *id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(505, data);
}

void NetManager::commit_story2_507(const char *id, const char* flag) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["flag"] = flag;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(507, data);
}

void NetManager::start_story2_509(const char *id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(509, data);
}

void NetManager::submit_story_danmaku_511(const char* storyId, const char* lineId, const char* words) {
    FastWriter writer;
    Value root;
    root["id"] = storyId;
    root["line"] = lineId;
    root["words"] = words;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(511, data);
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
        if (strcmp(key, "7") == 0) {
            CCDictionary* part7 = (CCDictionary* )dressed->objectForKey(key);
            int count = part7->count();
            CCArray* keys = part7->allKeys();
            for (int i = 0; i < count; i++) {
                CCString* key2 = (CCString* )keys->objectAtIndex(i);
                root["ornaments"][key2->getCString()] = ((CCInteger* )part7->objectForKey(key2->getCString()))->getValue();
            }
        }
        else {
            root["ondress"][key] = ((CCInteger* )dressed->objectForKey(key))->getValue();
        }
    }
//    CCDictElement* pElem = NULL;
//    CCDICT_FOREACH(dressed, pElem) {
//        const char* key = pElem->getStrKey();
//        if (strcmp(key, "7") == 0) {
//            CCDictionary* part7 = (CCDictionary* )dressed->objectForKey(key);
//            int count = part7->count();
//            CCArray* keys = part7->allKeys();
//            for (int i = 0; i < count; i++) {
//                CCString* key2 = (CCString* )keys->objectAtIndex(i);
//                root[key][key2->getCString()] = ((CCInteger* )part7->objectForKey(key2->getCString()))->getValue();
//            }
//        }
//        else {
//            root[key] = ((CCInteger* )dressed->objectForKey(key))->getValue();
//        }
//    }
    string data = writer.write(root);
    this->post_data(401, data);
}


void NetManager::ranking_list_300() {
    this->post_data(300, string(""));
}

void NetManager::take_energy_reward_301() {
    FastWriter writer;
    Value root;
    root["id"] = 15;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(301, data);
}

void NetManager::signin7_info_302(bool full) {
    FastWriter writer;
    Value root;
    root["full"] = full;
    string data = writer.write(root);
    this->post_data(302, data);
}

void NetManager::perform_signin7_303(string id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(303, data);
}

void NetManager::purchase_achievement_info_304(bool full) {
    FastWriter writer;
    Value root;
    root["full"] = full;
    string data = writer.write(root);
    this->post_data(304, data);
}

void NetManager::take_purchase_achievement_305(string &id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(305, data);
}

void NetManager::gashapon_info_306(bool full) {
    FastWriter writer;
    Value root;
    root["full"] = full;
    string data = writer.write(root);
    this->post_data(306, data);
}

void NetManager::single_lottery_307() {
    FastWriter writer;
    Value root;
    root["id"] = 1;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(307, data);
}

void NetManager::multiply_lottery_309() {
    FastWriter writer;
    Value root;
    root["id"] = 1;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(309, data);
}

void NetManager::exchange_clothes_311(string clothesId) {
    FastWriter writer;
    Value root;
    root["id"] = clothesId;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(311, data);
}

void NetManager::daily_share_321() {
    FastWriter writer;
    Value root;
    root["id"] = 1;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(321, data);
}

void NetManager::take_gift_333(int ChannelId, const char *code) {
    FastWriter writer;
    Value root;
    root["channel"] = ChannelId;
    root["code"] = code;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(333, data);
}

void NetManager::temp_signin_info_340() {
    this->post_data(340, string(""));
}

void NetManager::perform_temp_signin_341(string id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(341, data);
}


void NetManager::coffers_info_200(bool full) {
    FastWriter writer;
    Value root;
    root["full"] = full;
    string data = writer.write(root);
    this->post_data(200, data);
}

void NetManager::collect_coin_201() {
    FastWriter writer;
    Value root;
    root["id"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(201, data);
}

void NetManager::take_income_203() {
    FastWriter writer;
    Value root;
    root["id"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(203, data);
}

void NetManager::take_company_reward_205(string id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(205, data);
}


void NetManager::all_products_100() {
    this->post_data(100, string(""));
}

void NetManager::buy_energy_101() {
    FastWriter writer;
    Value root;
    root["id"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(101, data);
}

void NetManager::exchange_coin_103() {
    FastWriter writer;
    Value root;
    root["id"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(103, data);
}

void NetManager::verify_order_android_105(string &orderId, string &productId) {
    FastWriter writer;
    Value root;
    root["order_id"] = orderId;
    root["product_id"] = productId;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(105, data);
}

void NetManager::verify_order_iOS_107(string &orderId, string &productId) {
    FastWriter writer;
    Value root;
    root["order_id"] = orderId;
    root["product_id"] = productId;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(107, data);
}

void NetManager::buy_fee_story_109(const char* id, string &orderId) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["order_id"] = orderId;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(109, data);
}

void NetManager::buy_fee_story2_111(const char* id, string &orderId) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["order_id"] = orderId;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(111, data);
}

void NetManager::buchang_story2_113(const char *id) {
    FastWriter writer;
    Value root;
    root["id"] = id;
    root["extra"] = (int)DATA->cur_timestamp();
    string data = writer.write(root);
    this->post_data(113, data);
}

