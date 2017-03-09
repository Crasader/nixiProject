//
//  DataManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "DataManager.h"
#include "NetManager.h"
#include "WSManager.h"
#include "ConfigManager.h"
#include <sys/time.h>
#include "AppUtil.h"
#include "Loading2.h"
#include "PromptLayer.h"
#include "LoginScene.h"
#include "JNIController.h"
#include "YiJieLoginScene.h"

#include "TDCCTalkingDataGA.h"
#include "TDCCVirtualCurrency.h"
#include "TDCCItem.h"
#include "TDCCMission.h"


static DataManager* _instance = nullptr;


DataManager::~DataManager() {
    CC_SAFE_RELEASE_NULL(_dataSource);
    CC_SAFE_RELEASE_NULL(_taskSource);
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init_data();
        
        _instance->openUpdata();
    }
    
    return _instance;
}

void DataManager::init_data() {
    this->setRefreshTimeStampe(0);
    this->setAutoLogin(true);
    this->setHasLogin(false);
    
    this->setChatInterval(0);
    this->setLastTime(0);
    this->setDeltaTime(0);
    
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
    this->setMystery(MysteryComp::create());
    this->setTryst(TrystComp::create());
    this->setWelfare(WelfareComp::create());
    this->setAchievement(AchievementComp::create());
    this->setCompetition(CompetitionComp::create());
    this->setFlashSaleInfo(FlashSaleInfo::create());
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
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec;
//    CCLOG("timestamp sec = %ld", rtn);
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

const CCString* DataManager::clothes_name_with_id(int clothesId){
    CCDictionary* dic = CONFIG->clothes();// 所有衣服
    int part = clothesId / 10000;
    CCArray* partClothes = (CCArray* )dic->objectForKey(part);// 获得当前类型所有衣服
    int count = partClothes->count();
    for (int i = 0; i < count; i++) {
        CCDictionary* clothDic = (CCDictionary* )partClothes->objectAtIndex(i);
        int id = clothDic->valueForKey("id")->intValue();
        if (id == clothesId) {
            return clothDic->valueForKey("name");
        }
    }
    
    return NULL;
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
        if (getHasLogin() == false){ // 账号登入失败
            this->setAutoLogin(false);
            CCDirector::sharedDirector()->replaceScene(LoginScene::scene());
        }
        
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
        this->setAutoLogin(true);
        relogin();
    }
    else {
        LOADING->remove();
        const char* msg = root["content"].asCString();
        PromptLayer* prompt = PromptLayer::create();
        prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), msg);
        // 账号登入失败
        if (cid == 901){
            this->setAutoLogin(false);
            this->setHasLogin(false);
            CCScene* loginScene = LoginScene::scene();
            CCDirector::sharedDirector()->replaceScene(loginScene);
            PromptLayer* prompt = PromptLayer::create();
            prompt->show_prompt(loginScene, msg);
        }
    }
}
// 专用
void DataManager::openUpdata(){
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CONFIG->channelId != 0) {
        CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&DataManager::updateRelogin), this, .1f, false);
    }
#endif
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(SEL_SCHEDULE(&DataManager::updateRelogin), this, 2.f, false);
}
void DataManager::updateRelogin(float delta){
    
    if (JNIController::getRestartApplication() == 1) {
        JNIController::setRestartApplication(0);
        
        DATA->relogin();
    }
}


