//
//  TestScene.hpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#ifndef TestScene_hpp
#define TestScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class TestScene : public CCScene
{
public:
    CREATE_FUNC(TestScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
protected:
    void create_view();
    
private:
    void fast_login();
};

#endif /* TestScene_hpp */
