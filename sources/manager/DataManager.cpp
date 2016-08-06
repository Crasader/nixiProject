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
#include "Loading2.h"
#include "PromptLayer.h"

#include "LoginScene.h"

static DataManager* _instance = nullptr;

const float UpdateInterval = 60.0f;

DataManager::~DataManager() {
    CC_SAFE_RELEASE_NULL(_dataSource);
    CC_SAFE_RELEASE_NULL(_taskSource);
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init_data();
    }
    
    return _instance;
}

void DataManager::init_data() {
    this->setRefreshTimeStampe(0);
    this->setAutoLogin(true);
    
    this->setLogin(LoginComp::create());
    this->setPlayer(PlayerComp::create());
    this->setClothes(ClothesComp::create());
    this->setPurchase(PurchaseComp::create());
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
    this->setPaper(PaperComp::create());
    this->setCoffers(CoffersComp::create());
    this->setChat(ChatComp::create());
    this->setOperation(OperationComp::create());
    this->setHome(HomeComp::create());
}

time_t DataManager::cur_timestamp_msec() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec * 1000000 + t.tv_usec;
    CCLOG("timestamp msec = %ld", rtn);
    return rtn;
}

time_t DataManager::cur_timestamp() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec;
    CCLOG("timestamp sec = %ld", rtn);
    return rtn;
}

CCString* DataManager::clothes_icon_path_with_id(int clothesId) {
    CCString* rtn = NULL;
    int part = clothesId / 10000;
    if (part == 1) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/1toufa/icon%d.png", clothesId);
    }
    else if (part == 2) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/2waitao/icon%d.png", clothesId);
    }
    else if (part == 3) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/3shangyi/icon%d.png", clothesId);
    }
    else if (part == 4) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/4kuzi/icon%d.png", clothesId);
    }
    else if (part == 5) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/5wazi/icon%d.png", clothesId);
    }
    else if (part == 6) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/6xiezi/icon%d.png", clothesId);
    }
    else if (part == 7) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/7shipin/icon%d.png", clothesId);
    }
    else if (part == 8) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/8bao/icon%d.png", clothesId);
    }
    else if (part == 9) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/9zhuangrong/icon%d.png", clothesId);
    }
    else if (part == 10) {
        rtn = CCString::createWithFormat("pic/clothesScene/icon/10texiao/icon%d.png", clothesId);
    }
    else {
        rtn = CCString::create("pic/unknown.png");
    }
    
    return rtn;
}