void DataManager::handle_protocol(int cid, Value content) {
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    const char* notif_format = "HTTP_FINISHED_%d";
    CCObject* pData = NULL;
    switch (cid) {
        case 999: {
            pData = AppUtil::dictionary_with_json(content);
        } break;
            
        case 900: {
            _login->init_with_json(content);
        } break;
        
        case 901: {
            _login->init_with_json(content);
        } break;
        
        case 902: {
            _player->init_with_json(content["player"]);
            _player->init_custom(content["custom"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["show"]);
            _clothes->init_with_json(content["clothes"]);
            _news->init_with_json(content["news"]);
            _purchase->init_purchase(content["purchase"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            
            _welfare->update_statis(content["welfare"]["statis"]);
            _welfare->update_items(content["welfare"]["items"]);
            _achievement->just_update_new_count(content["achieved"]);
            
            _notif = content["notif"].asString();
            
            NET->start_check_news();
            
            this->setFirstOnMainScene(true);
        } break;
        
        case 903: {
            _login->init_with_json(content);
        } break;
        
        case 904: {
            _show->init_with_json(content["show"]);
        } break;
        
        case 905: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
        }
        
        case 907: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
        }
        
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
        
        case 813: {
            _social->init_with_json(content["social"]);
            _social->init_friends(content["friends"]);
        } break;
            
        case 820: {
            _competition->createThemeInfo(content["theme"]);
            _competition->createSelfInfo(content["competition"]);
            _competition->createRanklist(content["ranklist"]);
        } break;
            
        case 821: {
            _player->init_with_json(content["player"]);
            _competition->createSelfInfo(content["competition"]);
            _competition->createOpponentInfo(content["opponent"]);
        } break;
            
        case 823: {
            _player->init_with_json(content["player"]);
            _competition->createSelfInfo(content["competition"]);
        } break;
            
        case 825: {
            _player->init_with_json(content["player"]);
            _competition->createSelfInfo(content["competition"]);
        } break;
            
        case 827: {
            _competition->createSelfInfo(content["competition"]);
            _competition->createOpponentInfo(content["opponent"]);
        } break;
        
        case 700: {
            _mail->init_with_json(content["mail"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
        
        case 701: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
            //
            CCNotificationCenter::sharedNotificationCenter()->postNotification("UPDATE_MAIL_PURCHASE"),
            //
            pData = AppUtil::dictionary_with_json(content["info"]);
        } break;
            //            _mail->handle_mail_oper(content["info"]["id"].asInt(), content["info"]["oper"].asInt());
        
        case 704: {
            _home->init_house_template(content["template"]);
            _home->replace_home_info(content["home"]);
        } break;
        
        case 705: {
            _player->init_with_json(content["player"]);
            _home->replace_home_info(content["home"]);
        } break;
        
        case 707: {
            _home->replace_home_info(content["home"]);
            pData = AppUtil::dictionary_with_json(content["first"]);
        } break;
        
        case 600: {
            _mission->init_with_json(content["mission"]);
            
            _coffers->init_company_template(content["template"]);
            _coffers->replace_user_data(content["coffers"]);
        } break;
        
        case 601: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
        } break;
        
        case 603: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _mission->init_with_json(content["mission"]);
            _clothes->init_with_json(content["clothes"]);
            // 形如：{"rating":5,"levelup":false,"coin":50,"energy":6}.
            pData = AppUtil::dictionary_with_json(content["result"]);
            bool mystery = ((CCBool*)((CCDictionary*)pData)->objectForKey("mystery"))->getValue();
            this->setNeedShowUnlockMystery(mystery);
        } break;
        
        case 605: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _mission->init_with_json(content["mission"]);
            // 形如：{"rating":5,"levelup":false,"coin":50,"energy":6}.
            pData = AppUtil::dictionary_with_json(content["result"]);
        } break;
        
        case 610: {
            _mystery->init_precondition(content["precon"]);
            _mystery->init_template(content["template"]);
            _mystery->update_user_data(content["mystery"]);
        } break;
        
        case 611: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            pData = CCString::create(content["id"].asString());
        } break;
        
        case 613: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _mystery->update_user_data(content["mystery"]);
            // 形如：{"rating":5,"coin":50,"energy":6}.
            pData = AppUtil::dictionary_with_json(content["result"]);
        } break;
        
        case 615: {
            _player->init_with_json(content["player"]); // 不必然
            _clothes->init_with_json(content["clothes"]); // 不必然
            _mystery->update_user_data(content["mystery"]);
            
            if (content["id"] != CSJson::nullValue) {
                pData = CCString::create(content["id"].asString());
            }
            else { // 兼容旧版本
                pData = CCString::create(content["category"].asString());
            }
            
        } break;
        
        
        case 620: {
            _tryst->update_user_data(content["tryst"]);
        } break;
        
        case 621: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _tryst->update_user_data(content["tryst"]);
        } break;
        
        case 623: {
            _tryst->update_user_data(content["tryst"]);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            TrystUserdata* trystUserData = _tryst->getUserData();
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(ccs("tryst"), "name");
            dic->setObject(CCInteger::create(trystUserData->leftTime), "num");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("TRYST_ALERT", dic);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            TrystUserdata* trystUserData = _tryst->getUserData();
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(ccs("tryst"), "name");
            dic->setObject(CCInteger::create(trystUserData->leftTime), "num");
            CCNotificationCenter::sharedNotificationCenter()->postNotification("Push_Android2", dic);
#endif
        } break;
        
        case 625: {
            _tryst->update_user_data(content["tryst"]);
            _player->init_with_json(content["player"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            // 奖励信息
            pData = AppUtil::dictionary_with_json(content["reward"]);
        } break;
            
            
        case 630: {
            if (content["welfare"].type() == CSJson::nullValue) {
                CCLOGWARN("DataManager handle 630, welfare is json null.");
            }
            else {
                _welfare->update_statis(content["welfare"]["statis"]);
                _welfare->update_items(content["welfare"]["items"]);
            }
            
        } break;
            
        case 631: {
            if (content["welfare"].type() == CSJson::nullValue) {
                CCLOGWARN("DataManager handle 631, welfare is json null.");
            }
            else {
                _welfare->update_statis(content["welfare"]["statis"]);
                _welfare->update_items(content["welfare"]["items"]);
            }
            
            _player->init_with_json(content["player"]);
            
            pData = ccs( content["id"].asString());
        } break;
            
        case 633: {
            if (content["welfare"].type() == CSJson::nullValue) {
                CCLOGWARN("DataManager handle 633, welfare is json null.");
            }
            else {
                _welfare->update_statis(content["welfare"]["statis"]);
                _welfare->update_items(content["welfare"]["items"]);
            }
            
            _player->init_with_json(content["player"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            
            pData = ccs( content["id"].asString());
        } break;
            
        case 640: {
            _achievement->init_template(content["template"]);
            _achievement->update_user_accumulated(content["accumulated"]);
            _achievement->update_user_achieved(content["achieved"]);
            _achievement->update_user_finished(content["finished"]);
            _achievement->update_sorted_item_keys();        // important
        } break;
            
        case 641: {
            _player->init_with_json(content["player"]);
            
            _achievement->update_user_achieved(content["achieved"]);
            _achievement->update_user_finished(content["finished"]);
            
            pData = AppUtil::dictionary_with_json(content["result"]);
        }
        
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
            pData = AppUtil::dictionary_with_json(content["extra"]);
        } break;
        
        case 504: {
            _story->replace_story2_user(content["story2"]);
        } break;
        
        case 505: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _story->replace_story2_user(content["story2"]);
        } break;
        
        case 515: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _story->replace_story2_user(content["story2"]);
        } break;
        
        case 507: {
            _story->replace_story2_user(content["story2"]);
        } break;
        
        case 509: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
//            _story->replace_story2_user(content["story2"]);
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
            
        case 403: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
            _clothes->init_dressed(content["show"]);
            _clothes->update_clothes(content["newclothes"]);
            
            _operation->replace_gashapon_user(content["gashapon"]);
        } break;
            
        case 405: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _show->init_with_json(content["show"]);
