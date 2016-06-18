//
//  FlashNumberLabel22.h
//  mm3c
//
//  Created by mac on 16-1-8.
//
//

#ifndef __mm3c__FlashNumberLabel2__
#define __mm3c__FlashNumberLabel2__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class FlashNumberLabel2Delegate
{
public:
    virtual void will_number_scroll2() = 0;
    virtual void did_number_stoped2() = 0;
};


class FlashNumberLabel2 : public CCLabelAtlas
{
public:
    static FlashNumberLabel2* create_with_atlas2(const char *atlas, const char *origin, int step = 50, float frame_duration = 1.0/60.0);
    
    virtual void set_new_number2(const char *label);
    virtual void set_delegate2(FlashNumberLabel2Delegate* delegate);
    void set_step2(int new_step);
    
protected:
    virtual bool init_with_atlas2(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration);
    void update_number2(float delta);
    
    FlashNumberLabel2Delegate* m_delegate2;
    CCSize          m_sigle_num_size2;
    int             m_old_num2;
    int             m_new_num2;
    int             m_step2;
    float           m_fram_duration2;
    
    bool            m_is_scrolling2;
};

#endif /* defined(__mm3c__FlashNumberLabel22__) */
