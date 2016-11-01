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

class TrystData;
class TrystView;
class TrystDialogState;

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
    void playing(float dt);
    void onDialogAppeared();
    
private:
    TrystData*          _data;
    TrystView*          _view;
    TrystDialogState*   _state;
    int                 _nextDialogIndex;
};

#endif /* TrystScene_hpp */
