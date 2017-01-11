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
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXTRA;
using namespace CSJson;

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
    //
    void get_info_999();
    // channel : iOS = 0, 易接 = 1xx;
    void fast_login_900(const char* uuid, int channel);
    // 直接输入登录才用到
    void account_login_901(const char* account, const char* password);
    void account_regist_903(const char* account, const char* password, int channel);
    //
    void login_game_server_902();
    void save_nickname_904(const char* nickname);
    void update_guide_905(int guide);
    void reset_nickname_907(const char* nickname);
    
    void yijie_login_909(); // 易接专用
    
    void check_news_910();
    
    void social_info_800();
    void search_other_801(const char* other_sid);
    void recommend_stranger_802();
//    typedef enum {
//        e_Msg_Friend_Ask = 1,   // 加好友消息的类型
//        e_Msg_Send_Energy,      // 送体力消息的类型
//        e_Msg_Friend_Deletem,   // 当删除好友时消息的类型
//    } MesssageType;
    void send_message_803(const char* other_sid, int type);
    void all_messages_804();
    void response_message_805(CSJson::UInt64, int oper);      // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    void all_friends_806();
    void take_energy_807();
    void all_paper_808();
    void send_papar_809(const char* other_sid, const char* contents);
    void delete_paper_811(CSJson::UInt64 id);
    void friend_break_813(const char* other_sid);
    
    // 比拼
    void competition_info_820();
    void competition_search_opponent_821();
    void competition_bless_823();
    void competition_start_825();
    void competition_prepare_827();
    
    void all_mails_700();
    void response_mail_701(CSJson::UInt64 id, int oper);         // oper： 1接受(完成后自动删除)；2不做处理，直接删除；
    void home_info_704(bool full);
    void change_house_705(string id);
    void commit_game_707(string gameId, int score);
    
    void completed_mission_600();
//    void start_mission_601(int id);
    void commit_mission_603(int id);
    // id: 任务index; type: 对应next 有效值(2,3,4,5,6); flag: 附加结果，比如小游戏分数。
    void commit_extra_mission_605(int id, int type, int flag);
    
    // 神秘事件
    void fetch_mystery_info_610(bool full);
    void start_mystery_611(const char* category);
    void commit_mystery_613(const char* category);
    void take_mystery_achv_615(const char* category, const char* achvId);
    
    
    // 约会
    void fetch_tryst_info_620();
    void take_tryst_task_621();
    void start_tryst_task_623();
    void receive_tryst_reward_625();
    
    // 福利
    void welfare_info_630();
    void take_welfare_item_reward_631(const char* itemId);
    void take_welfare_total_reward_633(const char* progressId);
    
    //成就
    void achievement_info_640(bool full);
    void take_achievement_reward_641(int achieveId);
    
    
    void completed_story_500();
    void start_story_501(const char* id);
    void commit_story_503(const char* id, const char* flag);
    void completed_story2_504();
    void buy_story2_505(const char* id); // 由于Android白包可能使用，遂iOS弃用，使用下面新的接口
    void buy_ios_story2_515(const char* id); // Only for iOS new
    void commit_story2_507(const char* id, const char* flag);
    void start_story2_509(const char* id);
    // 发表弹幕：storyId-剧情ID, lineId-第几句的id, words-发表内容
    void submit_story_danmaku_511(const char* storyId, const char* lineId, const char* words);
    
    void owned_clothes_400();
    void save_dressed_401(CCDictionary* dressed);  // 保留以兼容旧版
    void new_save_dressed_403(CCDictionary* dressed);
    
    void ranking_list_300();
    // 每日体力奖励
    void take_energy_reward_301();
    void signin7_info_302(bool full);
    void perform_signin7_303(string id);
    
    void purchase_achievement_info_304(bool full);
    void take_purchase_achievement_305(string& id);
    
    void gashapon_info_306(bool full);
    void single_lottery_307();
    void multiply_lottery_309();
    void exchange_clothes_311(string clothesId);
    
    void daily_share_321();             // 每日分享
    void commit_daily_signin_323();     // 每日签到
    void commit_identity_325();         // 完善资料

    void take_gift_333(int ChannelId, const char* code); // 礼包兑换
    
    void temp_signin_info_340();    // 节日临时签到信息
    void perform_temp_signin_341(string id);
    
    void coffers_info_200(bool full);
    void collect_coin_201();
    void take_income_203();
    void take_company_reward_205(string id);
    
    void all_products_100();
    void buy_energy_101();
    void exchange_coin_103();
    void verify_order_android_105(string& orderId, string& productId);
    // 107又给Andoird用了，iOS换乘133
    void verify_order_iOS_107(string& orderId, string& productId);
    // 4 Andoird 旧版使用
    void buy_fee_story_109(const char* id, string& orderId);
    // 4 Andoird 新版使用
    void buy_fee_story2_111(const char* id, string& orderId);
    // 4 Andoird 补偿使用
    void buchang_story2_113(const char* id);
    // iOS新支付验证
    void verify_order_iOS_133(string& orderId, string& productId, const char* receipt);
    
private:
    string generate_sign(int cid, const char* data);
    CCString* obtain_login_url(const char* sid, int cid, string sign);
    CCString* obtain_game_url(const char* sid, int cid, string sign);
    void post_data(int cid, string data);
};

#endif /* NetManager_hpp */
