//
//  FlashNumberLabel22.cpp
//  mm3c
//
//  Created by mac on 16-1-8.
//
//

#include "FlashNumberLabel2.h"


FlashNumberLabel2* FlashNumberLabel2::create_with_atlas2(const char *atlas, const char *origin, int step, float frame_duration)
{
    CCSprite* spt_temp = CCSprite::create(atlas);
    if (spt_temp) {
        FlashNumberLabel2 *pRet = new FlashNumberLabel2();
        
        CCSize size = spt_temp->boundingBox().size;
        pRet->m_sigle_num_size2 = CCSizeMake(size.width / 10.f, size.height);
        if(pRet && pRet->init_with_atlas2(origin, atlas, pRet->m_sigle_num_size2.width, pRet->m_sigle_num_size2.height, '0', step, frame_duration))
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


bool FlashNumberLabel2::init_with_atlas2(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration)
{
    if(!CCLabelAtlas::initWithString(string, charMapFile, itemWidth, itemHeight, '0')) {
        return false;
    }
    
    m_delegate2 = NULL;
    m_is_scrolling2 = false;
    
    CCLabelAtlas::setString(string);
    m_old_num2 = CCString::create(string)->intValue();
    m_new_num2 = m_old_num2;
    m_step2 = step;
    m_fram_duration2 = frame_duration;
    CCAssert(m_fram_duration2 > 0, "m_fram_duration must > 0");
    CCAssert(m_old_num2 >= 0, "number must >= 0");
    
    return true;
}


void FlashNumberLabel2::set_delegate2(FlashNumberLabel2Delegate *delegate)
{
    m_delegate2 = delegate;
}


void FlashNumberLabel2::set_new_number2(const char *label)
{
    if (m_is_scrolling2) {
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel2::update_number2));
        m_old_num2 = m_new_num2;
        m_is_scrolling2 = false;
        
        if (m_delegate2) {
            m_delegate2->did_number_stoped2();
        }
    }
    
    //
    CCString* str_num = CCString::create(label);
    m_new_num2 = MAX(0, str_num->intValue()); // 显示的值不能小于0
    
    if (m_new_num2 == m_old_num2) {
        return;
    }
    
    if (abs(m_new_num2 - m_old_num2) >= 1000) {
        m_step2 = 123;
    }
    else if (abs(m_new_num2 - m_old_num2) >= 200) {
        m_step2 = 13;
    }else if (abs(m_new_num2 - m_old_num2) >= 100 && abs(m_new_num2 - m_old_num2) < 200){
        m_step2 = 11;
    }else if (abs(m_new_num2 - m_old_num2) >= 50 && abs(m_new_num2 - m_old_num2) < 100){
        m_step2 = 9;
    }else if (abs(m_new_num2 - m_old_num2) >= 20 && abs(m_new_num2 - m_old_num2) < 50){
        m_step2 = 3;
    }else if (abs(m_new_num2 - m_old_num2) >= 10 && abs(m_new_num2 - m_old_num2) < 20){
        m_step2 = 2;
    }else if (abs(m_new_num2 - m_old_num2) < 10){
        m_step2 = 1;
    }
    
    if ((m_old_num2 < m_new_num2 && 0 > m_step2) ||
        (m_old_num2 > m_new_num2 && 0 < m_step2)) {
        m_step2 = -m_step2;
    }
    
    m_is_scrolling2 = true;
    this->schedule(SEL_SCHEDULE(&FlashNumberLabel2::update_number2), m_fram_duration2);
    if (m_delegate2) {
        m_delegate2->will_number_scroll2();
    }
}


void FlashNumberLabel2::set_step2(int new_step)
{
    m_step2 = new_step;
}


void FlashNumberLabel2::update_number2(float delta)
{
    if ( (m_step2 > 0 && ((m_old_num2 += m_step2) >= m_new_num2))
        || (m_step2 < 0 && ((m_old_num2 += m_step2) <= m_new_num2)) ) {
        
        m_old_num2 = m_new_num2;
        m_is_scrolling2 = false;
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel2::update_number2));
        
        if (m_delegate2) {
            m_delegate2->did_number_stoped2();
        }
    }
    
    CCLabelAtlas::setString(CCString::createWithFormat("%d", m_old_num2)->getCString());
}




