//
//  TrystData.hpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#ifndef TrystData_hpp
#define TrystData_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystData : public CCObject
{
public:
    ~TrystData();
    static TrystData* create(CCArray* dialogs);
    
//    const char* nextDialog();
    
private:
    virtual bool init(CCArray* dialogs);

    
private:
    
private:
    CCArray*            _dialogs;
};

#endif /* TrystData_hpp */
