//
//  NotePanel.hpp
//  tiegao
//
//  Created by stevenLi on 16/6/20.
//
//

#ifndef NotePanel_hpp
#define NotePanel_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CursorTextField.h"


USING_NS_CC;
USING_NS_CC_EXT;


class NotePanel
: public CCLayer
, public CursorTextFieldDelegate
{
public:
    ~NotePanel();
    
    CREATE_FUNC(NotePanel);
    virtual void onEnter();
    virtual void onExit();
    
    void keyBackStatus(float dt);
    virtual void keyBackClicked();
    
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    
    virtual bool onCursorTextFieldInsertText(CursorTextField* sender, const char* newText, int newTextByte);
    virtual bool onCursorTextFieldDeleteBackward(CursorTextField * sender, const char * delText, int nLen);
    virtual void onCursorTextFieldAttachWithIME(CursorTextField * sender);
    virtual void onCursorTextFieldDetachWithIME(CursorTextField * sender);
//
//    virtual void keyBackClicked();
//
//    static NotePanel* create_with_other(Other* _other, bool _otherBool);
//    void init_with_other(Other* _other, bool _otherBool);
//    
//    static NotePanel* create_with_rank(RankItem* _item, bool _rankBool);
//    void init_with_rank(RankItem* _item, bool _rankBool);
//    
//    static NotePanel* create_with_paper(PaperItem* _paper, bool _paperBool);
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
    
    CC_SYNTHESIZE_PASS_BY_REF(std::string, _entranceType, EntranceType);
    
//    bool otherBool;
//    bool rankBool;
//    bool paperBool;
//    bool isOpen;
    
    CCSprite* note_panel;
    
    CCMenu* menu;
    
//    Other* other;
//    RankItem* item;
//    PaperItem* paper;
        
    //    MMJYTextField* m_text;
    
    int _index;
        
    CursorTextField*  m_text;
    
    int erase_pos;
    
    CCLabelTTF* _wordNum_lab;
    int         _word_count;
    
public:
    
    void update(float dt);
    
    void initView();
    void btn_send_callback();
    void closeNotePanel();
    
    void note_callback_809();
};

#endif /* NotePanel_hpp */
