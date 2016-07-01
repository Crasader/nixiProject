//
//  FlashNumberLabel.h
//  manman_client
//
//  Created by lakkey on 14-4-6.
//
//

#ifndef __manman_client__FlashNumberLabel__
#define __manman_client__FlashNumberLabel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FlashNumberLabelDelegate
{
public:
    virtual void will_number_scroll() = 0;
    virtual void did_number_stoped() = 0;
};


class FlashNumberLabel : public CCLabelAtlas
{
public:
    static FlashNumberLabel* create_with_atlas(const char *atlas, const char *origin, int step = 50, float frame_duration = 1.0/60.0);
    
    virtual void set_new_number(const char *label);
    virtual void set_delegate(FlashNumberLabelDelegate* delegate);
    void set_step(int new_step);
    
protected:
    virtual bool init_with_atlas(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration);
    void update_number(float delta);
    
    FlashNumberLabelDelegate* m_delegate;
    CCSize          m_sigle_num_size;
    int             m_old_num;
    int             m_new_num;
    int             m_step;
    float           m_fram_duration;
    
    bool            m_is_scrolling;
};

#endif /* defined(__manman_client__FlashNumberLabel__) */
