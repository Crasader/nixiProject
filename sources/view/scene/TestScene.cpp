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
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "IOSIAPManager.h"

#include "MainScene.h"

#include "MailPanel.h"
#include "OperationPanel.h"

#define PADDING 16

bool TestScene::init() {
    if (!CCScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
        
    CCSprite* bg = CCSprite::create("res/pic/loginScene/login_bg.png");
    bg->setPosition(DISPLAY->center());
    this->addChild(bg);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    return true;
}

void TestScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TestScene::login_game_server), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::master_view), "HTTP_FINISHED_902", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::stranger_view), "HTTP_FINISHED_802", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::message_view), "HTTP_FINISHED_804", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TestScene::mail_view), "HTTP_FINISHED_700", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TestScene::mission_view), "HTTP_FINISHED_600", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TestScene::dress_view), "HTTP_FINISHED_400", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TestScene::IAP_view), "HTTP_FINISHED_100", NULL);
    
    NetEnv netenv = NET->obtain_net_env();
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
    
//    IOSIAPManager::Inst();
}

void TestScene::onExit() {
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

#pragma mark -

void TestScene::login_view() {
    _content->removeAllChildren();
    
    CCMenuItemFont* btn_fast_login = CCMenuItemFont::create("游客登入", this, SEL_MenuHandler(&TestScene::fast_login));
    CCMenu* menu = CCMenu::createWithItem(btn_fast_login);
    menu->setColor(ccRED);
    menu->alignItemsVerticallyWithPadding(PADDING);
    _content->addChild(menu);
}


void TestScene::master_view() {
    CCScene* scene = MainScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
    
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 登入", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(900);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCMenuItemFont* btn_IAP = CCMenuItemFont::create("IAP", this, SEL_MenuHandler(&TestScene::all_products));
    CCMenuItemFont* btn_mail = CCMenuItemFont::create("查看 所有邮件", this, SEL_MenuHandler(&TestScene::all_mails));
    CCMenuItemFont* btn_social = CCMenuItemFont::create("进入 社交", this, SEL_MenuHandler(&TestScene::social_view));
    CCMenuItemFont* btn_mission = CCMenuItemFont::create("换装任务", this, SEL_MenuHandler(&TestScene::mission));
    CCMenuItemFont* btn_clothes = CCMenuItemFont::create("拥有衣物", this, SEL_MenuHandler(&TestScene::owned_clothes));
    
    CCMenu* menu = CCMenu::create(btn_IAP
                                  , btn_mail
                                  , btn_social
                                  , btn_mission
                                  , btn_clothes
                                  , NULL);
    menu->setColor(ccORANGE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    _content->addChild(menu);
}

void TestScene::IAP_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCMenuItemFont* btn_operation = CCMenuItemFont::create("运营活动", this, SEL_MenuHandler(&TestScene::operation));
    CCMenuItemFont* btn_transaction = CCMenuItemFont::create("支付界面", this, SEL_MenuHandler(&TestScene::transaction));
    CCMenu* menu = CCMenu::create( btn_operation
                                  , btn_transaction, NULL);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    menu->getChildren();
    _content->addChild(menu);
}

void TestScene::operation() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 IAP", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(100);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    OperationPanel* panel = OperationPanel::create();
    panel->show_from(CCPointMake(80, 900));
}

void TestScene::transaction() {
    
}

void TestScene::purchase_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 IAP", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(100);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCArray* products = DataManager::Inst()->getIAP()->products();
    CCObject* pObj = NULL;
    CCArray* btns = CCArray::create();
    CCARRAY_FOREACH(products, pObj) {
        ProductItem* prod = (ProductItem* )pObj;
        CCString* format = CCString::createWithFormat("Id:%s money:%d  => diam:%d",
                    prod->id.c_str(), prod->money, prod->diam);
        CCMenuItemFont* btn = CCMenuItemFont::create(format->getCString(), this, SEL_MenuHandler(&TestScene::buy_product));
        btn->setUserObject(ccs(prod->id));
        btns->addObject(btn);
    }
    
    CCMenu* menu = CCMenu::createWithArray(btns);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    menu->getChildren();
    _content->addChild(menu);
}

