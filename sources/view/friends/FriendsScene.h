//
//  FriendsScene.hpp
//  tiegao
//
//  Created by mac on 16/7/19.
//
//

#ifndef FriendsScene_hpp
#define FriendsScene_hpp

#include "cocos2d.h"
#include "BaseScene.h"

USING_NS_CC;

class ShowComp;

class FriendsScene : public BaseScene
{
public:
    static CCScene* scene();
    
    
    
public:
    ~FriendsScene();
    CREATE_FUNC(FriendsScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
private:
    void create_UI();
    void create_listview();
    void create_self_panel();
    void create_show_view(ShowComp* show);
    
    void update_self_panel(ShowComp* self);
    
    void on_btn_back_to_social(CCMenuItem* menuItem);
    void on_btn_self_panel(CCMenuItem* menuItem);
    void btn_no_realize(CCMenuItem* menuItem);
    
private:
    CCArray*                _data;
    CCMenuItemImage*        _selfPanelNormal;
    CCMenuItemImage*        _selfPanelSelected;
    CCNode*                 _nodeNormal;
    CCNode*                 _nodeSelected;
};

#endif /* FriendsScene_hpp */
