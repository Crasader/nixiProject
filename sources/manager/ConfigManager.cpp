//
//  ConfigManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "ConfigManager.h"
#include "AppUtil.h"

#define UD_ACCOUNT      "UDA"
#define UD_PASSWORD     "UDP"
#define UD_UUID         "UUID"

static ConfigManager* _instance = nullptr;

ConfigManager::~ConfigManager() {
    CC_SAFE_DELETE(_mission);
    CC_SAFE_DELETE(_clothes);
    CC_SAFE_DELETE(_missionDialog);
    CC_SAFE_DELETE(_signin7Template);
}

ConfigManager* ConfigManager::Inst() {
    if (_instance == nullptr) {
        _instance = new ConfigManager();
        _instance->_hasconfig = false;
        _instance->_mission = nullptr;
        _instance->_clothes = nullptr;
        _instance->_signin7Template = nullptr;
    }
    
    return _instance;
}

void ConfigManager::config() {
    if (_hasconfig) {
        return;
    }
    
    const char* config_file = "conf/conf";
    CSJson::Value config = AppUtil::read_json_file(config_file);
    
    int net = config["net"].asInt();
    this->conf_login_addr(net);
    
    int chat = config["chat"].asInt();
    this->conf_chat_addr(chat);
    
    this->conf_mission();
    _missionDialog = CCDictionary::create();
    _missionDialog->retain();
    this->conf_clothes();
    
    _hasconfig = true;
}

int ConfigManager::mission_index_in_phase(int mission, int phase) {
    int countAllOldPhase = 0;
    if (_mission) {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(_mission, pObj) {
            CCDictionary* phaseMission = (CCDictionary*)pObj;
            CCString* phaseInMission = (CCString*)phaseMission->objectForKey("phase");
            if (phaseInMission->intValue() < phase) {
                countAllOldPhase += 1;
            }
        }
    }
    
    return MAX(mission - countAllOldPhase, 0);
}

int ConfigManager::phase_up_required(int phase) {
    int rtn = 0;
    if (_mission) {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(_mission, pObj) {
            CCDictionary* mission = (CCDictionary*)pObj;
            CCString* phaseInMission = (CCString*)mission->objectForKey("phase");
            if (phaseInMission->intValue() == phase) {
                int gradeup = ((CCString*)mission->objectForKey("upgrade"))->intValue();
                if (gradeup > rtn) {
                    rtn = gradeup;
                }
            }
        }
    }
    
    return rtn;
}

int ConfigManager::mission_count(int phase) {
    int rtn = 0;
    if (_mission) {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH(_mission, pObj) {
            CCDictionary* mission = (CCDictionary*)pObj;
            CCString* phaseInMission = (CCString*)mission->objectForKey("phase");
            if (phaseInMission->intValue() == phase) {
                rtn += 1;
            }
        }
    }
    
    return rtn;
}

CCArray* ConfigManager::mission() {
    return _mission;
}

CCDictionary* ConfigManager::missionDialog() {
    return _missionDialog;
}

CCDictionary* ConfigManager::clothes() {
    return _clothes;
}

// phase 当前阶段  taskID 任务的ID
CCArray* ConfigManager::getMissionDialog(int phase, int taskID) {
    CCObject* pObj = _missionDialog->objectForKey(phase);
    if (! pObj) {
        conf_mission_dialog(phase);
        pObj = _missionDialog->objectForKey(phase);
    }
    if (pObj) {
        CCArray* phaseMission = (CCArray*)pObj;
        CCArray* rtn = CCArray::create();
        CCObject* obj = NULL;
        CCARRAY_FOREACH(phaseMission, obj) {
            CCDictionary* dialog = (CCDictionary*)obj;
            if (dialog->valueForKey("taskID")->intValue() == taskID) {
                CCLOG("%s", dialog->valueForKey("said")->getCString());
                rtn->addObject(dialog);
            }
        }
        
        return rtn;
    }
    
    return NULL;
}

bool ConfigManager::has_saved_uuid() {
    return !(this->saved_uuid().compare("") == 0);
}

string ConfigManager::saved_uuid() {
    return CCUserDefault::sharedUserDefault()->getStringForKey(UD_UUID);
}

void ConfigManager::save_uuid(const char *str) {
    CCUserDefault::sharedUserDefault()->setStringForKey(UD_UUID, str);
}

bool ConfigManager::has_saved_account() {
    return !(this->saved_account().compare("") == 0);
}

string ConfigManager::saved_account() {
    return CCUserDefault::sharedUserDefault()->getStringForKey(UD_ACCOUNT);
}

string ConfigManager::saved_password() {
    return CCUserDefault::sharedUserDefault()->getStringForKey(UD_PASSWORD);
}

void ConfigManager::save_account(const char *str) {
    CCUserDefault::sharedUserDefault()->setStringForKey(UD_ACCOUNT, str);
}

void ConfigManager::save_password(const char *str) {
    CCUserDefault::sharedUserDefault()->setStringForKey(UD_PASSWORD, str);
}

