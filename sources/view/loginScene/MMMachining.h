//
//  MMMachining.h
//  manman_client
//
//  Created by stevenLi on 14-4-25.
//
//

#ifndef __manman_client__MMMachining__
#define __manman_client__MMMachining__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using namespace std;

class MMMachining : public CCObject
{
    void init();
    vector<string> m_vecContainer;
    int     replace_all(std::string& str,const std::string& pattern,const std::string& newpat);
    void    split_string(std::vector<string>& vec,const std::string& src, char sp);
    bool    compareWithForbid(const char * name);
public:
    static MMMachining * getInstance();
    bool IsForbid( const char* name );
    
};

#endif /* defined(__manman_client__MMMachining__) */
