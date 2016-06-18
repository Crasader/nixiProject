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
public:
    ~ConfigManager();
    static ConfigManager* Inst();
    void config();
    
public:
    int                     net;
    string                  login_addr;

public:
    CCArray* mission();
    CCDictionary* clothes();
    bool has_saved_account();
    string saved_account();
    string saved_password();
    void save_account(const char* str);
    void save_password(const char* str);
    
private:
    void conf_login_addr(int net);
    void conf_mission();
    void conf_clothes();

private:
    bool            _hasconfig;
    CCArray*        _mission;
    CCDictionary*   _clothes;
};

#endif /* ConfigManager_hpp */
