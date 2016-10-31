//
//  TrystScene.hpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#ifndef TrystScene_hpp
#define TrystScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class TrystScene : public CCScene
{
public:
    ~TrystScene();
    static TrystScene* create(const char* id);
    
private:
    virtual bool init(const char* id);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    
private:
    
};

#endif /* TrystScene_hpp */
