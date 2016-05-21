//
//  DataManager.hpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#ifndef DataManager_hpp
#define DataManager_hpp

#include "cocos2d.h"
//#include "cocos-ext.h"
#include "json_lib.h"

#include "LoginComp.h"
#include "PlayerComp.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace CSJson;


class DataManager
{
public:
    ~DataManager();
    static DataManager* Inst();
    
    void http_response_error(int code, string msg);
    void http_response_handle(int resp_code, string response);
    
    time_t cur_timestamp();

public:
    CC_SYNTHESIZE_RETAIN(LoginComp*, _login, Login);
    CC_SYNTHESIZE_RETAIN(PlayerComp*, _player, Player);
    
private:

    
private:
    void init_data();
    void handle_protocol(int cid, Value content);
};

#endif /* DataManager_hpp */
