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

class Reward;

class MailItem : public CCObject
{
public:
    ~MailItem();
    CREATE_FUNC(MailItem);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_self();
    int             id;
    int             send_time;
    string          sender;
    string          title;
    string          content;
    Reward*        reward;
    
protected:
private:
};

class MailComp : public CCObject
{
public:
    ~MailComp();
    CREATE_FUNC(MailComp);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_mails();
    CCArray* mails() const;
    
protected:
    
private:
    CCArray*            _mails;
};

#endif /* MailComp_hpp */
