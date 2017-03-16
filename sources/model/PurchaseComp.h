//
//  PurchaseComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef PurchaseComp_hpp
#define PurchaseComp_hpp

#include "cocos2d.h"
#include "json_lib.h"
#include "extend_define.h"

USING_NS_CC;
using namespace CSJson;

class ProductItem : public CCObject
{
public:
    ~ProductItem();
    CREATE_FUNC(ProductItem);
    bool init();
    void init_with_json(Value json);
    
public:
    void print_self();
    string              id;
    int                 diam;
    int                 money;
    
private:
};


class MonthlyCardItem : public CCObject
{
public:
    ~MonthlyCardItem();
    CREATE_FUNC(MonthlyCardItem);
    bool init();
    void init_with_json(const char* id, Value json);
    void print_self();
    
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(string, _cardID, CardID); // 目前好像用不上
    CC_SYNTHESIZE_READONLY(int, _status, Status);          // 0-未获得；1-可领取；2-当日已领取
    CC_SYNTHESIZE_READONLY(int, _daysRest, DaysRest);      // 剩余天数，未获得为0
};


class PurchaseComp : public CCObject
{
public: // Export
    bool has_init_products();
    CCArray* products();
    CCDictionary* deals();
    bool has_purchased(const char* product_id);
    void print_all_products();
    
public:
    ~PurchaseComp();
    CREATE_FUNC(PurchaseComp);
    bool init();
    void init_products(Value json);
    void init_purchase(Value json);

public:
    SYNTHESIZE_RETAIN_READONLY(CCArray*, _products, Products);
    SYNTHESIZE_RETAIN_READONLY(CCDictionary*, _deals, Deals);
    
    SYNTHESIZE_READONLY(int, _energyBoughtTimes, EnergyBoughtTimes);
    SYNTHESIZE_READONLY(int, _energyBuyLimit, EnergyBuyLimit);
    SYNTHESIZE_READONLY(int, _coinExchangedTimes, CoinExchangedTimes);
    SYNTHESIZE_READONLY(int, _coinExchangeLimit, CoinExchangeLimit);
    
    SYNTHESIZE_READONLY(int, _energyBoughtCost, EnergyBoughtCost);
    SYNTHESIZE_READONLY(int, _energyBoughtGain, EnergyBoughtGain);
    SYNTHESIZE_READONLY(int, _coinExchangeCost, CoinExchangeCost);
    SYNTHESIZE_READONLY(int, _coinExchangeGain, CoinExchangeGain);
    
    // 月卡
    CC_SYNTHESIZE_RETAIN(MonthlyCardItem*, _monthlyCard1, MonthlyCard1);
    CC_SYNTHESIZE_RETAIN(MonthlyCardItem*, _monthlyCard2, MonthlyCard2);

private:

};

#endif /* PurchaseComp_hpp */
