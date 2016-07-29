//
//  WSManager.hpp
//  tiegao
//
//  Created by mac on 16/7/28.
//
//

#ifndef WSManager_hpp
#define WSManager_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "WebSocket.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define WS         WSManager::Inst()

class WSManager : public CCObject, public WebSocket::Delegate
{
public:
    void show_barrage();
    void connect();
    void disconnect();
    
public:
    ~WSManager();
    static WSManager* Inst();
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    
private:
    WebSocket*          _ws;
};

#endif /* WSManager_hpp */
