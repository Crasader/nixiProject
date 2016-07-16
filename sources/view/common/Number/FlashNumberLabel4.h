//
//  FlashNumberLabel4.h
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#ifndef __tiegao__FlashNumberLabel4__
#define __tiegao__FlashNumberLabel4__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FlashNumberLabel4Delegate
{
public:
    virtual void will_number_scroll4() = 0;
    virtual void did_number_stoped4() = 0;
};


class FlashNumberLabel4 : public CCLabelAtlas
{
public:
    static FlashNumberLabel4* create_with_atlas4(const char *atlas, const char *origin, int step = 50, float frame_duration = 1.0/60.0);
    
    virtual void set_new_number4(const char *label);
    virtual void set_delegate4(FlashNumberLabel4Delegate* delegate);
    void set_step4(int new_step);
    
protected:
    virtual bool init_with_atlas4(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration);
    void update_number4(float delta);
    
    FlashNumberLabel4Delegate* m_delegate4;
    CCSize          m_sigle_num_size4;
    int             m_old_num4;
    int             m_new_num4;
    int             m_step4;
    float           m_fram_duration4;
    
    bool            m_is_scrolling4;
};
#endif /* defined(__tiegao__FlashNumberLabel4__) */
