//
//  Floor.hpp
//  paperai
//
//  Created by mac on 16/7/3.
//
//

#ifndef Floor_hpp
#define Floor_hpp

#include "cocos2d.h"

USING_NS_CC;

const float FLOOR_WIDTH = 544.f;
const float FLOOR_HEIGHT = 194.f;

const float ROOF_WIDTH = FLOOR_WIDTH;
const float ROOF_HEIGHT = FLOOR_HEIGHT;

typedef enum {
    FloorType_Reception = 1,
    FloorType_Office = 2,
    FloorType_Manager_Office = 3,
    FloorType_Leisure = 4,
} FloorType;

class Floor : public CCObject
{
public:
    ~Floor();
    static Floor* create(CCLayer* building, int floorCount, int curFloor, FloorType type, CCPoint pos);
    bool init(CCLayer* building, int floorCount, int curFloor, FloorType type, CCPoint pos);
    
public:
    void start();
    
private:
    CCPoint randomStartPos(bool left);
    float randomDuration();
    CCPoint randomEdge();
    
private:
    FloorType           _type;
    CCSprite*           _sptFloor;
    CCArray*            _roles;
};

#endif /* Floor_hpp */
