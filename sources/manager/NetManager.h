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
    
    void fast_login_900(const char* uuid, const char* recommender = "");
    void account_login_901(const char* account, const char* password);
    void login_game_server_902();
    void account_regist_903(const char* account, const char* password, const char* recommender = "");
    void save_nickname_904(const char* nickname);
    void update_guide_905(int guide);
    
    void check_news_910();
    
    void social_info_800();
    void search_other_801(const char* other_sid);
    void recommend_stranger_802();
    void send_message_803(const char* other_sid, int type);
    void all_messages_804();
    void response_message_805(int id, int oper);      // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    void all_friends_806();
    void take_energy_807();
    void all_paper_808();
    void send_papar_809(const char* other_sid, const char* contents);
    void delete_paper_811(int id);
    
    void all_mails_700();
    void response_mail_701(int id, int oper);         // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    void home_info_704(bool full);
    void change_house_705(string id);
    void commit_game_707(string gameId, int score);
    
    void completed_mission_600();
//    void start_mission_601(int id);
    void commit_mission_603(int id);
    
    void completed_story_500();
    void start_story_501(const char* id);
    void commit_story_503(const char* id, const char* flag);
    void completed_story2_504();
    void buy_story2_505(const char* id);
    void commit_story2_507(const char* id, const char* flag);
    void start_story2_509(const char* id);
    
    void owned_clothes_400();
    void save_dressed_401(CCDictionary* dressed);
    
    void ranking_list_300();
    void take_energy_reward_301();
    void signin7_info_302(bool full);
    void perform_signin7_303(string id);
    void purchase_achievement_info_304(bool full);
    void take_purchase_achievement_305(string& id);
    void gashapon_info_306(bool full);
    void single_lottery_307();
    void multiply_lottery_309();
    void exchange_clothes_311(string clothesId);

    
    void coffers_info_200(bool full);
    void collect_coin_201();
    void take_income_203();
    void take_company_reward_205(string id);
    
    void all_products_100();
    void buy_energy_101();
    void exchange_coin_103();
    void verify_order_android_105(string& orderId, string& productId);
    
private:
    string generate_sign(int cid, const char* data);
    CCString* obtain_login_url(const char* sid, int cid, string sign);
    CCString* obtain_game_url(const char* sid, int cid, string sign);
    void post_data(int cid, string data);
};

#endif /* NetManager_hpp */
