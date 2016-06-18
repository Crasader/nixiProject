//
//  MMMachining.cpp
//  manman_client
//
//  Created by stevenLi on 14-4-25.
//
//

#include "MMMachining.h"
#include "apptools/HelperFunc.h"

static MMMachining * pRet = NULL;

MMMachining * MMMachining::getInstance()
{
    if ( !pRet )
    {
        pRet = new MMMachining();
        pRet->init();
        pRet->retain();
    }
    return pRet;
}

void MMMachining::init()
{
    string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("testfile");
    
    unsigned long bufferSize = 0;
    
//    char* pBuffer = (char* )CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &bufferSize);
    char* pBuffer = (char*)CZHelperFunc::getFileData(fullPath.c_str(), "r", &bufferSize);
    string content( pBuffer, bufferSize);
    replace_all(content, "\r\n", "\n");
	split_string(m_vecContainer, content, '\n');
    
    if (pBuffer)
    {
        delete [] pBuffer;
        pBuffer = NULL;
    }
}

int MMMachining::replace_all(std::string& str,const std::string& pattern,const std::string& newpat)
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

void MMMachining::split_string(std::vector<string>& vec,const std::string& src, char sp)
{
    vec.clear();
    size_t index = 0;
    size_t pos = 0;
    while((pos = src.find(sp, index)) != string::npos)
    {
        vec.push_back(src.substr(index, pos-index));
        index = pos+1;
    }
    
    vec.push_back(src.substr(index));
}

bool MMMachining::IsForbid( const char * name )
{
    
    bool res = compareWithForbid( name );
    
    if (res == false) {
        CCLOG(" Found it.！-----");
        return false;
    }
    
    return true;
}

bool MMMachining::compareWithForbid( const char * name)
{
    for (int i = 0; i < m_vecContainer.size(); ++i)
    {
        string str(m_vecContainer[i]);
        char * res =  strstr(name, str.c_str()) ;
        
        if ( res != NULL && m_vecContainer[i] != "") {
            CCLOG(" Found it.！");
            return false;       // 找到了
        }
    }
    return true;
}




