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
    
    CCDictionary* fetchDialog(int idx);
    
private:
    virtual bool init(CCArray* dialogs);
    
private:
    CCArray*            _dialogs;
};

#endif /* TrystData_hpp */
