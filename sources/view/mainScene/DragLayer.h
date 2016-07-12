//
//  DragLayer.hpp
//  tiegao
//
//  Created by stevenLi on 16/7/12.
//
//

#ifndef DragLayer_hpp
#define DragLayer_hpp

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class DragLayer: public CCLayer
{
public:
    ~DragLayer();
    
    CREATE_FUNC(DragLayer);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
        
    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent){}
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
    
private:
    CCPoint last_pos;
    CCPoint cur_pos;
};

#endif /* DragLayer_hpp */
