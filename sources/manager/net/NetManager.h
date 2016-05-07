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
//#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXTRA;
//USING_NS_CC_EXT;

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
    void fast_login(const char* uuid);
    
private:

    CCString* obtain_login_url(const char* sid, int cid, const char* sign);
    CCString* obtain_game_url(const char* sid, int cid, const char* sign);
    void post_data(const char* url, string data);
};

#endif /* NetManager_hpp */
