//
//  NoteLayer.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/29.
//
//

#ifndef NoteLayer_hpp
#define NoteLayer_hpp

#include <iostream>
#include "cocos2d.h"
#include "BaseScene.h"
#include "PromptLayer.h"
#include "AHMessageBox.h"

USING_NS_CC;

class NoteLayer
:public BaseScene
,public AHMessageBoxDelegate
{
public:
    
public:
    
    ~NoteLayer();
    
    virtual bool init();
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag);
    
    virtual void keyBackClicked();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(NoteLayer);
    
public:
    CCSprite* bgSpr;
    CCSprite* dikuangSpr;
    
    CCMenu* allMenu;
    
    PromptLayer* promptLayer;
    
public:
    
    void create_view();
    void backCallback(CCObject* pSender);
};


#endif /* NoteLayer_hpp */
