//
//  SigninComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef SigninComp_hpp
#define SigninComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class SigninComp : public CCObject
{
public:
    ~SigninComp();
    CREATE_FUNC(SigninComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    
protected:
    
private:
    
};

#endif /* SigninComp_hpp */
