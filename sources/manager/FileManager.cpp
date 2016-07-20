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

int replace_all(std::string& str,const std::string& pattern,const std::string& newpat)
{
    int count = 0;
    const size_t nsize = newpat.size();
    const size_t psize = pattern.size();
    
    for(size_t pos = str.find(pattern, 0);
        pos != std::string::npos;
        pos = str.find(pattern,pos + nsize))
    {
        str.replace(pos, psize, newpat);
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

FileManager::~FileManager() {
    
}

FileManager* FileManager::Inst() {
    if (_instance == nullptr) {
        _instance = new FileManager();
    }
    
    return _instance;
}

vector<string> FileManager::fetch_file_contents(const char *filePath, const char *splitChar) {
    vector<string> rtn;
    string fileFullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath);
    unsigned long bufferSize = 0;
    //    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(fileFullPath.c_str(), "r", &bufferSize);
    char* pBuffer = (char*)CZHelperFunc::getFileData(fileFullPath.c_str(), "r", &bufferSize);
    
    string buf(pBuffer, bufferSize);
    ReplaceStringWithGivenSubstring(buf, splitChar, "\n");
    SplitString(rtn, buf, '\n');
    
    if (pBuffer) {
        delete []pBuffer;
        pBuffer = NULL;
    }
    
    return rtn;
}

//bool FileManager::config_with_file(const char* fileName, const char splitChar)
//{
//    
//    string fileFullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName);
//    unsigned long bufferSize = 0;
////    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData(fileFullPath.c_str(), "r", &bufferSize);
//    char* pBuffer = (char*)CZHelperFunc::getFileData(fileFullPath.c_str(), "r", &bufferSize);
//    
//    string buf(pBuffer, bufferSize);
//    ReplaceStringWithGivenSubstring(buf, "\r\n", "\n");
//    SplitString(_container, buf, '\n');
//    
//    if (pBuffer) {
//        delete []pBuffer;
//        pBuffer = NULL;
//    }
//    
//    return true;
//}

const vector<string> FileManager::illegalWrods() {
    if (_illegalWords.empty()) {
        _illegalWords = this->fetch_file_contents("conf/learn2", "、");
    }
    
    long size = _illegalWords.size();
    for (int i = 0; i < size; i++) {
        CCLOG("%d -- %s", i, _illegalWords[i].c_str());
    }
    
    return _illegalWords;
}

bool FileManager::is_illegal(const char* name) {
    vector<string>cotents = this->illegalWrods();
    return this->whether_contain_string(cotents, name);
}

bool FileManager::whether_contain_string(vector<string>& contents, const char* str)
{
    unsigned long containerSize = contents.size();
    for (int i = 0; i < containerSize; ++i)
    {
        string item = contents.at(i);
        if (! item.empty()) {
            char* res = strstr(str, item.c_str()) ;
            if (res != NULL) {
                CCLOG("Found the str=%s in %s", str, item.c_str());
                return true;       // 找到了
            }
        }
        
    }
    
    return false;
}

string FileManager::rand_item(vector<string>& contents) {
    unsigned long count = contents.size();
    unsigned long choosed = CCRANDOM_0_1() * count;
    CCLOG("FileManager::randItem() -- choosed : %ld", choosed);
    return contents.at(choosed);
}

