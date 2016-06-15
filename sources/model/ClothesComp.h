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
    
public:
    CCDictionary* clothes();
    
private:
    CCDictionary*       _clothes;
};

#endif /* ClothesComp_hpp */
