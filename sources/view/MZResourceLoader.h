//
//  MZResourceLoader.h
//  tiegao
//
//  Created by mac on 16-6-1.
//
//

#ifndef __tiegao__MZResourceLoader__
#define __tiegao__MZResourceLoader__

#include <iostream>
#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;


class MZResourceLoader : public CCObject
{
public:
    ~MZResourceLoader();
    static MZResourceLoader* get_instance();
    void config();
    void purge();
    
    // 从Json解析出2dx对象
    CCObject* object_with_json(CSJson::Value root);
    CCArray* array_with_json(CSJson::Value root);
    CCDictionary* dictionary_with_json(CSJson::Value root);
    
    // 读取图片资源
    
    // 从图片集中读取图片资源
    
    // 读取文件
    char* reader_file(const char* file);
    char* reader_xxtea_file(const char* file);
    
    CSJson::Value get_json_data_with_file(const char *file);
    //    CSJson::Value get_json_data_with_lua(const char* file);
    
    // 写入文件
    
    //    void lua_test();
    
protected:
    
};

#endif /* defined(__tiegao__MZResourceLoader__) */