CCArray* ConfigManager::signin7_template() {
    if (! _signin7Template) {
        this->conf_signin7Template();
    }
    
    return _signin7Template;
}

#pragma mark - inner API

void ConfigManager::conf_login_addr(int net) {
    switch (net) {
        case 17:
            login_addr = "http://115.28.179.17:9765/account";
            break;
        case 170:
            login_addr = "http://118.178.24.170:9765/account";
            break;
        case 234:
            login_addr = "http://192.168.1.234:9765/account";
            break;
        default:
            login_addr = "";
            break;
    }
}

void ConfigManager::conf_chat_addr(int chat) {
//        const char* addr = "ws://echo.websocket.org";
    switch (chat) {
        case 17:
            chator_addr = "ws://115.28.179.17:20168/chator";
            break;
        case 170:
            chator_addr = "ws://118.178.24.170:20168/chator";
            break;
        case 234:
            chator_addr = "ws://192.168.1.234:20168/chator";
            break;
        default:
            chator_addr = "";
            break;
    }
}

void ConfigManager::conf_mission() {
    /*  "id" : 任务ID
     *  "phase" : 公司等级或者公司阶段
     *  "tag1" : 任务标签1
     *  "match1" : 标签1匹配度 我不管
     *  "tag2" : 任务标签2
     *  "match2" : 标签2匹配度 我不管
     *  "match12" : 标签1和2都匹配
     *  "special" : 是否需要指定衣服(如果是制定就是制定衣服id，没有就是0)
     *  "rare" : 是否需要特殊衣服
     *  "name" : 任务名称
     *  "description" : 任务描述
     *  "bg" : 任务指定背景
     *
     */
    
    const char* config_file = "conf/mission";
    CSJson::Value root = AppUtil::read_json_file(config_file);
    _mission = AppUtil::array_with_json(root);
    _mission->retain();
}

void ConfigManager::conf_clothes() {
    const char* config_file = "conf/clothes";
    CSJson::Value root = AppUtil::read_json_file(config_file);
    
    CCDictionary* clothes = CCDictionary::create();
    clothes->setObject(CCArray::create(), 1);
    clothes->setObject(CCArray::create(), 2);
    clothes->setObject(CCArray::create(), 3);
    clothes->setObject(CCArray::create(), 4);
    clothes->setObject(CCArray::create(), 5);
    clothes->setObject(CCArray::create(), 6);
    clothes->setObject(CCArray::create(), 7);
    clothes->setObject(CCArray::create(), 8);
    clothes->setObject(CCArray::create(), 9);
    
    for (CSJson::ValueIterator iterator = root.begin(); iterator != root.end(); iterator++) {
        CSJson::Value value = (CSJson::Value)* iterator;
        CCDictionary* cloth = AppUtil::dictionary_with_json(value);
        if (cloth) {
            CCString* part = (CCString* )cloth->objectForKey("part");
//            CCLOG("Cloth part: %s", part->getCString());
//            CCString* id = (CCString* )cloth->objectForKey("id");
//            CCLOG("Cloth id: %s", id->getCString());
            CCArray* clothes_part = (CCArray* )clothes->objectForKey(part->intValue());
            clothes_part->addObject(cloth);
        }
    }
    
    _clothes = clothes;
    _clothes->retain();
}

void ConfigManager::conf_mission_dialog(int phase) {
    CCString* fileName = CCString::createWithFormat("mission/mission_%d", phase);
    CSJson::Value root = AppUtil::read_json_file(fileName->getCString());
    CCArray* allPahseMission = AppUtil::array_with_json(root);
    _missionDialog->setObject(allPahseMission, phase);
}

void ConfigManager::conf_signin7Template() {
    /*
    "id" : 跟第几天相关
    "type" : 类型， 如："clothes",
    "retroactive_cost" : 补签花费
    "uri" : 资源定位，type == clothes时，为衣服的id
    */
    const char* config_file = "conf/signin7";
    CSJson::Value root = AppUtil::read_json_file(config_file);
    _signin7Template = AppUtil::array_with_json(root);
    _signin7Template->retain();
}


void ConfigManager::test_mission_count() {
    for (int i = 0; i < 8; i ++) {
        int count = this->mission_count(i);
        CCLOG("Phase %d mission_count=%d", i, count);
    }
}

void ConfigManager::test_phase_up_required() {
    for (int i = 0; i < 8; i ++) {
        CCLOG("Phase: %d phase_up_required=%d", i, this->phase_up_required(i));
    }
}

void ConfigManager::test_signin7Template() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_signin7Template, pObj) {
        CCDictionary* item = (CCDictionary*)pObj;
        CCLOG("签到 id = %s", item->valueForKey("id")->getCString());
        CCLOG("签到 type = %s", item->valueForKey("type")->getCString());
        CCLOG("签到 uri = %s", item->valueForKey("uri")->getCString());
    }
}