CCString* DataManager::clothes_icon_path_with_uri(string &uri) {
    CCString* strUri = CCString::create(uri);
    int clothesId = strUri->intValue();
    return this->clothes_icon_path_with_id(clothesId);
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
    else if (11000 == code) {
        relogin();
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
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["show"]);
            _news->init_with_json(content["news"]);
            _purchase->init_purchase(content["purchase"]);
            this->start_check_news();
        } break;
            
        case 903: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["show"]);
            _news->init_with_json(content["news"]);
            _purchase->init_purchase(content["purchase"]);
            this->start_check_news();
        } break;
            
        case 904: {
            _show->init_with_json(content["show"]);
        } break;
            
        case 910: {
            _coffers->reset_collected();
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
            
        case 800: {
            _social->init_with_json(content["social"]);
        } break;
            
        case 801: {
//            ShowComp* other = ShowComp::create();
//            other->init_with_json(content["other"]);
//            pData = other;
            CCString* id = ccs(content["other"].asString());
            pData = id;
        } break;
            
        case 802: {
            _social->update_strangers(content["strangers"]);
        } break;
            
        case 803: {
            _social->init_with_json(content["social"]);
        } break;
            
        case 804: {
            _message->init_with_json(content["messages"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
            
        case 805: {
            _social->init_with_json(content["social"]);
            _message->init_with_json(content["messages"]);
        } break;
            
        case 806: {
            _social->init_friends(content["friends"]);
        } break;
            
        case 807: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _social->init_with_json(content["social"]);
            pData = AppUtil::dictionary_with_json(content["info"]);
        } break;
            
        case 808: {
            _social->init_friends(content["friends"]);
            _paper->init_with_json(content["paper"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
            
        case 809: {
            
        } break;
            
        case 811: {
            _paper->delete_paper(content["id"].asInt());
        } break;
            
        case 700: {
            _mail->init_with_json(content["mail"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
            
        case 701: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
//            _mail->handle_mail_oper(content["info"]["id"].asInt(), content["info"]["oper"].asInt());
            pData = AppUtil::dictionary_with_json(content["info"]);
        } break;
            
        case 704: {
            _home->init_house_template(content["template"]);
            _home->replace_home_info(content["home"]);
        } break;
            
        case 600: {
            _mission->init_with_json(content["mission"]);
        } break;
            
        case 601: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
        } break;
            
        case 603: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _mission->init_with_json(content["mission"]);
            // 形如：{"rating":5,"levelup":0,"coin":50,"energy":6}.
            pData = AppUtil::dictionary_with_json(content["result"]);
        } break;
        
        case 500: {
            _story->init_with_json(content["story"]);
        } break;
            
        case 501: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
        } break;
            
        case 503: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _story->init_with_json(content["story"]);
        } break;
            
        case 400: {
            _clothes->init_with_json(content["clothes"]);
        } break;
            
        case 401: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["show"]);
            _clothes->update_clothes(content["newclothes"]);
        } break;
            
        case 300: {
            _ranking->init_with_json(content["ranking"]);
        } break;
            
        case 301: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
            
            pData = CCInteger::create(content["reward"].asInt());
        } break;
            
        case 302: {
            _signin->init_signin7_template(content["template"]);
            _signin->update_signin7_info(content["signin7"]);
        } break;
            
        case 303: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _signin->update_signin7_info(content["signin7"]);
            _clothes->init_with_json(content["clothes"]);
            _news->init_with_json(content["news"]);
        } break;
            
        case 304: {
            _operation->init_purchase_achievement_template(content["template"]);
            _operation->replace_purchase_achievement(content["pAchievement"]);
        } break;
            
        case 305: {
            _operation->replace_purchase_achievement(content["pAchievement"]);
            _clothes->init_with_json(content["clothes"]);
        } break;
            
        case 306: {
            _operation->init_gashapon_template(content["template"]);
            _operation->replace_gashapon_user(content["gashapon"]);
        } break;
            
        case 200: {
            _coffers->init_with_json(content["coffers"]);
        } break;
            
        case 201: {
            _coffers->init_with_json(content["coffers"]);
        } break;
            
        case 203: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _coffers->init_with_json(content["coffers"]);
        } break;
            
        case 100: {
            _purchase->init_products(content);
        } break;
            
        case 101: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
        } break;
            
        case 103: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
        } break;
            
        case 105: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
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

void DataManager::relogin() {
    CCDirector::sharedDirector()->replaceScene(LoginScene::scene());
}

void DataManager::creat_Energy_Time(){
    
    int tim = DATA->getPlayer()->left;
    int minute = tim/60;
    int second = tim%60;
    DATA->setTiliMinute(minute);
    DATA->setTiliSecond(second);
    
    int tili = DATA->getPlayer()->energy;
    if (tili >= def_TiliMax) {
        
        CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DataManager::updataTiliTime), this);
    }else{
        
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&DataManager::updataTiliTime), this, 1, false);
    }
}
void DataManager::updataTiliTime(float dt){
    
    _tili_second--;
    if (_tili_second < 0) {
        _tili_second = 59;
        _tili_Minute--;
        if (_tili_Minute <= 0) {
            _tili_Minute = 0;
        }
    }
    
    if (_tili_Minute == 0 && _tili_second == 0) {
        int tim = 6;
        _tili_Minute = tim;
        _tili_second = 0;
        
        int energy = DATA->getPlayer()->energy;
        energy++;
        
        if (energy >= def_TiliMax) {
            energy = def_TiliMax;
            CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DataManager::updataTiliTime), this);
        }
        
        DATA->getPlayer()->energy = energy;
    }
}

void DataManager::closeTiliTime(){
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(SEL_SCHEDULE(&DataManager::updataTiliTime), this);
}

bool DataManager::could_prduce() {
//    CCLOG("1 = %d", getNews()->coin);
//    CCLOG("2 = %d", getCoffers()->collected);
    return (getNews()->coin - getCoffers()->collected) > 0;
}


