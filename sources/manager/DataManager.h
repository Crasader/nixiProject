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
    
    //
    CC_SYNTHESIZE(int, _autoLogin, AutoLogin);
    CC_SYNTHESIZE(int, _loginType, LoginType);
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
    
    // 体力时间
    CC_SYNTHESIZE(int, _tili_Minute, TiliMinute);
    CC_SYNTHESIZE(int, _tili_second, TiliSecond);
    void creat_Energy_Time();
    void updataTiliTime(float dt);
    void closeTiliTime();
    
    // 陌生人刷新计时
    CC_SYNTHESIZE(time_t, _refreshTimeStampe, RefreshTimeStampe);
    //
    
    // 聊天计时
    CC_SYNTHESIZE(time_t, _beginTime, BeginTime);
    //
    bool could_prduce();
    
private:

    
private:
    void init_data();
    void handle_protocol(int cid, Value content);
    void start_check_news();
    void update(float dt);
    
};

#endif /* DataManager_hpp */
