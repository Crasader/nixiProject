//
//  ConfigManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "ConfigManager.h"
#include "AppUtil.h"

static ConfigManager* _instance = nullptr;

ConfigManager::~ConfigManager() {
    
}

ConfigManager* ConfigManager::Inst() {
    if (_instance == nullptr) {
        _instance = new ConfigManager();
        _instance->_hasconfig = false;
    }
    
    return _instance;
}

void ConfigManager::config() {
    if (_hasconfig) {
        return;
    }
    
    const char* config_file = "conf/conf";
    CSJson::Value config = AppUtil::read_json_file(config_file);
    
    net = config["net"].asInt();
    this->conf_login_addr(net);
    _hasconfig = true;
}

void ConfigManager::conf_login_addr(int net) {
    string addr;
    switch (net) {
        case 17:
            login_addr = "http://115.28.179.17:9765/account";
            break;
        case 234:
            login_addr = "http://192.168.1.234:9765/account";
            break;
        default:
            login_addr = "";
            break;
    }
}


