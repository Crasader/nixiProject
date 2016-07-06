//
//  Building.hpp
//  paperai
//
//  Created by mac on 16/7/3.
//
//

#ifndef Building_hpp
#define Building_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Building
    : public CCLayer
//    , CCScrollViewDelegate
{
public:
    ~Building();
    static Building* create(int floorCount);
    bool init(int floorCount);
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
//    virtual void scrollViewDidScroll(CCScrollView* view);
//    virtual void scrollViewDidZoom(CCScrollView* view);
    
private:
    void init_floors();
    CCPoint posWithFloor(int floor);
    void start();
    void go_back();
    void on_take_rewards(CCMenuItem* btn);
    
private:
    CCScrollView*   _scroll;
    int             _floorCount;
    CCArray*        _floors;
};

#endif /* Building_hpp */
