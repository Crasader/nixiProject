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
    int             id;
    int             type;
    string          sender;
    
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
    
protected:
    
private:
    CCArray*            _papers;
};

#endif /* PaperComp_hpp */
