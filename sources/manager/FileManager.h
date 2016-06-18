//
//  FileTool.h
//  mm3c
//
//  Created by lakkey on 15-10-11.
//
//

#ifndef __mm3c__FileTool__
#define __mm3c__FileTool__

#include <vector>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define FILEM    FileManager::Inst()

class FileManager : public CCObject
{
public:
    ~FileManager();
    static FileManager* Inst();
    
    bool searchString(const char* str);
    unsigned long itemCount();
    string randItem();
    
private:
    bool IsForbid( const char* name );
    bool config_with_file(const char* fileName);
    
private:
    vector<string>          _container;
};

#endif /* defined(__mm3c__FileTool__) */
