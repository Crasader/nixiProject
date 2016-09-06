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
    
    CC_SYNTHESIZE(int, _index, VipIndex);// 当前场景的id
    CC_SYNTHESIZE(std::string, _name, VipName);// 显示的名字
    CC_SYNTHESIZE(std::string, _nameId, VipNameId);// 显示要眨眼的id
    CC_SYNTHESIZE(std::string, _said, VipSaid);// 说的话
    CC_SYNTHESIZE(int, _states, VipStates);// 0没有人 1一个人 2有两个人 3没有变化 4只播放背景
    CC_SYNTHESIZE(int, _passersby, VipPassersby);// 是否有2个人 0没有 1有
    CC_SYNTHESIZE(std::string, _bg, VipBg);// 显示的背景
    CC_SYNTHESIZE(int, _bust, VipBust);// 1正常 2近景
    
    //<><><>主要显示的人<><><>//
    CC_SYNTHESIZE(std::string, _head_1, VipHead_1);// 头像
    CC_SYNTHESIZE(int, _figure_1, VipFigure_1);// 显示的是那个人
    CC_SYNTHESIZE(std::string, _zishi_1, VipZishi_1);// 姿势
    
    //<><><>第二个要显示的人<><><>//
    CC_SYNTHESIZE(std::string, _head_2, VipHead_2);// 头像
    CC_SYNTHESIZE(int, _figure_2, VipFigure_2);// 显示的是那个人
    CC_SYNTHESIZE(std::string, _zishi_2, VipZishi_2);// 姿势
    
    //<><><>所有选择数据保存在这个数组里<><><>//
    CC_SYNTHESIZE_RETAIN(CCArray*, _array, VipArray);
    
    //<><><>第一个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array1, VipArray1);// 说的话
    CC_SYNTHESIZE(int, _attribute_1, VipAttribute_1);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_1, VipAttributeValue_1);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array1next, VipArray1next);// 选择后下一个显示的文本的id
    
    //<><><>第二个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array2, VipArray2);// 说的话
    CC_SYNTHESIZE(int, _attribute_2, VipAttribute_2);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_2, VipAttributeValue_2);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array2next, VipArray2next);// 选择后下一个显示的文本的id
    
    //<><><>第三个选择对话内容<><><>//
    CC_SYNTHESIZE(std::string, _array3, VipArray3);// 说的话
    CC_SYNTHESIZE(int, _attribute_3, VipAttribute_3);// 对应的属性
    CC_SYNTHESIZE(int, _attributeValue_3, VipAttributeValue_3);// 对应属性的数值(数值没有达到要求，此选项不能点击)
    CC_SYNTHESIZE(int, _array3next, VipArray3next);// 选择后下一个显示的文本的id
    
    CC_SYNTHESIZE(int, _animation, VipAnimation);// 0(先出左边,再出右边. 先退左边,再退右边.)
    CC_SYNTHESIZE(std::string, _achievement, VipAchievement);// 此对话会获得的成就
    CC_SYNTHESIZE(int, _next, VipNext);// 下一个要显示的文本的id
    
    //
    CC_SYNTHESIZE(int, _cluesPetID, VipCluesPetID);
    CC_SYNTHESIZE_RETAIN(CCArray*, _cluesArr, VipCluesArr);
    CC_SYNTHESIZE(int, _cluesHead, VipCluesHead);
    
protected:
};

#endif /* defined(__manman_client__VipDialogItem__) */
