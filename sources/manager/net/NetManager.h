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
    
    void recommend_stranger_802();
    void send_message_803(const char *other_sid, int type);
    
private:
    string generate_sign(int cid, const char* data);
    CCString* obtain_login_url(const char* sid, int cid, string sign);
    CCString* obtain_game_url(const char* sid, int cid, string sign);
    void post_data(int cid, string data);
};

#endif /* NetManager_hpp */
