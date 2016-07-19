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
class ShowerView;
class FriendsListView;

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
    void create_show_view();
    void create_empty_prompt();
    
    void update_self_panel(ShowComp* self);
    
    void on_btn_back_to_social(CCMenuItem* menuItem);
    void on_btn_self_panel(CCMenuItemToggle* menuItem);
    void on_btn_goto_starngers(CCMenuItem* menuItem);
    void on_btn_take_energy(CCMenuItem* menuItem);
    void on_btn_send_paper(CCMenuItem* menuItem);
    
    void nc_change_shower(CCObject* pObj);
    void nc_goto_strangers_802(CCObject* pObj);
    void nc_take_energy_807(CCObject* pObj);

    
    void btn_no_realize(CCMenuItem* menuItem);

private:
    CCArray*                _data;
    ShowerView*             _showerView;
    FriendsListView*        _listView;
    
    int                     _curIndex;
    
    CCMenuItem*             _btnPaper;
    
    CCMenuItemToggle*       _btnSelfPanel;
    CCMenuItemImage*        _selfPanelNormal;
    CCMenuItemImage*        _selfPanelSelected;
    CCNode*                 _nodeNormal;
    CCNode*                 _nodeSelected;
};

#endif /* FriendsScene_hpp */
