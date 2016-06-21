//
//  IAPComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef IAPComp_hpp
#define IAPComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

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
    
protected:
private:
};

class IAPComp : public CCObject
{
public:
    ~IAPComp();
    CREATE_FUNC(IAPComp);
    bool init();
    void init_products(Value json);
    void init_purchased(Value json);
    
public:
    bool has_init_products();
    CCArray* products();
    CCDictionary* purchased();
    bool has_purchased(const char* product_id);
    void print_all_products();

private:
    CCArray*            _products;
    CCDictionary*       _purchased;
};

#endif /* IAPComp_hpp */
