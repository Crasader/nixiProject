//
//  StringUtil.h
//  tiegao
//
//  Created by mac on 16-8-17.
//
//

#ifndef __tiegao__StringUtil__
#define __tiegao__StringUtil__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class StringUtil : public CCObject {
public:
    static StringUtil* sharedStrUtil();
    
    bool init();
    
    /** 用分隔符分割字符串，结果存放到一个列表中，列表中的对象为CCString */
    CCArray* split(const char* srcStr, const char* sSep);
    
private:
    static StringUtil* mStringUtil;
};

#endif /* defined(__tiegao__StringUtil__) */
