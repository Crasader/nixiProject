//
//  FindPanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/21.
//
//

#ifndef FindPanel_hpp
#define FindPanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"


USING_NS_CC;
USING_NS_CC_EXT;


class FindPanel
: public CCLayer
, public CursorTextFieldDelegate
{
public:
    ~FindPanel();
    
    CREATE_FUNC(FindPanel);
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool init();
//    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual bool onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte);
    virtual void onCursorTextFieldAttachWithIME(CursorTextField * sender);
    virtual void onCursorTextFieldDetachWithIME(CursorTextField * sender);
    //
    //    virtual void keyBackClicked();
    //
    //    static FindPanel* create_with_other(Other* _other, bool _otherBool);
    //    void init_with_other(Other* _other, bool _otherBool);
    //
    //    static FindPanel* create_with_rank(RankItem* _item, bool _rankBool);
    //    void init_with_rank(RankItem* _item, bool _rankBool);
    //
    //    static FindPanel* create_with_paper(PaperItem* _paper, bool _paperBool);
    //    void init_with_paper(PaperItem* _paper, bool _paperBool);
    
    //    virtual void Attachaction();
    //    virtual void Detachaction();
    
    
    //    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvents) {return true;}
    //    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){};
    //    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){};
    //    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){}
    
    //    virtual void editBoxEditingDidBegin(CCEditBox* editBox);
    //    virtual void editBoxEditingDidEnd(CCEditBox* editBox);
    //    virtual void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    //    virtual void editBoxReturn(CCEditBox* editBox);
    
public:
    
//    bool otherBool;
//    bool rankBool;
//    bool paperBool;
//    bool isOpen;
    
    CCSprite* find_panel;
    
    CCMenu* menu;
    
    //    Other* other;
    //    RankItem* item;
    //    PaperItem* paper;
    
    //    MMPromptLayer* promptLayer;
    
    //    MMJYTextField* m_text;
    CursorTextField*  m_text;
    
public:
    
    void initView();
    void btn_find_callback();
    void btn_back_callback();
    void find_callback_801(CCObject* obj);
    //    void fhCallBack(CCObject* pSender);
    //    void fsCallBack(CCObject* pSender);
    //    void close_view(float dt);
    
};
#endif /* FindPanel_hpp */
