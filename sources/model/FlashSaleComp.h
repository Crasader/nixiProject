//
//  FlashSaleComp.hpp
//  tiegao
//
//  Created by ilakkey on 2017/3/8.
//
//

#ifndef FlashSaleComp_h
#define FlashSaleComp_h

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;

class FlashSaleInfo : public CCObject
{
public:
    ~FlashSaleInfo();
    CREATE_FUNC(FlashSaleInfo);
    virtual bool init() { return true; }
    void config(Value json);
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(int, _phase, Phase);
    CC_SYNTHESIZE(int, _type, Type);
    CC_SYNTHESIZE(unsigned long, _deadline, Deadline);
    CC_SYNTHESIZE(int, _oldPrice, OldPrice);
    CC_SYNTHESIZE(float, _discount, Discount);
    CC_SYNTHESIZE(int, _nowPrice, NowPrice);
    CC_SYNTHESIZE_RETAIN(CCArray*, _clothes, Clothes);
};


class FlashSaleComp : public CCObject
{
public:
    ~FlashSaleComp() {}
    CREATE_FUNC(FlashSaleComp);
    bool init() { return true; }
    
private:

};

#endif /* FlashSaleComp_h */
