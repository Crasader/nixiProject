//
//  ShowComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef ShowComp_hpp
#define ShowComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class ShowComp : public CCObject
{
public:
    ~ShowComp();
    CREATE_FUNC(ShowComp);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_dress();
    const char* nickname();
    int collected();            // 收集
    int ranking();              // 名次
    string getShowID();
    CCDictionary* ondress();    // 着装
    int resetCost();            // 改名花费
    
    int             isadd;
    int             issend;
    
public:
    string          _nickname;
    string          _id;
    int             _collected;
    int             _ranking;
    CCDictionary*   _ondress;
    CCDictionary*   _ornaments;
    int             _reset;
};

#endif /* ShowComp_hpp */
