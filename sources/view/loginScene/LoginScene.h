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
    void show_nicknameview();
//    void slide_in_logo();
    void init_snow(int huaIndex);
    
    void fast_login(CCMenuItem* pObj);
    void start_login(CCObject* pObj);
    void start_regist(CCObject* pObj);
    
    void get_info_999(CCObject* pObj);
    void fast_login_callback_900(CCObject* pObj);
    void account_login_callback_901(CCObject* pObj);
    void game_login_callback_902(CCObject* pObj);
    void account_regist_callback_903(CCObject* pObj);
    void save_nickname_callback_904(CCObject* pObj);
    
private:
    CCDictionary*           _views;
    CCDictionary*           _temp_account_pwd;
    CCLayer*                _container;
};

#endif /* LoginScene_hpp */
