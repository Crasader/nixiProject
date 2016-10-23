//
//  ConfigManager.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef ConfigManager_hpp
#define ConfigManager_hpp

#include "cocos2d.h"

USING_NS_CC;

#define CONFIG         ConfigManager::Inst()

class ConfigManager
{
public: // Export
    // 任务
    int mission_count(int phase);
    int mission_index_in_phase(int mission, int phase);
    int phase_up_required(int phase);       // 某公司阶段的升级总星要求
    CCArray* mission();
    
    // 颜文字表
    const vector<string> emoticon();
    // 换装任务对话
    CCDictionary* missionDialog();
    CCArray* getMissionDialog(int phase, int taskID);
    // 神秘人物对话
    CCArray* mysteryDialog(const char* taskId);
    // 衣服
    CCDictionary* clothes();
    // 登录
    bool has_saved_uuid();
    void save_uuid(const char* str);
    string saved_uuid();
    bool has_saved_account();
    
    int saved_login_type();     // 1游客；2账号
    string saved_account();
    string saved_password();
    void save_login_type(int type);
    void save_account(const char* str);
    void save_password(const char* str);

public:
    ~ConfigManager();
    static ConfigManager* Inst();
    void config();
    
public:
    string                  version;
    int                     netId;      // 显示用
    int                     channelId;  // 渠道标示, 0 = iOS, 易接 = 1xx;
    int                     baiOrYijie; // 白包 = 0, 易接 = 1;
    int                     openPay;    // 不开支付 = 0, 开支付 = 1;
    string                  login_addr;
    string                  chator_addr;
    
public: //test
    void test_mission_count();
    void test_phase_up_required();
    
private:
    void conf_login_addr(int net);
    void conf_chat_addr(int chat);
    void conf_mission();
    void conf_mission_dialog(int phase);
    void conf_mystery_dialog();
    void conf_clothes();
    void conf_signin7Template();

private:
    bool            _hasconfig;
    CCArray*        _mission;
    CCDictionary*   _missionDialog;
    CCDictionary*   _mysteryDialog;
    CCDictionary*   _clothes;
    vector<string>  _emoticon;
};

#endif /* ConfigManager_hpp */
