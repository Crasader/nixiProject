//
//  VipDialogItem.h
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#ifndef __manman_client__VipDialogItem__
#define __manman_client__VipDialogItem__

#include <iostream>
#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class VipDialogItem : public CCObject
{
public:
    ~VipDialogItem();
    CREATE_FUNC(VipDialogItem);
    bool init();
    
    void config_static_with_json(CSJson::Value value);
    
    void clues_config_static_with_json(CSJson::Value value);
    
    //<><><><><><><><><><><>剧情数据<><><><><><><><><><><><><><><>//
    
    CC_SYNTHESIZE(int, _index, Index);// 当前场景的id
    CC_SYNTHESIZE(std::string, _name, Name);// 显示的名字
    CC_SYNTHESIZE(std::string, _nameId, NameId);// 显示要眨眼的id
    CC_SYNTHESIZE(std::string, _said, Said);// 说的话
    CC_SYNTHESIZE(int, _states, States);// 0没有人 1一个人 2有两个人 3没有变化 4只播放背景
    CC_SYNTHESIZE(int, _passersby, Passersby);// 是否有2个人 0没有 1有
    CC_SYNTHESIZE(std::string, _bg, Bg);// 显示的背景
    CC_SYNTHESIZE(int, _bust, Bust);// 1正常 2近景
    
    //<><><>主要显示的人<><><>//
    CC_SYNTHESIZE(std::string, _head_1, Head_1);// 头像
    CC_SYNTHESIZE(int, _figure_1, Figure_1);// 显示的是那个人
    CC_SYNTHESIZE(std::string, _zishi_1, Zishi_1);// 姿势
    
    //<><><>第二个要显示的人<><><>//
    CC_SYNTHESIZE(std::string, _head_2, Head_2);// 头像
    CC_SYNTHESIZE(int, _figure_2, Figure_2);// 显示的是那个人
    CC_SYNTHESIZE(std::string, _zishi_2, Zishi_2);// 姿势
    
    //<><><>所有选择数据保存在这个数组里<><><>//
    CC_SYNTHESIZE_RETAIN(CCArray*, _array, Array);
    
    //<><><>第一个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array1, Array1);// 说的话
    CC_SYNTHESIZE(int, _attribute_1, Attribute_1);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_1, AttributeValue_1);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array1next, Array1next);// 选择后下一个显示的文本的id
    
    //<><><>第二个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array2, Array2);// 说的话
    CC_SYNTHESIZE(int, _attribute_2, Attribute_2);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_2, AttributeValue_2);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array2next, Array2next);// 选择后下一个显示的文本的id
    
    //<><><>第三个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array3, Array3);// 说的话
    CC_SYNTHESIZE(int, _attribute_3, Attribute_3);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_3, AttributeValue_3);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array3next, Array3next);// 选择后下一个显示的文本的id
    
    CC_SYNTHESIZE(int, _animation, Animation);// 0(先出左边,再出右边. 先退左边,再退右边.)
    CC_SYNTHESIZE(std::string, _achievement, Achievement);// 此对话会获得的成就
    CC_SYNTHESIZE(int, _next, Next);// 下一个要显示的文本的id
    
    //
    CC_SYNTHESIZE(int, _cluesPetID, CluesPetID);
    CC_SYNTHESIZE_RETAIN(CCArray*, _cluesArr, CluesArr);
    CC_SYNTHESIZE(int, _cluesHead, CluesHead);
    
protected:
};

#endif /* defined(__manman_client__VipDialogItem__) */
