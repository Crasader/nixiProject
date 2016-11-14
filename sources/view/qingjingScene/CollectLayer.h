//
//  CollectLayer.h
//  tiegao
//
//  Created by mac on 16-11-11.
//
//

#ifndef __tiegao__CollectLayer__
#define __tiegao__CollectLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class CollectLayer
: public CCLayer
{
public:
    CollectLayer();
    ~CollectLayer();
    
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(CollectLayer);
    
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    void backCallBack(CCObject* pSender);
    
public:
    
    int num_child;
    
    CCSprite* bgSpr;
    
public:
    
    void creat_view();
    void bgMethods(CCObject* obj);
    
};

#endif /* defined(__tiegao__CollectLayer__) */
