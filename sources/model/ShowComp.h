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
    void print_dress2();
    const char* nickname();
    int collected();            // 收集
    CCDictionary* ondress();    // 着装
    CCDictionary* MyClothesTemp(); // 临时数组
    
protected:
    
private:
    string          _nickname;
    int             _collected;
    CCDictionary*   _ondress;
    CCDictionary*   _myClothesTemp;
};

#endif /* ShowComp_hpp */
