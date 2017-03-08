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
    CC_SYNTHESIZE(int, _id, Id);                            // ID，暂时用不上
    CC_SYNTHESIZE(int, _phase, Phase);                      // 套装公司等级要求
    CC_SYNTHESIZE(int, _type, Type);                        // 购买类型，1-金币；2-钻石
    CC_SYNTHESIZE(unsigned long, _deadline, Deadline);      // 截止剩余时间，单位秒
    CC_SYNTHESIZE(int, _oldPrice, OldPrice);                // 原总价
    CC_SYNTHESIZE(float, _discount, Discount);              // 折扣比，小数
    CC_SYNTHESIZE(int, _nowPrice, NowPrice);                // 折扣后的价格
    CC_SYNTHESIZE_RETAIN(CCArray*, _clothes, Clothes);      // 包含的衣服，Interge数组
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
