//
//  MailComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef MailComp_hpp
#define MailComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class MailComp : public CCObject
{
public:
    ~MailComp();
    CREATE_FUNC(MailComp);
    bool init();
    
    void init_with_json(Value json);
    
public:
    
protected:
    
private:

};

#endif /* MailComp_hpp */
