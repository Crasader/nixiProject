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
    CCArray* clothes();
    
private:
    void conf_login_addr(int net);
    void conf_mission();
    void conf_clothes();

private:
    bool            _hasconfig;
    CCArray*        _mission;
    CCArray*        _clothes;
};

#endif /* ConfigManager_hpp */
