//
//  YiJieYiJieLoginScene.hpp
//  tiegao
//
//  Created by mac on 16/9/9.
//
//

#ifndef YiJieLoginScene_hpp
#define YiJieLoginScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class YiJieLoginScene : public CCLayer
{
public:
    ~YiJieLoginScene();
    static CCScene* scene();
    CREATE_FUNC(YiJieLoginScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void create_views();
    void show_nicknameview();
    
    void fast_login_callback_900(CCObject* pObj);   // 开始登录游戏工程
    void game_login_callback_902(CCObject* pObj);
    void save_nickname_callback_904(CCObject* pObj);
};

#endif /* YiJieLoginScene_hpp */
