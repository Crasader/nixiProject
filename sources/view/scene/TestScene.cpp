//
//  TestScene.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "TestScene.h"
#include "DataManager.h"
#include "NetManager.h"

#define PADDING 16

bool TestScene::init() {
    if (!CCScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::create("pic/test_bg.png");
    bg->setPosition(ccp(win_size.width * 0.5, win_size.height * 0.5));
    this->addChild(bg);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    return true;
}

void TestScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TestScene::login_game_server), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::social_view), "HTTP_FINISHED_902", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::stranger_view), "HTTP_FINISHED_802", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::message_view), "HTTP_FINISHED_804", NULL);
    
    NetEnv netenv = NetManager::Inst()->obtain_net_env();
    std::string env_info;
    switch (netenv) {
        case e_NetEnv_NotReachable:
            env_info = "没有网络!~";
            break;
        case e_NetEnv_ViaWiFi:
            env_info = "使用 WiFi 接入";
            break;
        case e_NetEnv_ViaWWAN:
            env_info = "使用 WWAN 网络";
            break;
        case e_NetEnv_Unknown:
        default:
            env_info = "未知网络!~";
            break;
    }
    CCLOG("%s", env_info.c_str());
    
    this->login_view();
}

void TestScene::onExit() {
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

void TestScene::login_view() {
    _content->removeAllChildren();
    
    CCMenuItemFont* btn_fast_login = CCMenuItemFont::create("游客登入", this, SEL_MenuHandler(&TestScene::fast_login));
    CCMenu* menu = CCMenu::createWithItem(btn_fast_login);
    menu->setColor(ccRED);
    menu->alignItemsVerticallyWithPadding(PADDING);
    _content->addChild(menu);
}

void TestScene::social_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 登入", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(900);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCMenuItemFont* btn_recommend_stranger = CCMenuItemFont::create("推荐陌生人", this, SEL_MenuHandler(&TestScene::recommend_stranger));
    CCMenuItemFont* btn_search_other = CCMenuItemFont::create("查找 玩家", this, SEL_MenuHandler(&TestScene::search_other));
    CCMenuItemFont* btn_all_messages = CCMenuItemFont::create("查看所有消息", this, SEL_MenuHandler(&TestScene::all_messages));
    
    CCMenu* menu = CCMenu::create(btn_recommend_stranger, btn_search_other, btn_all_messages, NULL);
    menu->setColor(ccORANGE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    _content->addChild(menu);
}

void TestScene::stranger_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 社交", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(800);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCDictionary* strangers = DataManager::Inst()->getSocial()->strangers();
    CCArray* sids = strangers->allKeys();
    CCObject* pObj = NULL;
    CCArray* btns = CCArray::create();
    CCARRAY_FOREACH(sids, pObj) {
        CCString* sid = (CCString* )pObj;
        CCMenuItemFont* btn = CCMenuItemFont::create(sid->getCString(), this, SEL_MenuHandler(&TestScene::msg_with_friend_ask));
        btn->setUserObject(ccs(sid->getCString()));
        btns->addObject(btn);
    }
    
    CCMenu* menu = CCMenu::createWithArray(btns);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    menu->getChildren();
    _content->addChild(menu);
}

void TestScene::message_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 社交", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(800);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCArray* messages = DataManager::Inst()->getMessage()->messages();
    CCObject* pObj = NULL;
    CCArray* btns = CCArray::create();
    CCARRAY_FOREACH(messages, pObj) {
        MessageItem* msg = (MessageItem* )pObj;
        CCString* format = NULL;
        int type = msg->type;
        if (1 == type) {
            format = CCString::createWithFormat("玩家%s向您请求加为好友!", msg->sender.c_str());
        }
        else if (2 == type) {
            format = CCString::createWithFormat("玩家%s向您送出xx点体力!", msg->sender.c_str());
        }
        else if (3 == type) {
            format = CCString::createWithFormat("玩家%s 。。。。。。!", msg->sender.c_str());
        }
        CCMenuItemFont* btn = CCMenuItemFont::create(format->getCString(), this, SEL_MenuHandler(&TestScene::response_message));
        btn->setTag(msg->id);
        btns->addObject(btn);
    }
    
    CCMenu* menu = CCMenu::createWithArray(btns);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    menu->getChildren();
    _content->addChild(menu);
}

void TestScene::on_return(CCMenuItem* btn) {
    int tag = btn->getTag();
    switch (tag) {
        case 900: {
            this->login_view();
        } break;
            
        case 800: {
            this->social_view();
        } break;
            
        default:
            break;
    }
}

void TestScene::fast_login() {
    NetManager::Inst()->fast_login_900(DataManager::Inst()->getLogin()->obtain_UUID());
}

void TestScene::login_game_server() {
    NetManager::Inst()->login_game_server_902();
}

void TestScene::recommend_stranger() {
    NetManager::Inst()->recommend_stranger_802();
}

void TestScene::msg_with_friend_ask(CCMenuItem* btn) {
    CCString* other_sid = (CCString* )btn->getUserObject();
    CCLOG("Send msg to other: %s", other_sid->getCString());
    NetManager::Inst()->send_message_803(other_sid->getCString(), e_Msg_Friend_Ask);
}

void TestScene::all_messages() {
    NetManager::Inst()->all_messages_804();
}

void TestScene::response_message(cocos2d::CCMenuItem *btn) {
    NetManager::Inst()->response_message_805(btn->getTag(), 1);
}

void TestScene::search_other() {
    NetManager::Inst()->search_other_806("8A79B644");
}
