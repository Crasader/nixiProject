//
//  ClothesComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef ClothesComp_hpp
#define ClothesComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class ClothesComp : public CCObject
{
public:
    ~ClothesComp();
    CREATE_FUNC(ClothesComp);
    bool init();
    void init_with_json(Value json);
    void init_clothestemp(Value json);
    void print_dress();
    
public:
    CCDictionary* clothes();
    CCDictionary* MyClothesTemp(); // 临时数组
    
private:
    CCDictionary*       _clothes;
    CCDictionary*       _myClothesTemp;
};

#endif /* ClothesComp_hpp */
