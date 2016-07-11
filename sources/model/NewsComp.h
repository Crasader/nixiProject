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
    void print_news();
    bool otherday;
    int coin;
    int energy;
    int mail;
    int message;
    int paper;
    
protected:
    
private:
};

#endif /* NewsComp_hpp */
