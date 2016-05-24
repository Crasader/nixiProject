//
//  IAPComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef IAPComp_hpp
#define IAPComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class IAPComp : public CCObject
{
public:
    ~IAPComp();
    CREATE_FUNC(IAPComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    
protected:
    
private:
    
};

#endif /* IAPComp_hpp */
