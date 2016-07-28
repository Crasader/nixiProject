//
//  WSManager.cpp
//  tiegao
//
//  Created by mac on 16/7/28.
//
//

#include "WSManager.h"
//#include "ConfigManager.h"
//#include "DataManager.h"
//#include "IMD5.h"
//#include "json_lib.h"
//using namespace CSJson;

static WSManager* _instance = nullptr;


const int CONNECT_TIMEOUT = 60;

#pragma mark - Export API

WSManager* WSManager::Inst() {
    if (_instance == nullptr) {
        _instance = new WSManager();
//        const char* addr = "ws://localhost:8080/websocket";
        const char* addr = "ws://192.168.1.234:8080/websocket";
//        const char* addr = "ws://echo.websocket.org";
        WebSocket* ws = new WebSocket();
        ws->init(*_instance, addr);
    }
    
    return _instance;
}

#pragma mark - Inner API

WSManager::~WSManager() {
    
}

#pragma mark - WebSocket Delegate API

void WSManager::onOpen(WebSocket* ws) {
    CCLog("Websocket (%p) opened", ws);
    ws->send("A la xi ba.");
}

void WSManager::onMessage(WebSocket* ws, const WebSocket::Data& data) {
    if (!data.isBinary) {
        CCLOG("WSManager::onMessage() - 1111111111");
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
        
        CCLog("WSManager::onMessage() - %s", binaryStr.c_str());
//        _sendBinaryStatus->setString(binaryStr.c_str());
    }
}

void WSManager::onClose(WebSocket* ws) {
    CCLog("websocket instance (%p) closed.", ws);
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
}

void WSManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error) {
    CCLog("WSManager::onError(), error code: %d", error);
}

