//
//  AppUtil.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef AppUtil_hpp
#define AppUtil_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;

typedef enum {
    e_operate_pause = 0,
    e_operate_resume,
    e_operate_stop,
} NodeOperateType;


class AppUtil : public CCObject
{
public:
    static void setNewObj(CCObject* old, CCObject* anew);
    
    // 读取文件
    static char* read_file(const char* filename);
    static CSJson::Value read_json_file(const char* filename);
    
    // 从Json解析出2dx对象
    static CCArray* array_with_json(CSJson::Value root);
    static CCDictionary* dictionary_with_json(CSJson::Value root);
    
    // array排序
    static void sort_string_array(CCArray* arr);
    
    // 动画及显示
    static CCSprite* get_self_sprite(const char* file); // 返回一个中心位置的精灵
    static CCAnimation* animationWithFrame(const char* frameFormat, int frameCount, int startNum, float interval);
    static CCAnimation* animationWithPics(const char* picFormat, int picCount, int startNum, float interval);
    /* 返回一个扩张渐隐的Action
     经验值：duration 不要小于1.f， interval 在0.5f以内 */
    static CCActionInterval* action_expand_fade_out(float duration = 1.6f, float interval = 0.3f, float scale_range = 1.33f);
    
    // 保存屏幕为图片
    static cocos2d::CCRenderTexture* saveScreenAsRenderTexture();
    
    // 暂停和恢复节点及其子节点的所有动画和动作
    static void operate_all_scheduel_and_actions(CCNode* node, NodeOperateType type);
    
    // 字符串操作
    static void split(std::string src, const char* token, vector<string>& vect);
    
protected:
    
private:
    static char* read_xxtea_file(const char* filename);
    static CCObject* object_with_json(CSJson::Value root);
};

#endif /* AppUtil_hpp */
