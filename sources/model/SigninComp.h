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
    e_SigninState_Done = 2,         // 已签
    e_SigninState_Expired = 3       // 过期(非常规签到用)
} SigninState;

class SigninComp : public CCObject
{
public: // Export
    bool has_init_signin7_template();
    CCArray* signin7_template() const;                          // 7日签模版
    CCDictionary* signin7_info() const;
    SigninState fetch_signin7_state(const string& id);    // 签到状态
    
public:
    ~SigninComp();
    CREATE_FUNC(SigninComp);
    bool init();
    void init_signin7_template(Value json);
    void update_signin7_info(Value json);
    
protected:
    void test_signin7Template();
    
private:
    CCArray*                _signin7Template;
    CCDictionary*           _signin7Info;
};

#endif /* SigninComp_hpp */
