//
//  SliderBar.h
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#ifndef __tiegao__SliderBar__
#define __tiegao__SliderBar__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class SliderBar:public CCNode{
    
public:
    
    /* 创建对象 参数：滑动条资源 背景资源 背景大小 滑动条大小比率 */
    static SliderBar * createSliderBar(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize1,CCSize bgSize2);
    /* 初始化方法 */
    bool init(const char * sliderFileName,const char * backgroundFileName,CCSize bgSize1,CCSize bgSize2);
    SliderBar();   //构造函数
    ~SliderBar();  //析构函数
    /* 设置滚动条精灵在其背景精灵上位置的百分比 区间[0,1] 当超过这个范围时进行缩放 */
    void setValue(float value);
    
public:
    /* 创建成员变量及get、set方法 */
    CC_SYNTHESIZE(CCScale9Sprite *, m_sliderSprite, SliderSprite);
    CC_SYNTHESIZE(CCScale9Sprite *, m_backgroundSprite, BackgroundSprite);
    /* 滚动条精灵初始位置 */
    CCPoint _beginPos;
    /* 滚动条精灵最终位置 */
    CCPoint _endPos;
    /* 滚动条背景精灵大小 */
    CCSize _bgSize;
    /*  滚动条精灵大小   */
    CCSize _sdSize;
};
#endif /* defined(__tiegao__SliderBar__) */
