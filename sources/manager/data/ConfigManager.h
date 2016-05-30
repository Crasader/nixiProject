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
    
protected:
private:
    bool            _hasconfig;
    void conf_login_addr(int net);
};

#endif /* ConfigManager_hpp */
