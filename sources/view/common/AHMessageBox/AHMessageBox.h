//
//  AHMessageBox.h
//  AnHeiShiJie_Self
//
//  Created by iLakkey on 13-9-7.
//
//

#ifndef __AnHeiShiJie_Self__AHMessageBox__
#define __AnHeiShiJie_Self__AHMessageBox__

#include "cocos2d.h"

USING_NS_CC;

typedef enum {
    AH_AVATAR_TYPE_NO = 0,
    AH_AVATAR_TYPE_NORMAL = 1,
    AH_AVATAR_TYPE_CONSIDER,
    AH_AVATAR_TYPE_CONFUSED,
    AH_AVATAR_TYPE_SURPRISED,
    AH_AVATAR_TYPE_HAPPY
} AH_AVATAR_TYPE;

typedef enum {
    AH_BUTTON_TYPE_YES = 1001,
    AH_BUTTON_TYPE_YES2 = 1002,
    AH_BUTTON_TYPE_YES3 = 1003,
    AH_BUTTON_TYPE_YES4 = 1004,
    
    AH_BUTTON_TYPE_YESNO = 2001,
    AH_BUTTON_TYPE_YESNO2 = 2002,
    AH_BUTTON_TYPE_YESNO3 = 2003,
    AH_BUTTON_TYPE_YESNO4 = 2004,
    AH_BUTTON_TYPE_YESNO5 = 2005,
} AH_BUTTON_TYPE;

typedef enum {
    AH_BUTTON_TAG_YES = 1,
    AH_BUTTON_TAG_MIDDLE = 2,
    AH_BUTTON_TAG_NO = 3,
    AH_BUTTON_TAG_CLOSE = 4,
} AH_BUTTON_TAGS;

// --------------------------------------------------------------

class AHMessageBox;

class AHMessageBoxDelegate
{
public:
    virtual void message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag) = 0;
};


class AHMessageBox : public CCLayerColor
{
public:
    ~AHMessageBox();
    CREATE_FUNC(AHMessageBox);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    static AHMessageBox* create_with_message(const char* message, CCObject* deleaget, AH_AVATAR_TYPE avatar_type, AH_BUTTON_TYPE button_type, bool has_close_btn);
    void clean_box();
    void animation_out();
    void on_out();
    void stopClick();
    void openClick();
    
//    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    CC_SYNTHESIZE_RETAIN(CCString*, _message, Message);
    CC_SYNTHESIZE(AH_AVATAR_TYPE, _avatar_type, Avatar_type);
    CC_SYNTHESIZE(AH_BUTTON_TYPE, _button_type, Button_type);
    
    CC_SYNTHESIZE_READONLY(CCLabelTTF*, m_lbl_msg, Lbl_msg);
    
    CC_SYNTHESIZE_READONLY(CCMenuItem*, m_item_yes, Item_yes);
    CC_SYNTHESIZE_READONLY(CCMenuItem*, m_item_middle, Item_middle);
    CC_SYNTHESIZE_READONLY(CCMenuItem*, m_item_no, Item_no);
    
protected:
    void load_content();
    void on_menuitem_click(CCObject* pSender);
//    void on_close_btn_click(CCObject* pSender);

    void change_menu_priority();
        
    CCMenu*                 m_main_menu;
    CCMenu*                 m_close_menu;
    
    bool                m_has_close_btn;
    
    AHMessageBoxDelegate* _delegate;
};

#endif /* defined(__AnHeiShiJie_Self__AHMessageBox__) */