//            _clothes->init_dressed(content["show"]);
            _clothes->update_clothes(content["newclothes"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            _competition->createSelfInfo(content["competition"]);
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
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
        
        case 304: {
            _operation->init_purchase_achievement_template(content["template"]);
            _operation->replace_purchase_achievement(content["pAchievement"]);
        } break;
        
        case 305: {
            _operation->replace_purchase_achievement(content["pAchievement"]);
            _clothes->init_with_json(content["clothes"]);
            _show->init_with_json(content["show"]);
        } break;
        
        case 306: {
            _operation->init_gashapon_template(content["template"]);
            _operation->init_costs(content["cost"]);
            _operation->replace_gashapon_user(content["gashapon"]);
        } break;
        
        case 307: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _clothes->init_with_json(content["clothes"]);
            _show->init_with_json(content["show"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            pData = AppUtil::dictionary_with_json(content["result"]);
            
            bool isFree = content["free"].asBool();
            if (isFree) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                CCDictionary* dic = CCDictionary::create();
                dic->setObject(ccs("gashapon"), "name");
                dic->setObject(CCInteger::create(content["gashapon"]["free_point"].asInt()), "num");
                CCNotificationCenter::sharedNotificationCenter()->postNotification("FREE_GASHAPON", dic);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
                CCDictionary* dic = CCDictionary::create();
                dic->setObject(ccs("gashapon"), "name");
                dic->setObject(CCInteger::create(content["gashapon"]["free_point"].asInt()), "num");
                CCNotificationCenter::sharedNotificationCenter()->postNotification("Push_Android", dic);
#endif
            }
        } break;
        
        case 309: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _clothes->init_with_json(content["clothes"]);
            _show->init_with_json(content["show"]);
            _operation->replace_gashapon_user(content["gashapon"]);
            _operation->init_extra(content["extra"]);
            pData = AppUtil::dictionary_with_json(content["result"]);
        } break;
        
        case 311: {
            _clothes->init_with_json(content["clothes"]);
            _operation->replace_gashapon_user(content["gashapon"]);
        } break;
        
        case 321: {
            _player->init_with_json(content["player"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
        
        case 323: {
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
            _player->init_with_json(content["player"]);     // maybe
            _operation->replace_gashapon_user(content["gashapon"]); // maybe
            // 奖励信息
            pData = AppUtil::dictionary_with_json(content["reward"]);
        } break;
            
        case 325: {
            _player->init_with_json(content["player"]);
            // 奖励信息
            pData = AppUtil::dictionary_with_json(content["reward"]);
        } break;
        
        case 327: {
            _player->init_with_json(content["player"]);
            // 奖励信息
            pData = AppUtil::dictionary_with_json(content["reward"]);
        } break;
        
        case 333: {
            _player->init_with_json(content["player"]);
            pData = AppUtil::dictionary_with_json(content["result"]);
        } break;
        
        case 340: {
            this->setTempSigninTemplate(AppUtil::array_with_json(content["template"]));
            this->setTempSigninUserdata(AppUtil::dictionary_with_json(content["info"]));
        } break;
        
        case 341: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            this->setTempSigninUserdata(AppUtil::dictionary_with_json(content["info"]));
            _clothes->init_with_json(content["clothes"]);
            _news->init_with_json(content["news"]);
            nc->postNotification("UPDATE_NEWS_STATUS");
        } break;
        
        case 200: {
            _coffers->init_company_template(content["template"]);
            _coffers->replace_user_data(content["coffers"]);
        } break;
        
        case 201: {
            _coffers->replace_user_data(content["coffers"]);
        } break;
        
        case 203: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _coffers->replace_user_data(content["coffers"]);
        } break;
        
        case 205: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _coffers->replace_user_data(content["coffers"]);
            pData = AppUtil::dictionary_with_json(content["result"]);
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
        
        case 107: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
        } break;
        
        case 109: {
            _story->replace_story2_user(content["story2"]);
            _purchase->init_purchase(content["purchase"]);
        } break;
        
        case 111: {
            _story->replace_story2_user(content["story2"]);
            _purchase->init_purchase(content["purchase"]);
        } break;
        
        case 113: {
            _story->replace_story2_user(content["story2"]);
        } break;
            
        case 133: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _purchase->init_purchase(content["purchase"]);
            // 返回订单号
            pData = ccs(content["order_id"].asString());
        } break;
            
            
        case 160: {
            _flashSaleInfo->config(content);
            pData = _flashSaleInfo;
        } break;
            
        case 161: {
            _player->init_with_json(content["player"]);
            this->creat_Energy_Time();
            _clothes->init_with_json(content["clothes"]);
            _show->init_with_json(content["show"]);
        } break;
        
        default:
            break;
    }
    
    nc->postNotification(CCString::createWithFormat(notif_format, cid)->getCString(), pData);
}


