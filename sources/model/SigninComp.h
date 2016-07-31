//
//  SigninComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//

#ifndef SigninComp_hpp
#define SigninComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

typedef enum {
    e_SigninState_Locked = 0,       // 锁定
    e_SigninState_Available = 1,    // 可用
    e_SigninState_Retroactive = 2,  // 可补签
    e_SigninState_Done = 3          // 已签
} SigninState;

class SigninComp : public CCObject
{
public: // Export
    CCArray* signin7_template();                // 7日签模版
    SigninState fetch_signin7_state(int id);    // 签到状态
    
public:
    ~SigninComp();
    CREATE_FUNC(SigninComp);
    bool init();
    void parse_signin7_info(Value json);
    
protected:
    
private:
    CCArray*            _signin7Info;
};

#endif /* SigninComp_hpp */
