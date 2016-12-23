//
//  WelfareCell.hpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#ifndef WelfareCell_hpp
#define WelfareCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class WelfareItem;

class WelfareCell : public CCTableViewCell
{
public:
    ~WelfareCell();
    
    void configWithWelfareItem(int idx, WelfareItem* item, float cellWidth, float cellHeight);
    
private:
    CCProgressTimer*        _progress;
};

#endif /* WelfareCell_hpp */
