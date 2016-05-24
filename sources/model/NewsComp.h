//
//  NewsComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef NewsComp_hpp
#define NewsComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class NewsComp : public CCObject
{
public:
    ~NewsComp();
    CREATE_FUNC(NewsComp);
    bool init();
    void init_with_json(Value json);
    
public:
    bool is_other_day();
    int energy();
    int mail();
    int message();
    int paper();
    
protected:
    
private:
    bool _otherday;
    int _energy;
    int _mail;
    int _message;
    int _paper;
};

#endif /* NewsComp_hpp */
