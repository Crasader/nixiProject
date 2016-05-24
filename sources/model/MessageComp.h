//
//  MessageComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef MessageComp_hpp
#define MessageComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class MessageComp : public CCObject
{
public:
    ~MessageComp();
    CREATE_FUNC(MessageComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    
protected:
    
private:
    
};

#endif /* MessageComp_hpp */
