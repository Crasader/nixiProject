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
public: // Export
    static FileManager* Inst();
    vector<string> fetch_file_contents(const char* filePath, const char* splitChar="\r\n");
    bool is_illegal(const char* toCheck);
    bool whether_contain_string(vector<string>& contents, const char* str);
    void replace_all_illegal(string& toCheck, const char* replace);
    
public:
    ~FileManager();
    string rand_item(vector<string>& contents);
    
private:
    const vector<string> illegalWrods();
    
private:
    vector<string>          _illegalWords;
};

#endif /* defined(__mm3c__FileTool__) */
