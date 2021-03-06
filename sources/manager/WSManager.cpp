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
    if (_ws && _ws->getReadyState() == cocos2d::extension::WebSocket::kStateOpen) {
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
    CCNotificationCenter::sharedNotificationCenter()->postNotification("OPEN_CHAT");
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
                    ChatComp* compChat = DATA->getChat();
                    compChat->addItem(chat);
                    compChat->setNewChatCount(compChat->getNewChatCount() + 1);
                    if (chat->channel == 1) {
                        compChat->addShoutItem(chat);
                        compChat->setNewShoutCount(compChat->getNewShoutCount() + 1);
                    }
                    
                    //
                    int length = compChat->getItems()->count();
                    if( length >= 200) {
                        CCLOG("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
                        int oldShoutCount = compChat->getShoutItems()->count();
                        CCArray* arr = CCArray::create();
                        CCArray* shouts = CCArray::create();
                        
                        for (int i = length - 100; i < length; i++) {
                            ChatItem* item = (ChatItem* )DATA->getChat()->getItems()->objectAtIndex(i);
                            arr->addObject(item);
                            if (item->channel == 1) {
                                shouts->addObject(item);
                            }
                        }
                        
                        compChat->setItems(arr);
                        compChat->setShoutItems(shouts);
                        int newShoutCount = shouts->count();
                        int ccc = MAX(compChat->getShoutCursor() - (oldShoutCount - newShoutCount), 0);
                        compChat->setShoutCursor(ccc);
                    }
                    
//                    CCLOG("Message_count = %d", DATA->getChat()->getItems()->count());
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

