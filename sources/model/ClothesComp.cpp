//
//  ClothesComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "ClothesComp.h"

ClothesComp::~ClothesComp() {
}

bool ClothesComp::init() {

    return true;
}

void ClothesComp::init_with_json(Value json) {
    if (!json.isObject()) {
        CCLOG("ClothesComp::init_with_json() json object error.");
        return;
    }
    
}
