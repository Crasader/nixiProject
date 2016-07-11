//
//  CoffersComp.hpp
//  tiegao
//
//  Created by mac on 16/7/11.
//
//

#ifndef CoffersComp_hpp
#define CoffersComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class CoffersComp : public CCObject
{
public:
    ~CoffersComp();
    CREATE_FUNC(CoffersComp);
    bool init();
    void init_with_json(Value json);
    
public:
    bool could_prduce();
    void produce();
    void reset_collected();
    
public:
    int         profit;
    int         top;
    int         collected;
    
private:
};

#endif /* CoffersComp_hpp */
