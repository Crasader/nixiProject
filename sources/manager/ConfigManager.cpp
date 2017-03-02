//
//  ConfigManager.cpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#include "ConfigManager.h"
#include "AppUtil.h"
#include "FileManager.h"

#define UD_TYPE         "UTYPE"
#define UD_ACCOUNT      "UDA"
#define UD_PASSWORD     "UDP"
#define UD_UUID         "UUID"

static ConfigManager* _instance = nullptr;

ConfigManager::~ConfigManager() {
    CC_SAFE_DELETE(_mission);
    CC_SAFE_DELETE(_clothes);
    CC_SAFE_DELETE(_missionDialog);
}

ConfigManager* ConfigManager::Inst() {
    if (_instance == nullptr) {
        _instance = new ConfigManager();
        _instance->_hasconfig = false;
        _instance->_mission = nullptr;
        _instance->_clothes = nullptr;
        _instance->_mysteryDialog = NULL;
    }
    
    return _instance;
}

void ConfigManager::config() {
    if (_hasconfig) {
        return;
    }
    
    const char* config_file = "conf/conf";
    CSJson::Value config = AppUtil::read_json_file(config_file);
    
    version = config["version"].asString();
    
    this->channelId = config["can"].asInt();
    this->baiOrYijie = config["baiOrYijie"].asInt();
    this->openPay = config["openPay"].asInt();
    if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {
        CCAssert(this->channelId == 0, "Channel ID is wrong!");
    }
    else if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) {
        if (baiOrYijie == 0) {
            CCAssert(this->channelId == 0, "Channel ID is wrong!");
        }else{
            CCAssert(this->channelId != 0, "Channel ID is wrong!");
        }
    }
    
    netId = config["net"].asInt();
    this->conf_login_addr(netId);
    
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

const vector<string> ConfigManager::emoticon() {
    if (_emoticon.empty()) {
        _emoticon = FILEM->fetch_file_contents("conf/emot");
    }
    
    return _emoticon;
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

CCArray* ConfigManager::mysteryDialog(const char* taskId) {
    if (_mysteryDialog == NULL) {
        this->conf_mystery_dialog();
        
    }
    
    return dynamic_cast<CCArray*>(_mysteryDialog->objectForKey(taskId));
}

CCArray* ConfigManager::trystDialog(const char* taskId) {
    if (_trystDialog == NULL) {
        this->conf_tryst_dialog();
        
    }
    
    return dynamic_cast<CCArray*>(_trystDialog->objectForKey(taskId));
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

int ConfigManager::saved_login_type() {
    return CCUserDefault::sharedUserDefault()->getIntegerForKey(UD_TYPE);
}

string ConfigManager::saved_account() {
    return CCUserDefault::sharedUserDefault()->getStringForKey(UD_ACCOUNT);
}

string ConfigManager::saved_password() {
    return CCUserDefault::sharedUserDefault()->getStringForKey(UD_PASSWORD);
}


void ConfigManager::save_login_type(int type) {
    CCUserDefault::sharedUserDefault()->setIntegerForKey(UD_TYPE, type);
}

void ConfigManager::save_account(const char *str) {
    CCUserDefault::sharedUserDefault()->setStringForKey(UD_ACCOUNT, str);
}

void ConfigManager::save_password(const char *str) {
    CCUserDefault::sharedUserDefault()->setStringForKey(UD_PASSWORD, str);
}

int ConfigManager::pkSwitchTipCount() {
    int count = CCUserDefault::sharedUserDefault()->getIntegerForKey("pk_tip", 5);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("pk_tip", count - 1);
    return count;
}

#pragma mark - inner API

void ConfigManager::conf_login_addr(int net) {
    switch (net) {
        case 17:
//            login_addr = "http://115.28.179.17:9765/account";
            login_addr = "http://tgtest.yunlookgame.cn:9765/account";
            break;
        case 170:
//            login_addr = "http://118.178.24.170:9765/account";
            login_addr = "http://tiegao.yunlookgame.cn:9765/account";
            break;
        case 59:
            login_addr = "http://nixi.yunlookgame.cn:9765/account";
            break;
        case 234:
            login_addr = "http://192.168.1.234:9765/account";
            break;
        case 222:
            login_addr = "http://192.168.1.222:9765/account";
            break;
        case 127:
            login_addr = "http://127.0.0.1:9765/account";
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
//            chator_addr = "ws://tgtest.yunlookgame.cn:20168/chator";
            chator_addr = "ws://tiegao-chat.yunlookgame.cn:20168/chator";
            break;
        case 170:
//            chator_addr = "ws://tiegao.yunlookgame.cn:20168/chator";
            chator_addr = "ws://tiegao-chat.yunlookgame.cn:20168/chator";
            break;
        case 59:
            chator_addr = "ws://nixi.yunlookgame.cn:20168/chator";
            break;
        case 234:
            chator_addr = "ws://192.168.1.234:20168/chator";
            break;
        case 222:
            chator_addr = "ws://192.168.1.222:20168/chator";
            break;
        case 127:
            chator_addr = "ws://127.0.0.1:20168/chator";
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

void ConfigManager::conf_mystery_dialog() {
    CSJson::Value root = AppUtil::read_json_file("conf/mystery_dialog");
    _mysteryDialog = AppUtil::dictionary_with_json(root);
    _mysteryDialog->retain();
}

void ConfigManager::conf_tryst_dialog() {
    CSJson::Value root = AppUtil::read_json_file("conf/tryst_dialog");
    CC_SAFE_RELEASE(_trystDialog);
    _trystDialog = AppUtil::dictionary_with_json(root);
    _trystDialog->retain();
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
