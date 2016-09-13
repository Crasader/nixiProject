//
//  LoginComp.hpp
//  tiegao
//
//  Created by mac on 16/5/16.
//
//

#ifndef LoginComp_hpp
#define LoginComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class LoginComp : public CCObject
{
public:
    ~LoginComp();
    CREATE_FUNC(LoginComp);
    bool init();
    void init_with_json(Value json);
    void config_UUID();
    
public:
    const char* obtain_UUID();
    const char* obtain_sid();
    const char* obtain_skey();
    const char* obtain_game_addr();
    CCDictionary* config();
    
    void setUUid(CCString* uuStr);
    
protected:
    
private:
    string          _uuid;
    string          _sid;
    string          _skey;
    string          _gameaddr;
    CCDictionary*   _config;
};

#endif /* LoginComp_hpp */
