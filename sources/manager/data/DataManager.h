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

#include "ClothesComp.h"
#include "IAPComp.h"
#include "LoginComp.h"
#include "MailComp.h"
#include "MessageComp.h"
#include "MissionComp.h"
#include "NewsComp.h"
#include "NoticeComp.h"
#include "PlayerComp.h"
#include "RankingComp.h"
#include "ShowComp.h"
#include "SigninComp.h"
#include "SocialComp.h"
#include "StoryComp.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace CSJson;

#define DATA         DataManager::Inst()

class DataManager : CCObject
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
    CC_SYNTHESIZE_RETAIN(ClothesComp*, _clothes, Clothes);
    CC_SYNTHESIZE_RETAIN(IAPComp*, _IAP, IAP);
    CC_SYNTHESIZE_RETAIN(MailComp*, _mail, Mail);
    CC_SYNTHESIZE_RETAIN(MessageComp*, _message, Message);
    CC_SYNTHESIZE_RETAIN(MissionComp*, _mission, Mission);
    CC_SYNTHESIZE_RETAIN(NewsComp*, _news, News);
    CC_SYNTHESIZE_RETAIN(NoticeComp*, _notice, Notice);
    CC_SYNTHESIZE_RETAIN(RankingComp*, _ranking, Ranking);
    CC_SYNTHESIZE_RETAIN(ShowComp*, _show, Show);
    CC_SYNTHESIZE_RETAIN(SigninComp*, _signin, Signin);
    CC_SYNTHESIZE_RETAIN(SocialComp*, _social, Social);
    CC_SYNTHESIZE_RETAIN(StoryComp*, _story, Story);
    
private:

    
private:
    void init_data();
    void handle_protocol(int cid, Value content);
    void start_check_news();
    void update(float dt);
};

#endif /* DataManager_hpp */
