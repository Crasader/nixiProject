//
//  DataManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "DataManager.h"
#include "NetManager.h"
#include "ConfigManager.h"
#include <sys/time.h>
#include "AppUtil.h"
#include "Loading.h"
#include "PromptLayer.h"

#include "LoginScene.h"

static DataManager* _instance = nullptr;

const float UpdateInterval = 60.0f;

DataManager::~DataManager() {
    CC_SAFE_RELEASE_NULL(_dataSource);
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init_data();
    }
    
    return _instance;
}

void DataManager::init_data() {
    this->setLogin(LoginComp::create());
    this->setPlayer(PlayerComp::create());
    this->setClothes(ClothesComp::create());
    this->setIAP(IAPComp::create());
    this->setMail(MailComp::create());
    this->setMessage(MessageComp::create());
    this->setMission(MissionComp::create());
    this->setNews(NewsComp::create());
    this->setNotice(NoticeComp::create());
    this->setRanking(RankingComp::create());
    this->setShow(ShowComp::create());
    this->setSignin(SigninComp::create());
    this->setSocial(SocialComp::create());
    this->setStory(StoryComp::create());
}

time_t DataManager::cur_timestamp() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec * 100000 + t.tv_usec;
    CCLOG("Now time = %ld", rtn);
    return rtn;
}

void DataManager::http_response_error(int code, string msg) {
    
}

void DataManager::http_response_handle(int resp_code, string response) {
    if (200 != resp_code) {
        CCLOG("DataManager::http_response_handle() - HTTP response status code: %d.", resp_code);
        return;
    }
    
    Reader reader;
    Value root;
    if (!reader.parse(response, root, false)) {
        CCLOG("DataManager::http_response_handle() json reader error.");
        return;
    }
    
    int code = root["code"].asInt();
    int cid = root["cid"].asInt();
    if (0 == code) {
        Value content = root["content"];
        this->handle_protocol(cid, content);
    }
    else {
        LOADING->remove();
        const char* msg = root["content"].asCString();
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), msg);
        //
        if (cid == 901 && this->getLoginType() == 2){
            CONFIG->save_account("");
            CONFIG->save_password("");
            CCDirector::sharedDirector()->replaceScene(LoginScene::scene());
        }
    }
}

void DataManager::handle_protocol(int cid, Value content) {
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    const char* notif_format = "HTTP_FINISHED_%d";
    CCObject* pData = NULL;
    switch (cid) {
        case 900: {
            _login->init_with_json(content);
        } break;
            
        case 901: {
            _login->init_with_json(content);
        } break;
            
        case 902: {
            _player->init_with_json(content["player"]);
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["dressed"]);
            _news->init_with_json(content["news"]);
            this->start_check_news();
        } break;
            
        case 903: {
            _player->init_with_json(content["player"]);
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["dressed"]);
            _news->init_with_json(content["news"]);
            this->start_check_news();
        } break;
            
        case 904: {
            _show->init_with_json(content["show"]);
        } break;
            
        case 910: {
            _news->init_with_json(content["news"]);
        } break;
            
        case 802: {
            _social->update_stranger(content["strangers"]);
        } break;
            
        case 803: {
            // 只返回OK
//            _social->init_with_json(content["social"]);
        } break;
            
        case 804: {
            _message->init_with_json(content["messages"]);
        } break;
            
        case 700: {
            _mail->init_with_json(content["mail"]);
        } break;
            
        case 701: {
            _player->init_with_json(content["player"]);
//            _mail->handle_mail_oper(content["info"]["id"].asInt(), content["info"]["oper"].asInt());
            pData = AppUtil::dictionary_with_json(content["info"]);
        } break;
            
        case 600: {
            _mission->init_with_json(content["mission"]);
        } break;
            
        case 601: {
            _player->init_with_json(content["player"]);
        } break;
            
        case 602: {
            _player->init_with_json(content["player"]);
            _mission->init_with_json(content["mission"]);
        } break;
            
        case 500: {
            _story->init_with_json(content["story"]);
        } break;
            
        case 501: {
            _player->init_with_json(content["player"]);
        } break;
            
        case 503: {
            _player->init_with_json(content["player"]);
            _story->init_with_json(content["story"]);
        } break;
            
        case 400: {
            _clothes->init_with_json(content["clothes"]);
        } break;
            
        case 401: {
            _player->init_with_json(content["player"]);
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["dressed"]);
            _clothes->update_clothes(content["newclothes"]);
        } break;
            
        case 100: {
            _IAP->init_with_json(content);
        } break;
            
        default:
            break;
    }
    
    nc->postNotification(CCString::createWithFormat(notif_format, cid)->getCString(), pData);
}

void DataManager::start_check_news() {
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DataManager::update), this);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&DataManager::update), this, UpdateInterval, kCCRepeatForever, UpdateInterval, false);
}

void DataManager::update(float dt) {
    NET->check_news_910();
}

