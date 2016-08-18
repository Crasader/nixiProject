//
//  DialogItem.cpp
//  manman_client
//
//  Created by lakkey on 14-1-13.
//
//

#include "DialogItem.h"
#include "DataManager.h"


DialogItem::~DialogItem()
{
    CC_SAFE_RELEASE_NULL(_array);
    CC_SAFE_RELEASE_NULL(_cluesArr);
}


bool DialogItem::init()
{
    return true;
}


void DialogItem::config_static_with_json(CSJson::Value value)
{
    // 当前场景的id
    _index = ccs(value["id"].asString())->intValue();
    // 0没有人 1一个人 2有两个人 3没有变化 4只播放背景
    _states = ccs(value["states"].asString())->intValue();
    // 是否有2个人 0没有 1有
    _passersby = ccs(value["passerby"].asString())->intValue();
    
    // 显示的名字
    _name = value["name"].asString();
    if (_name == "public") {
        _name = "";
    }else if (_name == "player"){
        CCString* string = CCString::createWithFormat("%s", "云诗彤");
        _name = string->getCString();
    }else{
        std::string name = value["name"].asString();
        CCString* str = CCString::createWithFormat("%s", name.c_str());
        _name = str->getCString();
    }
    
    // 显示要眨眼的id
    _nameId = value["nameID"].asString();
    
    // 说的话
    _said = value["said"].asString();
    // 显示的背景
    _bg = value["bg"].asString();
    // 1正常 2近景
    _bust = ccs(value["bust"].asString())->intValue();
    
    //<><><>主要显示的人<><><>//
    // 头像
	_head_1 = value["head_1"].asString();
    // 显示的是那个人
    _figure_1 = ccs(value["figure_1"].asString())->intValue();
    // 姿势
    _zishi_1 = value["zishi_1"].asString();
    // 表情
    _biaoqing_1 = ccs(value["biaoqing_1"].asString())->intValue();
    
    //<><><>第二个人要显示的人<><><>//
    // 头像
	_head_2 = value["head_2"].asString();
    // 显示的是那个人
    _figure_2 = ccs(value["figure_2"].asString())->intValue();
    // 姿势
    _zishi_2 = value["zishi_2"].asString();
    // 表情
    _biaoqing_2 = ccs(value["biaoqing_2"].asString())->intValue();
    
    //<><><>第一个选择对话内容<><><>//
    // 说的话
    _array1 = value["array_1"].asString();
    // 对应的属性
    _attribute_1 = ccs(value["attribute_1"].asString())->intValue();
    // 对应属性的数值(如果对应属性的数值没有达到要求，次选项不能点击)
    _attributeValue_1 = ccs(value["value_1"].asString())->intValue();
    // 选择后下一个显示的文本的id
    _array1next = ccs(value["array_next_1"].asString())->intValue();
    
    //<><><>第二个选择对话内容<><><>//
    // 说的话
    _array2 = value["array_2"].asString();
    // 对应的属性
    _attribute_2 = ccs(value["attribute_2"].asString())->intValue();
    // 对应属性的数值(如果对应属性的数值没有达到要求，次选项不能点击)
    _attributeValue_2 = ccs(value["value_2"].asString())->intValue();
    // 选择后下一个显示的文本的id
    _array2next = ccs(value["array_next_2"].asString())->intValue();
    
    //<><><>第三个选择对话内容<><><>//
    // 说的话
    _array3 = value["array_3"].asString();
    // 对应的属性
    _attribute_3 = ccs(value["attribute_3"].asString())->intValue();
    // 对应属性的数值(如果对应属性的数值没有达到要求，次选项不能点击)
    _attributeValue_3 = ccs(value["value_3"].asString())->intValue();
    // 选择后下一个显示的文本的id
    _array3next = ccs(value["array_next_3"].asString())->intValue();
    
    if (_array1.length() == 0) {
        _next = ccs(value["next"].asString())->intValue();
    }else{
        this->setArray(CCArray::create());
        if (_array1.length() != 0) {
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCString::create(_array1), "dialog");
            dic->setObject(CCString::createWithFormat("%d", _attribute_1), "attribute");
            dic->setObject(CCString::createWithFormat("%d", _attributeValue_1), "attributeValue");
            dic->setObject(CCString::createWithFormat("%d", _array1next), "next");
            _array->addObject(dic);
        }
        
        if (_array2.length() != 0) {
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCString::create(_array2), "dialog");
            dic->setObject(CCString::createWithFormat("%d", _attribute_2), "attribute");
            dic->setObject(CCString::createWithFormat("%d", _attributeValue_2), "attributeValue");
            dic->setObject(CCString::createWithFormat("%d", _array2next), "next");
            _array->addObject(dic);
        }
        
        if (_array3.length() != 0) {
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCString::create(_array3), "dialog");
            dic->setObject(CCString::createWithFormat("%d", _attribute_3), "attribute");
            dic->setObject(CCString::createWithFormat("%d", _attributeValue_3), "attributeValue");
            dic->setObject(CCString::createWithFormat("%d", _array3next), "next");
            _array->addObject(dic);
        }
    }
    
    // 默认为0。 0(先出左边,再出右边. 先退左边,再退右边.)
    _animation = ccs(value["animation"].asString())->intValue();
    // 此对话会获得的成就
    _achievement = value["achievement"].asString();
    
}


void DialogItem::clues_config_static_with_json(CSJson::Value value){
    
    _cluesPetID = value["petid"].asInt();
//    CCLog("_cluesPetID ************** %d",_cluesPetID);
    
    CSJson::Value array = value["array"];
    if (array.isArray()) {
        this->setCluesArr(CCArray::create());
        int count = array.size();
//        CCLog("count ==== %d", count);
        for (int i = 0; i < count; i++) {
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCString::create(array[i]["dialog"].asString()), "dialog");
            dic->setObject(CCString::createWithFormat("%d",array[i]["head"].asInt()), "head");
            _cluesArr->addObject(dic);
        }
    }
}








