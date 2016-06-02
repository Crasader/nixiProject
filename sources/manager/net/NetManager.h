//
//  NetManager.hpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#ifndef NetManager_hpp
#define NetManager_hpp

#include "cocos2d.h"
#include "network/CCHTTPRequest.h"

USING_NS_CC;
USING_NS_CC_EXTRA;

#define NET         NetManager::Inst()

typedef enum
    { e_NetEnv_NotReachable = 0
    , e_NetEnv_ViaWiFi = 1
    , e_NetEnv_ViaWWAN = 2
    , e_NetEnv_Unknown = 3
} NetEnv;

class NetManager : public CCHTTPRequestDelegate
{
public:
    ~NetManager();
    static NetManager* Inst();
    
    virtual void requestFinished(CCHTTPRequest* request);
    virtual void requestFailed(CCHTTPRequest* request);
    
public:
    NetEnv obtain_net_env();
    
    void fast_login_900(const char* uuid);
    void login_game_server_902();
    void check_news_910();
    
    void recommend_stranger_802();
    void send_message_803(const char* other_sid, int type);
    void all_messages_804();
    void response_message_805(int id, int oper);      // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    void search_other_806(const char* other_sid);
    
    void all_mails_700();
    void response_mail_701(int id, int oper); // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    
    void completed_mission_600();
    void start_mission_601(int id);
    void commit_mission_602();
    
    void completed_story_500();
    
    void all_products_100();
    
private:
    string generate_sign(int cid, const char* data);
    CCString* obtain_login_url(const char* sid, int cid, string sign);
    CCString* obtain_game_url(const char* sid, int cid, string sign);
    void post_data(int cid, string data);
};

#endif /* NetManager_hpp */
