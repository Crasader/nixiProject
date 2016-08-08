//
//  LogoScene.h
//  tiegao
//
//  Created by mac on 16-8-8.
//
//

#ifndef __tiegao__LogoScene__
#define __tiegao__LogoScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"


USING_NS_CC;

class LogoScene : public CCLayer
{
public:
    ~LogoScene();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(LogoScene);
    
    virtual void onEnter();
    virtual void onExit();
    
public:
    
    void creat_view();
    void next_scene();
};
#endif /* defined(__tiegao__LogoScene__) */
