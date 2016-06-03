//
//  TransactionScene.hpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#ifndef TransactionScene_hpp
#define TransactionScene_hpp

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class TransactionScene : public BaseScene
{
public:
    ~TransactionScene();
    CREATE_FUNC(TransactionScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
protected:
    
private:
    
};

#endif /* TransactionScene_hpp */
