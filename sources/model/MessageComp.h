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

class MessageItem : public CCObject
{
public:
    ~MessageItem();
    CREATE_FUNC(MessageItem);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_self();
    int             id;
    int             type;
    string          sender;
    
protected:
private:
};

class MessageComp : public CCObject
{
public:
    ~MessageComp();
    CREATE_FUNC(MessageComp);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_messages();
    CCArray* messages() const;
    
protected:
    
private:
    CCArray*            _messages;
};

#endif /* MessageComp_hpp */
