//
//  FlashNumberLabel4.cpp
//  tiegao
//
//  Created by mac on 16-7-15.
//
//

#include "FlashNumberLabel4.h"


FlashNumberLabel4* FlashNumberLabel4::create_with_atlas4(const char *atlas, const char *origin, int step, float frame_duration)
{
    CCSprite* spt_temp = CCSprite::create(atlas);
    if (spt_temp) {
        FlashNumberLabel4 *pRet = new FlashNumberLabel4();
        
        CCSize size = spt_temp->boundingBox().size;
        pRet->m_sigle_num_size4 = CCSizeMake(size.width / 10.f, size.height);
        CCLog("width = %f, height = %f", pRet->m_sigle_num_size4.width, pRet->m_sigle_num_size4.height);
        if(pRet && pRet->init_with_atlas4(origin, atlas, pRet->m_sigle_num_size4.width, pRet->m_sigle_num_size4.height, '0', step, frame_duration))
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


bool FlashNumberLabel4::init_with_atlas4(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap, int step, float frame_duration)
{
    if(!CCLabelAtlas::initWithString(string, charMapFile, itemWidth, itemHeight, '0')) {
        return false;
    }
    
    m_delegate4 = NULL;
    m_is_scrolling4 = false;
    
    CCLabelAtlas::setString(string);
    m_old_num4 = CCString::create(string)->intValue();
    m_new_num4 = m_old_num4;
    m_step4 = step;
    m_fram_duration4 = frame_duration;
    CCAssert(m_fram_duration4 > 0, "m_fram_duration must > 0");
    CCAssert(m_old_num4 >= 0, "number must >= 0");
    
    return true;
}


void FlashNumberLabel4::set_delegate4(FlashNumberLabel4Delegate *delegate)
{
    m_delegate4 = delegate;
}


void FlashNumberLabel4::set_new_number4(const char *label)
{
    if (m_is_scrolling4) {
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel4::update_number4));
        m_old_num4 = m_new_num4;
        m_is_scrolling4 = false;
        
        if (m_delegate4) {
            m_delegate4->did_number_stoped4();
        }
    }
    
    //
    CCString* str_num = CCString::create(label);
    m_new_num4 = MAX(0, str_num->intValue()); // 显示的值不能小于0
    
    if (m_new_num4 == m_old_num4) {
        return;
    }
    
    if (abs(m_new_num4 - m_old_num4) >= 1000) {
        m_step4 = 123;
    }
    else if (abs(m_new_num4 - m_old_num4) >= 200) {
        m_step4 = 13;
    }else if (abs(m_new_num4 - m_old_num4) >= 100 && abs(m_new_num4 - m_old_num4) < 200){
        m_step4 = 11;
    }else if (abs(m_new_num4 - m_old_num4) >= 50 && abs(m_new_num4 - m_old_num4) < 100){
        m_step4 = 9;
    }else if (abs(m_new_num4 - m_old_num4) >= 20 && abs(m_new_num4 - m_old_num4) < 50){
        m_step4 = 3;
    }else if (abs(m_new_num4 - m_old_num4) >= 10 && abs(m_new_num4 - m_old_num4) < 20){
        m_step4 = 2;
    }else if (abs(m_new_num4 - m_old_num4) < 10){
        m_step4 = 1;
    }
    
    if ((m_old_num4 < m_new_num4 && 0 > m_step4) ||
        (m_old_num4 > m_new_num4 && 0 < m_step4)) {
        m_step4 = -m_step4;
    }
    
    m_is_scrolling4 = true;
    this->schedule(SEL_SCHEDULE(&FlashNumberLabel4::update_number4), m_fram_duration4);
    if (m_delegate4) {
        m_delegate4->will_number_scroll4();
    }
}


void FlashNumberLabel4::set_step4(int new_step)
{
    m_step4 = new_step;
}


void FlashNumberLabel4::update_number4(float delta)
{
    if ( (m_step4 > 0 && ((m_old_num4 += m_step4) >= m_new_num4))
        || (m_step4 < 0 && ((m_old_num4 += m_step4) <= m_new_num4)) ) {
        
        m_old_num4 = m_new_num4;
        m_is_scrolling4 = false;
        this->unschedule(SEL_SCHEDULE(&FlashNumberLabel4::update_number4));
        
        if (m_delegate4) {
            m_delegate4->did_number_stoped4();
        }
    }
    
    CCLabelAtlas::setString(CCString::createWithFormat("%d", m_old_num4)->getCString());
}



