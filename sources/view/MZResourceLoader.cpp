//
//  MZResourceLoader.cpp
//  tiegao
//
//  Created by mac on 16-6-1.
//
//

#include "MZResourceLoader.h"
#include "apptools/HelperFunc.h"


static MZResourceLoader* _shared_resource_loader = NULL;

MZResourceLoader::~MZResourceLoader()
{
    if (_shared_resource_loader) {
        this->purge();
    }
}


MZResourceLoader* MZResourceLoader::get_instance()
{
    if (!_shared_resource_loader) {
        _shared_resource_loader = new MZResourceLoader();
//        _shared_resource_loader->config();
    }
    
    return _shared_resource_loader;
}


void MZResourceLoader::purge()
{
    CC_SAFE_RELEASE_NULL(_shared_resource_loader);
}


#define PATH_PIC        "pic"
#define PATH_PLIST      "plist"
void MZResourceLoader::config()
{
//    CCFileUtils* file_utils = CCFileUtils::sharedFileUtils();
//    std::vector<std::string> order;
//    std::string write_path = file_utils->getWritablePath();
//    CCString* str_pic = CCString::createWithFormat("%s%s", write_path.c_str(), PATH_PIC);
//    CCString* str_plist = CCString::createWithFormat("%s%s", write_path.c_str(), PATH_PLIST);
//    order.push_back(str_pic->getCString());
//    order.push_back(str_plist->getCString());
//    file_utils->setSearchPaths(order);
}


char* MZResourceLoader::reader_file(const char* file)
{
    CCLOG("MZResourceLoader::reader_file:: %s", file);
    CCFileUtils* file_utils = CCFileUtils::sharedFileUtils();
    
    unsigned long file_size = 0;
    char* file_data = (char*)file_utils->getFileData(file, "rb", &file_size);
    
    if (file_data[file_size] != '\0') {
        file_data[file_size] = '\0';
    }
    
//    CCLOG("there are %lu char in file ....", file_size);
//    CCLOG("file data :::: %s", file_data);
    return file_data;
}


char* MZResourceLoader::reader_xxtea_file(const char* file)
{
    CCLOG("file:: %s", file);
    unsigned long file_size = 0;
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
    char* result = (char*)CZHelperFunc::getFileData(fullPath.c_str(), "rb", &file_size);
//    CCLOG("there are %d char in file ....", resultLength);
//    CCLOG("file data :::: %s", file_data);
    return result;
}


CSJson::Value MZResourceLoader::get_json_data_with_file(const char *file)
{
    char* file_data = this->reader_file(file);
//    char* file_data = this->reader_xxtea_file(file);
    
    CSJson::Reader reader;
    CSJson::Value root;
    
    if (!reader.parse(file_data, root, false)) {
        CCLOG("CSJson reader error~~~");
        return CSJson::nullValue;
    }
    return root;
}


CCObject* MZResourceLoader::object_with_json(CSJson::Value root)
{
    if (root.isObject()) { // 对象
        CCDictionary* dic = CCDictionary::create();
        std::vector<std::string> keys = root.getMemberNames();
        for (std::vector<std::string>::iterator iterator = keys.begin(); iterator != keys.end(); iterator++) {
            std::string key = (std::string)* iterator;
            //            CCLOG("std::string key == %s", key.c_str());
            CSJson::Value value = root.get(key, value.jsonNull);
            if (value != value.jsonNull) {
                CCObject* pObj = this->object_with_json(value);
                if (pObj) {
                    dic->setObject(pObj, key);
                }
            }
        }
        
        return dic;
    }
    else if (root.isArray()) { // 数组
        CCArray* array = CCArray::create();
        for (CSJson::ValueIterator iterator = root.begin(); iterator != root.end(); iterator++) {
            CSJson::Value value = (CSJson::Value)* iterator;
            CCObject* pObj = this->object_with_json(value);
            if (pObj) {
                array->addObject(pObj);
            }
        }
        
        return array;
    }
    else if (root.isString()) {
        CCString* str = CCString::create(root.asString());
        return str;
    }
    else if (root.isBool()) {
        CCBool* value = CCBool::create(root.asBool());
        return value;
    }
    else if (root.isInt() || root.isUInt()) {
        CCInteger* value = CCInteger::create(root.asInt());
        return value;
    }
    else if (root.isDouble()) {
        CCDouble* value = CCDouble::create(root.asDouble());
        return value;
    }
    else {
        CCLOG("There is a NULL ... in MZResourceLoader::object_with_json(CSJson::Value)");
        return NULL;
    }
}


CCArray* MZResourceLoader::array_with_json(CSJson::Value root)
{
    return (CCArray*)this->object_with_json(root);
}


CCDictionary* MZResourceLoader::dictionary_with_json(CSJson::Value root)
{
    return (CCDictionary*)this->object_with_json(root);
}