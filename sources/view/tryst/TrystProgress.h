//
//  TrystProgress.hpp
//  tiegao
//
//  Created by mac on 16/11/2.
//
//

#ifndef TrystProgress_hpp
#define TrystProgress_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystProgress : public CCNode
{
public:
    ~TrystProgress();
    static TrystProgress* create(CCObject *target, SEL_CallFunc callback);
    
    void startInput(const char* text);
    
private:
    virtual bool init(CCObject *target, SEL_CallFunc callback);
    void gogogo();
    
private:
    CCObject*           _target;
    SEL_CallFunc        _callback;
    CCLabelTTF*         _lbl;
    std::string         _origText;
    int                 _inputCount;
    float               _savedInputboxWidth;
};

#endif /* TrystProgress_hpp */
