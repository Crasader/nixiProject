//
//  LoginScene.hpp
//  tiegao
//
//  Created by mac on 16/6/17.
//
//

#ifndef LoginScene_hpp
#define LoginScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class LoginScene : public CCLayer
{
public:
    ~LoginScene();
    static CCScene* scene();
    CREATE_FUNC(LoginScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void create_views();
    void show_loginview();
    void show_registview();
    void slide_in_logo();
    CCLayer* loginview();
    CCLayer* registview();
    
private:
    CCDictionary*           _views;
    CCLayer*                _container;
};

#endif /* LoginScene_hpp */
