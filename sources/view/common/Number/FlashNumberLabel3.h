//
//  FlashNumberLabel3.h
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#ifndef __tiegao__FlashNumberLabel3__
#define __tiegao__FlashNumberLabel3__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FlashNumberLabel3Delegate
{
public:
    virtual void will_number_scroll3() = 0;
    virtual void did_number_stoped3() = 0;
};


class FlashNumberLabel3 : public CCLabelAtlas
{
public:
    static FlashNumberLabel3* create_with_atlas3(const char *atlas, const char *origin, int step = 50, float frame_duration = 1.0/60.0);
    
    virtual void set_new_number3(const char *label);
    virtual void set_delegate3(FlashNumberLabel3Delegate* delegate);
    void set_step3(int new_step);
    
protected:
    virtual bool init_with_atlas3(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration);
    void update_number3(float delta);
    
    FlashNumberLabel3Delegate* m_delegate3;
    CCSize          m_sigle_num_size3;
    int             m_old_num3;
    int             m_new_num3;
    int             m_step3;
    float           m_fram_duration3;
    
    bool            m_is_scrolling3;
};
#endif /* defined(__tiegao__FlashNumberLabel3__) */
