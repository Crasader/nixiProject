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
#include "PurchaseComp.h"
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
#include "PaperComp.h"
#include "CoffersComp.h"
#include "ChatComp.h"
#include "OperationComp.h"
#include "HomeComp.h"

USING_NS_CC;
//USING_NS_CC_EXT;
using namespace CSJson;

#define DATA         DataManager::Inst()
#define def_TiliMax 100

class DataManager : CCObject
{
public:
    ~DataManager();
    static DataManager* Inst();
    void http_response_error(int code, string msg);
    void http_response_handle(int resp_code, string response);
    time_t cur_timestamp_msec();
    time_t cur_timestamp();
    
    CCString* clothes_icon_path_with_id(int clothesId);
    CCString* clothes_icon_path_with_uri(string& uri);
    
    void relogin();

public:
    CC_SYNTHESIZE_RETAIN(LoginComp*, _login, Login);
    CC_SYNTHESIZE_RETAIN(PlayerComp*, _player, Player);
    CC_SYNTHESIZE_RETAIN(ClothesComp*, _clothes, Clothes);
    CC_SYNTHESIZE_RETAIN(PurchaseComp*, _purchase, Purchase);
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
    CC_SYNTHESIZE_RETAIN(PaperComp*, _paper, Paper);
    CC_SYNTHESIZE_RETAIN(CoffersComp*, _coffers, Coffers);
    CC_SYNTHESIZE_RETAIN(ChatComp*, _chat, Chat);
    CC_SYNTHESIZE_RETAIN(OperationComp*, _operation, Operation);
    CC_SYNTHESIZE_RETAIN(HomeComp*, _home, Home);
    
    // 国庆签到信息
    CC_SYNTHESIZE_RETAIN(CCArray*, _tempSigninTemplate, TempSigninTemplate);
    CC_SYNTHESIZE_RETAIN(CCDictionary*, _tempSigninUserdata, TempSigninUserdata);
    
    //
    CC_SYNTHESIZE(int, _autoLogin, AutoLogin);
//    CC_SYNTHESIZE(int, _loginType, LoginType);    // 1游客；2账号
    CC_SYNTHESIZE(bool, _hasLogin, HasLogin);       // 是否完成登入
    CC_SYNTHESIZE(bool, _firstOnMainScene, FirstOnMainScene);       // 是否首次进入主界面
    // 剧情选中数
    CC_SYNTHESIZE(int, chapterNumber, ChapterNumber);
    CC_SYNTHESIZE(bool, chapterNumberBool, ChapterNumberBool);
    // 剧情存储文字
    CC_SYNTHESIZE_RETAIN(CCArray* , _storyLabelArr, StoryLabelArr);
    // 下框的大小
    CC_SYNTHESIZE(CCSize, _diKuangSize, DiKuangSize);
    // 剧情快进开关
    CC_SYNTHESIZE(bool, _fastForward, FastForward);
    // 日常选中数
    CC_SYNTHESIZE(int, taskNumber, TaskNumber);
    CC_SYNTHESIZE_RETAIN(CCArray*, _taskSource, TaskSource);
    CC_SYNTHESIZE_RETAIN(CCArray*, _dataSource, DataSource);
    // 任务记录阶段
    CC_SYNTHESIZE(int, taskPhase, TaskPhase);
    // 家记录状态
    CC_SYNTHESIZE(bool, homeBool, HomeBool);
    // 房间记录状态
    CC_SYNTHESIZE(int, _houseIndex, HouseIndex);
    // 小游戏状态
    CC_SYNTHESIZE(bool, _taskGameBool2, TaskGameBool2);
    CC_SYNTHESIZE(bool, _taskGameBool3, TaskGameBool3);
    CC_SYNTHESIZE(bool, _taskGameBool4, TaskGameBool4);
    CC_SYNTHESIZE(int, _taskGameIndex4, TaskGameIndex4);
    CC_SYNTHESIZE(bool, _taskGameBool5, TaskGameBool5);
    CC_SYNTHESIZE(bool, _taskGameBool6, TaskGameBool6);
    CC_SYNTHESIZE(int, _taskGameIndex6, TaskGameIndex6);
    CC_SYNTHESIZE(int, _taskTempID, TaskTempID);
    // 初始化talkingdata数据
    CC_SYNTHESIZE(bool, _init_talkingBool, Init_TalkingBool);
    
    // 体力时间
    CC_SYNTHESIZE(int, _tili_Minute, TiliMinute);
    CC_SYNTHESIZE(int, _tili_second, TiliSecond);
    void creat_Energy_Time();
    void updataTiliTime(float dt);
    void closeTiliTime();
    
    // guide
    bool _guideBool1[4];
    bool _guideBool2[6];
    bool _guideBool3[10];
    bool _guideBool4[8];
    bool _guideBool5[5];
    bool _guideBool6[10];
    bool _guideBool7[10];
    bool _guideBool8[10];
    bool _guideBool9[10];
    CC_SYNTHESIZE(bool, _openGuideBool9, OpenGuideBool9);
    int current_guide_step();
    
    // 陌生人刷新计时
    CC_SYNTHESIZE(time_t, _refreshTimeStampe, RefreshTimeStampe);
    
    // 聊天计时
    CC_SYNTHESIZE(time_t, _beginTime, BeginTime);
    //
    
    CC_SYNTHESIZE(bool, _chatOut, ChatOut);
    
    bool could_prduce();
    
    
    // talkingData自定义事件
    void onEvent(std::string eventStr, std::string pageStr, std::string contentStr);
    void onChargeRequest(std::string orderId, std::string iapId, double currencyAmount, double virtualCurrencyAmount);
    void onChargeSuccess(std::string orderId);
    // 记录付费点
    void onPurchase(std::string clothesIdStr, int index, double money);
    // 记录赠予
    void onReward(double money, std::string contentStr);
    // 接受或进入
    void onBegin(std::string taskId);
    // 任务完成
    void onCompleted(std::string taskId);
    // 任务id处理
    CC_SYNTHESIZE(int, _taskTalkingdataID, TaskTalkingdataID);
    
private:

    
private:
    void init_data();
    void handle_protocol(int cid, Value content);
    void start_check_news();
    void update(float dt);
    
};

#endif /* DataManager_hpp */
