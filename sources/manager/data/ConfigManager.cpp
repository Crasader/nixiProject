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
    CC_SAFE_DELETE(_mission);
    CC_SAFE_DELETE(_clothes);
}

ConfigManager* ConfigManager::Inst() {
    if (_instance == nullptr) {
        _instance = new ConfigManager();
        _instance->_hasconfig = false;
        _instance->_mission = nullptr;
        _instance->_clothes = nullptr;
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
    
    this->conf_mission();
    this->conf_clothes();
    
    _hasconfig = true;
}

CCArray* ConfigManager::mission() {
    return _mission;
}

CCArray* ConfigManager::clothes() {
    return _clothes;
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

void ConfigManager::conf_mission() {
    const char* config_file = "conf/mission";
    CSJson::Value root = AppUtil::read_json_file(config_file);
    _mission = AppUtil::array_with_json(root);
    _mission->retain();
}

void ConfigManager::conf_clothes() {
    const char* config_file = "conf/clothes";
    CSJson::Value root = AppUtil::read_json_file(config_file);
    _clothes = AppUtil::array_with_json(root);
    _clothes->retain();
}

