//
//  FileManager.cpp
//  mm3c
//
//  Created by lakkey on 15-10-11.
//
//

#include "FileManager.h"
#include "apptools/HelperFunc.h"

int ReplaceStringWithGivenSubstring(string& src, const string& substring, const string& newSubstring)
{
    int count = 0;
    const size_t nsize = newSubstring.size();
    const size_t psize = substring.size();
    
    for(size_t pos = src.find(substring, 0);
        pos != std::string::npos;
        pos = src.find(substring, pos + nsize))
    {
        src.replace(pos, psize, newSubstring);
        count++;
    }
    
    return count;
}

void SplitString(vector<string>& output, const string& input, char separator)
{
    output.clear();
    
    size_t index = 0;
    size_t pos = 0;
    
    while((pos = input.find(separator, index)) != string::npos)
    {
        output.push_back(input.substr(index, pos - index));
        index = pos + 1;
    }
    
    output.push_back(input.substr(index));
}

// -------------------------------------------------

static FileManager* _instance = nullptr;

const int CONNECT_TIMEOUT = 60;

FileManager::~FileManager() {
    
}

FileManager* FileManager::Inst() {
    if (_instance == nullptr) {
        _instance = new FileManager();
        _instance->config_with_file("res/config/learn");
    }
    
    return _instance;
}

bool FileManager::config_with_file(const char* fileName)
{
    string fileFullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
    unsigned long bufferSize = 0;
//    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(fileFullPath.c_str(), "r", &bufferSize);
    char* pBuffer = (char*)CZHelperFunc::getFileData(fileFullPath.c_str(), "r", &bufferSize);
    
    string buf(pBuffer, bufferSize);
    ReplaceStringWithGivenSubstring(buf, "\r\n", "\n");
    SplitString(_container, buf, '\n');
    
    if (pBuffer) {
        delete []pBuffer;
        pBuffer = NULL;
    }
    
    return true;
}

bool FileManager::IsForbid(const char* name)
{
    bool res = searchString(name);
    
    CCLOG("res");
    if (res == false) {
        CCLOG(" Found it.！-----");
        return false;
    }
    
    return true;
}

bool FileManager::searchString(const char* str)
{
    unsigned long containerSize = this->itemCount();
    for (int i = 0; i < containerSize; ++i)
    {
        string item = _container.at(i);
        char* res = strstr(str, item.c_str()) ;
        
        if (!item.empty() && res != NULL) {
            CCLOG("Found the result=%s", str);
            return true;       // 找到了
        }
    }
    
    return false;
}

unsigned long FileManager::itemCount() {
    if (_container.empty()) {
        return 0;
    }
    else {
        return _container.size();
    }
}

string FileManager::randItem() {
    unsigned long count = this->itemCount();
    unsigned long choosed = CCRANDOM_0_1() * count;
    CCLOG("FileManager::randItem() -- choosed : %ld", choosed);
    return _container.at(choosed);
}


