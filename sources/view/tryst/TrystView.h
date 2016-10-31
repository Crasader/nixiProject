//
//  TrystView.hpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#ifndef TrystView_hpp
#define TrystView_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystView : public CCScene
{
public:
    ~TrystView();
    static TrystView* create(const char* id);
    
private:
    virtual bool init(const char* id);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    
private:
    
};

#endif /* TrystView_hpp */
