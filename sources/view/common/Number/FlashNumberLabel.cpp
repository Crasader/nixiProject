//
//  FlashNumberLabel.cpp
//  manman_client
//
//  Created by lakkey on 14-4-6.
//
//

#include "FlashNumberLabel.h"

FlashNumberLabel* FlashNumberLabel::create_with_atlas(const char *atlas, const char *origin, int step, float frame_duration)
{
    CCSprite* spt_temp = CCSprite::create(atlas);
    if (spt_temp) {
        FlashNumberLabel *pRet = new FlashNumberLabel();
        
        CCSize size = spt_temp->boundingBox().size;
        pRet->m_sigle_num_size = CCSizeMake(size.width / 10.f, size.height);
        CCLog("width = %f, height = %f", pRet->m_sigle_num_size.width, pRet->m_sigle_num_size.height);
        if(pRet && pRet->init_with_atlas(origin, atlas, pRet->m_sigle_num_size.width, pRet->m_sigle_num_size.height, '0', step, frame_duration))
        {
            pRet->autorelease();
            return pRet;
        }
        else {
            CC_SAFE_DELETE(pRet);
            return NULL;
        }
    }
    
    return NULL;
}


bool FlashNumberLabel::init_with_atlas(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration)
{
    if(!CCLabelAtlas::initWithString(string, charMapFile, itemWidth, itemHeight, '0')) {
        return false;
    }
    
    m_delegate = NULL;
    m_is_scrolling = false;
    
    CCLabelAtlas::setString(string);
    m_old_num = CCString::create(string)->intValue();
    m_new_num = m_old_num;
    m_step = step;
    m_fram_duration = frame_duration;
    CCAssert(m_fram_duration > 0, "m_fram_duration must > 0");
    CCAssert(m_old_num >= 0, "number must >= 0");
    
    return true;
}


void FlashNumberLabel::set_delegate(FlashNumberLabelDelegate *delegate)
{
    m_delegate = delegate;
}


void FlashNumberLabel::set_new_number(const char *label)
{
    if (m_is_scrolling) {
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel::update_number));
        m_old_num = m_new_num;
        m_is_scrolling = false;
        
        if (m_delegate) {
            m_delegate->did_number_stoped();
        }
    }
    
    //
    CCString* str_num = CCString::create(label);
    m_new_num = MAX(0, str_num->intValue()); // 显示的值不能小于0
    
    if (m_new_num == m_old_num) {
        return;
    }
    
    if (abs(m_new_num - m_old_num) >= 1000) {
        m_step = 123;
    }
    else if (abs(m_new_num - m_old_num) >= 200) {
        m_step = 13;
    }else if (abs(m_new_num - m_old_num) >= 50 && abs(m_new_num - m_old_num) < 200){
        m_step = 9;
    }else if (abs(m_new_num - m_old_num) >= 20 && abs(m_new_num - m_old_num) < 100){
        m_step = 3;
    }else if (abs(m_new_num - m_old_num) >= 10 && abs(m_new_num - m_old_num) < 50){
        m_step = 2;
    }else if (abs(m_new_num - m_old_num) < 10){
        m_step = 1;
    }
    
    if ((m_old_num < m_new_num && 0 > m_step) ||
        (m_old_num > m_new_num && 0 < m_step)) {
        m_step = -m_step;
    }
    
    m_is_scrolling = true;
    this->schedule(SEL_SCHEDULE(&FlashNumberLabel::update_number), m_fram_duration);
    if (m_delegate) {
        m_delegate->will_number_scroll();
    }
}


void FlashNumberLabel::set_step(int new_step)
{
    m_step = new_step;
}


void FlashNumberLabel::update_number(float delta)
{
    if ( (m_step > 0 && ((m_old_num += m_step) >= m_new_num))
        || (m_step < 0 && ((m_old_num += m_step) <= m_new_num)) ) {
        
        m_old_num = m_new_num;
        m_is_scrolling = false;
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel::update_number));
        
        if (m_delegate) {
            m_delegate->did_number_stoped();
        }
    }
    
    CCLabelAtlas::setString(CCString::createWithFormat("%d", m_old_num)->getCString());
}




