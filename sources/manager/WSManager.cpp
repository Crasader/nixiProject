//
//  WSManager.cpp
//  tiegao
//
//  Created by mac on 16/7/28.
//
//

#include "WSManager.h"
#include "ConfigManager.h"
#include "DataManager.h"
#include "ChatComp.h"
#include "BarrageView.h"
#include "json_lib.h"
#include "ChatPanel.h"

using namespace CSJson;

static WSManager* _instance = nullptr;

#pragma mark - Export API

void WSManager::show_barrage() {
    
}

void WSManager::connect() {
    _ws = new WebSocket();
    CCLOG("WS start connect to chat server addr: %s", CONFIG->chator_addr.c_str());
    _ws->init(*_instance, CONFIG->chator_addr);
    _isConnected = true;
}

bool WSManager::isConnected() {
    return _isConnected;
}

void WSManager::send(const string& msg) {
    _ws->send(msg);
}

void WSManager::disconnect() {
    if (_ws) {
        _ws->close();
    }
}

#pragma mark - Inner API

WSManager::~WSManager() {
    this->disconnect();
    CC_SAFE_DELETE(_ws);
}

WSManager* WSManager::Inst() {
    if (_instance == nullptr) {
        _instance = new WSManager();
        _instance->_ws = NULL;
    }
    
    return _instance;
}

#pragma mark - WebSocket Delegate API

void WSManager::onOpen(WebSocket* ws) {
    CCLOG("Websocket (%p) opened", ws);
//    BarrageView::show();
    ChatPanel* panel = ChatPanel::create();
    CCDirector::sharedDirector()->getRunningScene()->addChild(panel);
}

void WSManager::onMessage(WebSocket* ws, const WebSocket::Data& data) {
    if(DATA->getChatOut()){
        return;
    }
    
    if (data.isBinary) {
        std::string binaryStr = "response bin msg: ";
        for (int i = 0; i < data.len; ++i) {
            if (data.bytes[i] != '\0') {
                binaryStr += data.bytes[i];
            }
            else {
                binaryStr += "\'\\0\'";
            }
        }
        CCLOG("WSManager::onMessage(BINARY) - %s", binaryStr.c_str());
    }
    else {
        CCLOG("WSManager::onMessage(TEXT) - %s", data.bytes);
        Reader reader;
        Value root;
        if (reader.parse(data.bytes, root, false)) {
            Value reply = root["reply"];
            if (reply != nullValue) {
                DATA->getChat()->setInterval(reply.asInt());
                CCNotificationCenter::sharedNotificationCenter()->postNotification("CHAT_REPLY");
            }
            else {
                ChatItem* chat = ChatItem::create();
                if (chat->init_with_json(root)) {
                    DATA->getChat()->addItem(chat);
                    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEW_CHAT", chat);
                }
            }
        }
    }
}

void WSManager::onClose(WebSocket* ws) {
    CCLOG("websocket instance (%p) closed.", ws);
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
    ws = NULL;
    _isConnected = false;
}

void WSManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("WSManager::onError(), error code: %d", error);
    
}

