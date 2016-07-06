//
//  FloorCell.hpp
//  tiegao
//
//  Created by mac on 16/7/6.
//
//

#ifndef FloorCell_hpp
#define FloorCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

const float FLOOR_CELL_WIDTH = 544.f;
const float FLOOR_CELL_HEIGHT = 194.f;

typedef enum {
    FloorCellType_Reception = 1,
    FloorCellType_Office = 2,
    FloorCellType_Manager_Office = 3,
    FloorCellType_Leisure = 4,
    FloorCellType_Roof = 5,
} FloorCellType;

class FloorCell : public CCTableViewCell
{
public:
    virtual ~FloorCell();
    static FloorCell* create(FloorCellType type, int phase, int idx);
    bool init(FloorCellType type, int phase, int idx);
    
public:
    void start();
    
private:
    CCPoint randomStartPos(bool left);
    float randomDuration();
    CCPoint randomEdge();
    void on_take_rewards(CCMenuItem *btn);
    
private:
    FloorCellType       _type;
    CCSprite*           _sptFloor;
    CCArray*            _roles;
};

#endif /* FloorCell_hpp */