void TestScene::mail_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    MailPanel* panel = MailPanel::create();
    panel->show_from(CCPointMake(560, 900));
}

void TestScene::social_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }

    CCMenuItemFont* btn_recommend_stranger = CCMenuItemFont::create("推荐陌生人", this, SEL_MenuHandler(&TestScene::recommend_stranger));
    CCMenuItemFont* btn_search_other = CCMenuItemFont::create("查找 玩家", this, SEL_MenuHandler(&TestScene::search_other));
    CCMenuItemFont* btn_all_messages = CCMenuItemFont::create("查看 所有消息", this, SEL_MenuHandler(&TestScene::all_messages));
    
    CCMenu* menu = CCMenu::create( btn_recommend_stranger
                                  , btn_search_other
                                  , btn_all_messages, NULL);
    menu->setColor(ccORANGE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    _content->addChild(menu);
}

void TestScene::stranger_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCDictionary* strangers = DATA->getSocial()->strangers();
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
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCArray* messages = DATA->getMessage()->messages();
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

void TestScene::dress_view() {
    
}

void TestScene::mission_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 选项", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(902);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCMenuItemFont* btn_start = CCMenuItemFont::create("开始任务", this, SEL_MenuHandler(&TestScene::start_mission));
    CCMenuItemFont* btn_commit = CCMenuItemFont::create("提交任务", this, SEL_MenuHandler(&TestScene::commit_mission));
    CCMenu* menu = CCMenu::create( btn_start
                                  , btn_commit, NULL);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(PADDING);
    menu->getChildren();
    _content->addChild(menu);
}

#pragma mark -

void TestScene::on_return(CCMenuItem* btn) {
    int tag = btn->getTag();
    switch (tag) {
        case 900: {
            this->login_view();
        } break;
            
        case 902: {
            this->master_view();
        } break;
            
        case 100: {
            this->IAP_view();
        } break;
            
        default:
            break;
    }
}

void TestScene::fast_login() {
    NET->fast_login_900(DATA->getLogin()->obtain_UUID());
}

void TestScene::login_game_server() {
    NET->login_game_server_902();
}

void TestScene::all_mails() {
    NET->all_mails_700();
}

void TestScene::recommend_stranger() {
    NET->recommend_stranger_802();
}

void TestScene::msg_with_friend_ask(CCMenuItem* btn) {
    CCString* other_sid = (CCString* )btn->getUserObject();
    CCLOG("Send msg to other: %s", other_sid->getCString());
    NET->send_message_803(other_sid->getCString(), e_Msg_Friend_Ask);
}

void TestScene::all_messages() {
    NET->all_messages_804();
}

void TestScene::response_message(cocos2d::CCMenuItem *btn) {
    NET->response_message_805(btn->getTag(), 1);
}

void TestScene::search_other() {
    NET->search_other_806("8A79B644");
}

void TestScene::all_products() {
    NetManager::Inst()->all_products_100();
}

void TestScene::buy_product(cocos2d::CCMenuItem *btn) {
    CCString* prod_id = (CCString* )btn->getUserObject();
    CCLOG("buy_product() - %s", prod_id->getCString());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IOSIAPManager* d = IOSIAPManager::Inst();
    if (d->canMakePurchases()) {
        CCLOG("can purchases");
        d->buyProduct(prod_id->getCString());
    }
    else {
        CCLOG("can not purchases");
    }
#endif
}

void TestScene::mission() {
//    CCArray* mission = CONFIG->mission();
//    CCDictionary* m1 = (CCDictionary* )mission->objectAtIndex(0);
//    CCString* name = (CCString* )m1->objectForKey("name");
//    CCLOG("m1:name = %s", name->getCString());
    NET->completed_mission_600();
}

void TestScene::owned_clothes() {
    NET->owned_clothes_400();
}

void TestScene::start_mission() {
    int id = DATA->getPlayer()->next_mission;
    NET->start_mission_601(id);
}

void TestScene::commit_mission() {
    NET->commit_mission_602();
}




/*
 * series 套系
 * access_type 付费类型  1-金币 2-钻石 11-赠送
 * for_sale 是否出售 1-显示出售 2-不显示不出售
 *
 *
 *
 *
 *
 *
 */




