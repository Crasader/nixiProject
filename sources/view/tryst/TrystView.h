//
//  TrystView.hpp
//  tiegao
//
//  Created by mac on 16/10/31.
//
//

#ifndef TrystView_hpp
#define TrystView_hpp

#include "cocos2d.h"
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TrystInputBar : public CCNode
{
public:
    ~TrystInputBar();
    static TrystInputBar* create(CCObject *target, SEL_CallFunc callback);
    
    void startInput(const char* text);
    
private:
    virtual bool init(CCObject *target, SEL_CallFunc callback);
    void gogogo();
    
private:
    CCObject*           _target;
    SEL_CallFunc        _callback;
    CCLabelTTF*         _lbl;
    std::string         _origText;
    int                 _inputCount;
    float               _savedInputboxWidth;
};

// ------------------------------------------------------
// ------------------------------------------------------

class TrystView
: public CCLayer
, public CCTableViewDataSource
{
public:
    ~TrystView();
    static TrystView* create(const char* id);
    
    void insertDialog(bool isLeft, const char* dialog);
    
private:
    virtual bool init(const char* id);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
    virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
    
private:
    CCLayerColor* createDialogDisplay(bool isLeft, const char* dialog);
    void prepareLeftDialog(CCLayerColor *dialogDisplay);
    void prepareRightDialog(const char* inputText);
    void appearDialog(CCLayerColor* dialogDisplay);
    void whenDialogAppeared();
    void updateOtherInputPrompt(CCString* newText);
    void whenRightInputCompleted();
    
private:
    CCTableView*            _dialogListView;
    CCArray*                _dataSource;
    CCLabelTTF*             _otherInputPrompt;
    TrystInputBar*          _inputBar;
};

#endif /* TrystView_hpp */
