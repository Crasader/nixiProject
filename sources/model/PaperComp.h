//
//  PaperComp.hpp
//  tiegao
//
//  Created by mac on 16/6/29.
//
//

#ifndef PaperComp_hpp
#define PaperComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class PaperItem : public CCObject
{
public:
    ~PaperItem();
    CREATE_FUNC(PaperItem);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_self();
    
    CSJson::UInt64  id;
    string          sender;
    string          sender_name;
    string          content;
    
protected:
private:
};

class PaperComp : public CCObject
{
public:
    ~PaperComp();
    CREATE_FUNC(PaperComp);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_papers();
    CCArray* papers() const;
    
    CC_SYNTHESIZE(const char*, _replyID, ReplyID);
    CC_SYNTHESIZE(const char*, _nickname, NickName);
    
    void delete_paper(CSJson::UInt64 id);

protected:
    
private:
    CCArray*            _papers;
};

#endif /* PaperComp_hpp */
