//
//  FlashNumberLabel3.cpp
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#include "FlashNumberLabel3.h"


FlashNumberLabel3* FlashNumberLabel3::create_with_atlas3(const char *atlas, const char *origin, int step, float frame_duration)
{
    CCSprite* spt_temp = CCSprite::create(atlas);
    if (spt_temp) {
        FlashNumberLabel3 *pRet = new FlashNumberLabel3();
        
        CCSize size = spt_temp->boundingBox().size;
        pRet->m_sigle_num_size3 = CCSizeMake(size.width / 10.f, size.height);
        CCLog("width = %f, height = %f", pRet->m_sigle_num_size3.width, pRet->m_sigle_num_size3.height);
        if(pRet && pRet->init_with_atlas3(origin, atlas, pRet->m_sigle_num_size3.width, pRet->m_sigle_num_size3.height, '0', step, frame_duration))
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


bool FlashNumberLabel3::init_with_atlas3(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration)
{
    if(!CCLabelAtlas::initWithString(string, charMapFile, itemWidth, itemHeight, '0')) {
        return false;
    }
    
    m_delegate3 = NULL;
    m_is_scrolling3 = false;
    
    CCLabelAtlas::setString(string);
    m_old_num3 = CCString::create(string)->intValue();
    m_new_num3 = m_old_num3;
    m_step3 = step;
    m_fram_duration3 = frame_duration;
    CCAssert(m_fram_duration3 > 0, "m_fram_duration must > 0");
    CCAssert(m_old_num3 >= 0, "number must >= 0");
    
    return true;
}


void FlashNumberLabel3::set_delegate3(FlashNumberLabel3Delegate *delegate)
{
    m_delegate3 = delegate;
}


void FlashNumberLabel3::set_new_number3(const char *label)
{
    if (m_is_scrolling3) {
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel3::update_number3));
        m_old_num3 = m_new_num3;
        m_is_scrolling3 = false;
        
        if (m_delegate3) {
            m_delegate3->did_number_stoped3();
        }
    }
    
    //
    CCString* str_num = CCString::create(label);
    m_new_num3 = MAX(0, str_num->intValue()); // 显示的值不能小于0
    
    if (m_new_num3 == m_old_num3) {
        return;
    }
    
    if (abs(m_new_num3 - m_old_num3) >= 1000) {
        m_step3 = 123;
    }
    else if (abs(m_new_num3 - m_old_num3) >= 200) {
        m_step3 = 13;
    }else if (abs(m_new_num3 - m_old_num3) >= 100 && abs(m_new_num3 - m_old_num3) < 200){
        m_step3 = 11;
    }else if (abs(m_new_num3 - m_old_num3) >= 50 && abs(m_new_num3 - m_old_num3) < 100){
        m_step3 = 9;
    }else if (abs(m_new_num3 - m_old_num3) >= 20 && abs(m_new_num3 - m_old_num3) < 50){
        m_step3 = 3;
    }else if (abs(m_new_num3 - m_old_num3) >= 10 && abs(m_new_num3 - m_old_num3) < 20){
        m_step3 = 2;
    }else if (abs(m_new_num3 - m_old_num3) < 10){
        m_step3 = 1;
    }
    
    if ((m_old_num3 < m_new_num3 && 0 > m_step3) ||
        (m_old_num3 > m_new_num3 && 0 < m_step3)) {
        m_step3 = -m_step3;
    }
    
    m_is_scrolling3 = true;
    this->schedule(SEL_SCHEDULE(&FlashNumberLabel3::update_number3), m_fram_duration3);
    if (m_delegate3) {
        m_delegate3->will_number_scroll3();
    }
}


void FlashNumberLabel3::set_step3(int new_step)
{
    m_step3 = new_step;
}


void FlashNumberLabel3::update_number3(float delta)
{
    if ( (m_step3 > 0 && ((m_old_num3 += m_step3) >= m_new_num3))
        || (m_step3 < 0 && ((m_old_num3 += m_step3) <= m_new_num3)) ) {
        
        m_old_num3 = m_new_num3;
        m_is_scrolling3 = false;
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel3::update_number3));
        
        if (m_delegate3) {
            m_delegate3->did_number_stoped3();
        }
    }
    
    CCLabelAtlas::setString(CCString::createWithFormat("%d", m_old_num3)->getCString());
}






