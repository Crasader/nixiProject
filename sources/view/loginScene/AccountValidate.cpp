//
//  AccountValidate.cpp
//  mm3c
//
//  Created by lakkey on 14-12-25.
//
//

#include "AccountValidate.h"

enum {
    State_CheckLiteral_Legal = 100,
    State_CheckLiteral_Empty = 200,
    State_CheckLiteral_Min = 300,
    State_CheckLiteral_Max = 400,
    State_CheckLiteral_SPECIAL = 500,
    State_CheckLiteral_Inconsistence = 600,
};


enum {
    Error_Type_Usr = 111,
    Error_Type_Pwd = 222,
};

AccountValidate::~AccountValidate()
{
}

AccountValidate::AccountValidate()
{
}

bool AccountValidate::check_legal_number(const char* text)
{
    unsigned long length = strlen(text);
    for (int i = 0; i < length; ++i) {
        if (('0' <= text[i] && text[i] <= '9')) {
            continue;
        }
        else {
            return false;
        }
    }
    
    return true;
}

bool AccountValidate::check_legal_text(const char* text)
{
    unsigned long length = strlen(text);
    for (int i = 0; i < length; ++i) {
        if (('0' <= text[i] && text[i] <= '9') ||
            ('A' <= text[i] && text[i] <= 'Z') ||
            ('a' <= text[i] && text[i] <= 'z')) {
            continue;
        }
        else {
            return false;
        }
    }
    
    return true;
}

bool AccountValidate::check_legal_char(const char* text) {
    unsigned long length = strlen(text);
    for (int i = 0; i < length; ++i) {
        if (('@' == text[i]) ||
            ('.' == text[i])) {
            continue;
        }
        else {
            return false;
        }
    }
    
    return true;
}

int AccountValidate::checkLiteralLegal(std::string str, int minLimmit, int maxLimmit)
{
    if (str.empty() || str.compare("") == 0)  return State_CheckLiteral_Empty;
    
    unsigned long length = str.length();
    if (length < minLimmit)  return State_CheckLiteral_Min;
    if (length > maxLimmit)  return State_CheckLiteral_Max;
    
    const char* pChar = str.c_str();
    for (int i = 0; i < length; ++i) {
        if (('0' <= pChar[i] && pChar[i] <= '9') || ('A' <= pChar[i] && pChar[i] <= 'Z') || ('a' <= pChar[i] && pChar[i] <= 'z')) {
            
        }
        else
            return State_CheckLiteral_SPECIAL;
    }
    
    return State_CheckLiteral_Legal;
}


//void AccountValidate::showErrorMessage(int state, int type)
//{
//    std::string msg;
//    if (type == Error_Type_Usr) {
//        switch (state) {
//            case State_CheckLiteral_Empty: {
//                msg = "账号密码不能为空";
//            } break;
//                
//            case State_CheckLiteral_SPECIAL: {
//                msg = "账号密码不能使用特殊字符及汉字";
//            } break;
//                
//            case State_CheckLiteral_Min: {
//                msg = "账号为6－16位英文数字";
//            } break;
//                
//            case State_CheckLiteral_Max: {
//                msg = "账号位数过长";
//            } break;
//                
//            default:
//                break;
//        }
//    }
//    else if (type == Error_Type_Pwd) {
//        switch (state) {
//            case State_CheckLiteral_Empty: {
//                msg = "账号密码不能为空";
//            } break;
//                
//            case State_CheckLiteral_SPECIAL: {
//                msg = "账号密码不能使用特殊字符及汉字";
//            } break;
//                
//            case State_CheckLiteral_Min: {
//                msg = "密码为6－16位英文数字";
//            } break;
//                
//            case State_CheckLiteral_Max: {
//                msg = "密码位数过长";
//            } break;
//                
//            case State_CheckLiteral_Inconsistence: {
//                msg = "两次输入的密码不一致";
//            } break;
//                
//            default:
//                break;
//        }
//    }
//    
//    if (!msg.empty()) {
//        if (MMAudioManager::get_instance()->is_effect_on()) {
//            MMAudioManager::get_instance()->play_effect(kAudio_audio_AHMessageBox, false);
//        }
//        
//        AHMessageBox* mb = AHMessageBox::create_with_message(msg.c_str(), NULL, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YES, false);
//        mb->setPosition(Center);
//        CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3900);
//    }
//}
