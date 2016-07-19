//
//  ShowerView.hpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#ifndef ShowerView_hpp
#define ShowerView_hpp

#include "cocos2d.h"

USING_NS_CC;

class ShowerView : public CCLayer
{
public:
    CREATE_FUNC(ShowerView);
    void change_shower(CCDictionary* clothes);
    
public:
    ~ShowerView();
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
private:
    void change_finish();
    
private:
    CCNode*             _curShower;
};

#endif /* ShowerView_hpp */
