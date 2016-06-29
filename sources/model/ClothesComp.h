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
    void init_dressed(Value json);
    void copy_clothesTemp();
    void update_clothes(Value json);
    bool is_owned(int part, int cloth_id);
    bool is_owned(const char* part, int cloth_id);
    void print_dress();
    
public:
    bool                has_init_clothes;

public:
    CCDictionary* clothes();
    CCDictionary* dress();
    CCDictionary* MyClothesTemp(); // 临时数组
    
private:
    CCDictionary*       _clothes;
    CCDictionary*       _dress;
    CCDictionary*       _ornaments;
    CCDictionary*       _myClothesTemp;
};

#endif /* ClothesComp_hpp */