void DataManager::relogin() {
    WS->disconnect();
    this->setHasLogin(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CCDirector::sharedDirector()->replaceScene(LoginScene::scene());
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    CCDirector::sharedDirector()->replaceScene(YiJieLoginScene::scene());
#endif
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

// 兼容旧版
bool DataManager::isMysteryEventUnlocked() {
    if (_player && _player->mystery == 1) {
        return true;
    }
    
    return false;
}

SigninState DataManager::fetch_tempsignin_state(CCDictionary* info, const string& id) {
    CCInteger* state = (CCInteger*)info->objectForKey(id);
    if (state == NULL) {
        return e_SigninState_Locked;
    }
    else {
        return (SigninState)state->getValue();
    }
}

int DataManager::current_guide_step(){
    CCDictionary* mainConf = this->getLogin()->config();
    CCInteger* guideConf = (CCInteger*)mainConf->objectForKey("guide");
    if (guideConf->getValue() == 1) {
//        return _player->getGuide();
        return 0;
    }
    else {
        return 0;
    }
}



// talkingData自定义事件
void DataManager::onEvent(std::string eventStr, std::string pageStr, std::string contentStr){
    EventParamMap paramMap;
    paramMap.insert(EventParamPair(pageStr, contentStr));
    TDCCTalkingDataGA::onEvent(eventStr.c_str(), &paramMap);
}

/*
 *  orderId 订单ID, iapId 充值内容, currencyAmount 金额
 *  virtualCurrencyAmount 虚拟币金额
 */
void DataManager::onChargeRequest(std::string orderId, std::string iapId, double currencyAmount, double virtualCurrencyAmount){
    CCString* typeStr = CCString::createWithFormat("%d", CONFIG->channelId);
    TDCCVirtualCurrency::onChargeRequest(orderId.c_str(), iapId.c_str(), currencyAmount, "CNY", virtualCurrencyAmount, typeStr->getCString());
}

void DataManager::onChargeSuccess(std::string orderId){
    TDCCVirtualCurrency::onChargeSuccess(orderId.c_str());
}

// 记录付费点
void DataManager::onPurchase(std::string clothesIdStr, int index, double money){
    TDCCItem::onPurchase(clothesIdStr.c_str(), index, money);
}

// 记录赠予
void DataManager::onReward(double money, std::string contentStr){
//    TDCCVirtualCurrency::onReward(100, "初始化赠予100钻石");
    TDCCVirtualCurrency::onReward(money, contentStr.c_str());
}

//接受或进入
void DataManager::onBegin(std::string taskId){
    TDCCMission::onBegin(taskId.c_str());
}
//任务完成
void DataManager::onCompleted(std::string taskId){
    TDCCMission::onCompleted(taskId.c_str());
}




