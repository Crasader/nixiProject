//
//  UnreusedTableView.hpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#ifndef UnreusedTableView_hpp
#define UnreusedTableView_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class UnreusedTableView : public CCTableView
{
public:
    virtual ~UnreusedTableView();
    static UnreusedTableView* create(CCTableViewDataSource* dataSource, CCSize size);
    static UnreusedTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container);
    bool initWithViewSize(CCSize size, CCNode* container = NULL);
    
    void scrollViewDidScroll(CCScrollView* view);
    
protected:
    void _moveCellOutOfSight(CCTableViewCell *cell);
    
private:
    
};

#endif /* UnreusedTableView_hpp */