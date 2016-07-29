//
//  WSManager.cpp
//  tiegao
//
//  Created by mac on 16/7/28.
//
//

#include "WSManager.h"
#include "DataManager.h"
#include "ChatComp.h"
#include "BarrageView.h"
#include "json_lib.h"

using namespace CSJson;

static WSManager* _instance = nullptr;

const int CONNECT_TIMEOUT = 60;

#pragma mark - Export API

void WSManager::show_barrage() {
    
}

void WSManager::connect() {
//        const char* addr = "ws://localhost:8080/websocket";
    const char* addr = "ws://192.168.1.234:8080/websocket";
//        const char* addr = "ws://echo.websocket.org";
    WebSocket* ws = new WebSocket();
    ws->init(*_instance, addr);
}

void WSManager::disconnect() {
    if (_ws) {
        CC_SAFE_DELETE(_ws);
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
        _instance->_ws = nullptr;
    }
    
    return _instance;
}

#pragma mark - WebSocket Delegate API

void WSManager::onOpen(WebSocket* ws) {
    CCLOG("Websocket (%p) opened", ws);
    BarrageView::show();
}

void WSManager::onMessage(WebSocket* ws, const WebSocket::Data& data) {
    if (! data.isBinary) {
        CCLOG("WSManager::onMessage() - %s", data.bytes);
        Reader reader;
        Value root;
        if (! reader.parse(data.bytes, root, false)) {
            CCLOG("WSManager::onMessage() json reader error.");
            return;
        }
        ChatItem* chat = ChatItem::create();
        chat->init_with_json(root);
//        chat->print_self();
        DATA->getChat()->addItem(chat);
        CCNotificationCenter::sharedNotificationCenter()->postNotification("NEW_CHAT", chat);
    }
    else {
        std::string binaryStr = "response bin msg: ";
        
        for (int i = 0; i < data.len; ++i) {
            if (data.bytes[i] != '\0')
            {
                binaryStr += data.bytes[i];
            }
            else
            {
                binaryStr += "\'\\0\'";
            }
        }
        
        CCLOG("WSManager::onMessage() - %s", binaryStr.c_str());
    }
}

void WSManager::onClose(WebSocket* ws) {
    CCLOG("websocket instance (%p) closed.", ws);
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void WSManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLOG("WSManager::onError(), error code: %d", error);
}

