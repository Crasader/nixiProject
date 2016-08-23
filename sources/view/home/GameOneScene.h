//
//  GameOneScene.hpp
//  tiegao
//
//  Created by stevenLi on 16/8/17.
//
//

#ifndef GameOneScene_hpp
#define GameOneScene_hpp

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class GameOneScene :public CCLayer{
public:
    ~GameOneScene();
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(GameOneScene);
    
    virtual void onEnter();
    virtual void onExit();
    
//    void initView();
    void btn_back_callback(CCObject* obj);
    void createGarbage(float dt);
    void startCreateGarbage();
    void updateTime(float dt);
    void update(float dt);
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCLabelTTF*         _time;
    CCSprite*           _boy;
//    CCArray*            _
};

#endif /* GameOneScene_hpp */
