//
//  MessageLayer.h
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#ifndef __tiegao__MessageLayer__
#define __tiegao__MessageLayer__

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
//#include "ClothesTableView.h"
#include "AHMessageBox.h"

USING_NS_CC;


class MessageLayer
: public BaseScene
, public AHMessageBoxDelegate
{
public:
    
    MessageLayer();
    ~MessageLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    virtual void keyBackClicked();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(MessageLayer);
    
public:
    
//    ClothesTableView* _delegate;
    
    
    CCSprite* bgSpr;
    CCSprite* dikuangSpr;
    
    CCMenu* allMenu;
    
public:
    
    void creat_View();
    
    void backCallBack(CCObject* pSender);
    
private:
//    void Http_Finished_401(CCObject* pObj);
    
};
#endif /* defined(__tiegao__MessageLayer__) */
