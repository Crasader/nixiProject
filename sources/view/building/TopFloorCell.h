//
//  TopFloorCell.hpp
//  tiegao
//
//  Created by mac on 16/8/18.
//
//

#ifndef TopFloorCell_hpp
#define TopFloorCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

class TopFloorCell : public CCTableViewCell
{
public:
    CC_SYNTHESIZE_RETAIN(CCArray*, _boxes, Boxes);

public:
    virtual ~TopFloorCell();
    static TopFloorCell* create(int phase, int idx);
    bool init(int phase, int idx);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
private:
    void update_boxes();
    void nc_take_company_reward_205(CCObject* pObj);
    
private:
    CCSprite*           _sptFloor;
    CCProgressTimer*    _progress;
    CCSprite*           _topBar;
};

#endif /* TopFloorCell_hpp */
