//
//  GashaponScene.h
//  tiegao
//
//  Created by mac on 17-1-3.
//
//

#ifndef __tiegao__GashaponScene__
#define __tiegao__GashaponScene__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"


USING_NS_CC;

class GashaponScene : public BaseScene
{
public:
    GashaponScene();
    ~GashaponScene();
    static GashaponScene* create();
    virtual bool init();
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
        
public:
    
    void creat_view();
    void creat_Exchange();
    void backCallBack(CCObject* pSender);
    
public:
    
    int num_child;
    
};
#endif /* defined(__tiegao__GashaponScene__) */
