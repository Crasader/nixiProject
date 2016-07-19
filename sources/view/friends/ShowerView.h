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
    
    
public:
    ~ShowerView();
    CREATE_FUNC(ShowerView);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
};

#endif /* ShowerView_hpp */
