//
//  SliderBar.cpp
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#include "SliderBar.h"
SliderBar * SliderBar::createSliderBar(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize1,CCSize bgSize2){
    SliderBar * pRet=new SliderBar();
    if (pRet && pRet->init(sliderFileName, backgroundFileName,bgSize1,bgSize2)) {
        pRet->autorelease();
        return pRet;
    }
    else
        CC_SAFE_DELETE(pRet);
    
    return NULL;
}

SliderBar::SliderBar(){
}
SliderBar::~SliderBar(){
}
bool SliderBar::init(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize1,CCSize bgSize2){
    
    /*  初始化变量  */
    _bgSize=bgSize1;
    
    /* 一定要使用九宫格精灵 */
    m_backgroundSprite=CCScale9Sprite::create(backgroundFileName);
    m_backgroundSprite->setContentSize(CCSizeMake(bgSize1.width, m_backgroundSprite->getContentSize().height));
    this->addChild(m_backgroundSprite);
    
    m_sliderSprite=CCScale9Sprite::create(sliderFileName);
    m_sliderSprite->setContentSize(bgSize2);
    this->addChild(m_sliderSprite);
    
    _beginPos=ccp(-_bgSize.width/2 + m_sliderSprite->getContentSize().width/2,0);
    _endPos= ccp(_bgSize.width/2 - m_sliderSprite->getContentSize().width/2,0);
    m_sliderSprite->setPosition(_beginPos);
    
    _sdSize=m_sliderSprite->getContentSize();
    
    return true;
}

void SliderBar::setValue(float value){
    
    /*  正常区间范围活动 */
    if (value>=0 && value<=1) {
        /*  重新设置位置 */
        m_sliderSprite->setPosition(ccp(_beginPos.x+(_endPos.x-_beginPos.x) * value , 0));
    }
    /*  滑动到最左侧    */
    else if (value<0){
        /*  重新设置大小及位置  */
        m_sliderSprite->setContentSize(CCSize(_sdSize.width+value *_sdSize.width,_sdSize.height));
        m_sliderSprite->setPosition(ccp(_beginPos.x+value *_sdSize.width/2, 0));
    }
    /*  滑动到最右侧    */
    else if (value>1){
        /*  重新设置大小及位置  */
        m_sliderSprite->setContentSize(CCSize(_sdSize.width+(1-value) *_sdSize.width,_sdSize.height));
        m_sliderSprite->setPosition(ccp(_endPos.x-(1-value) *_sdSize.width/2, 0));
    }
    
}



