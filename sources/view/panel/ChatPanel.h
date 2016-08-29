//
//  ChatPanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/7/29.
//
//

#ifndef ChatPanel_hpp
#define ChatPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ChatPanel: public CCLayer
, public CCTextFieldDelegate
{
public:
    ~ChatPanel();
    CREATE_FUNC(ChatPanel);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    void initChatPanel();
    
    void btn_sendMessage(CCMenuItem* item);
    
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF* sender);                                 //当用户启动虚拟键盘的时候的回调函数
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF* sender);                                 //当用户关闭虚拟键盘的时候的回调函数
    virtual bool onTextFieldInsertText(CCTextFieldTTF* sender, const char* text, int nLen);        //当用户输入的时候的回调函数
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF* sender, const char* delText, int nLen); //当用户删除文字的时候的回调函数
    
//    void keyBackStatus(float dt);
//    virtual void keyBackClicked();
    
    
//    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
//    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
//    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
//    virtual void editBoxReturn(CCEditBox* editBox);
    
public:
    void show_panel();
    
private:
    void initTopMessage();
    void closeChatPanel();
    
//    void send_replay_callback();
//    void updateSendTime();
//    void start_updatetime(int secondLeft);
//    void schedule_count_down(float dt);
    
    void nc_on_emoticon(CCObject* pObj);
    
private:
    CCSprite*           _panel_bg;
    CCSprite*           _input_bg;
    
    CCMenuItemSprite* item_send;
//    CCSprite* send_mask;
//    CCLabelAtlas* lab_time;
//    float               _timeLeft;
    
    CCTextFieldTTF* _input_text;
//    CCEditBox* _input_text;
    CCLabelTTF* _text_lab;
    CCClippingNode* _node;
    bool isCanClose;
};

#endif /* ChatPanel_hpp */
