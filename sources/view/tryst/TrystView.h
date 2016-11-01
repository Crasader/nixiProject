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

class TrystDialogDisplayFactory
{
public:
    static CCLayerColor* createDialogDisplay(bool isLeft, const char* dialog);
    
};

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
    void appearDialog(CCLayerColor* dialogDisplay);
    void whenDialogAppeared();
    
private:
    CCTableView*            _dialogListView;
    CCArray*                _dataSource;
};

#endif /* TrystView_hpp */
