//
//  JNIController.h
//  tiegao
//
//  Created by mac on 16-9-8.
//
//

#ifndef __tiegao__JNIController__
#define __tiegao__JNIController__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"
#endif

#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/Tiegao"
#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/AlarmClock"
#define  SDK_TALKINGDATA_CLASS "cn/mzplay/tiegao/TalkingDataLayer"

extern "C"
{
    
    class JNIController {
        
    public:
    	
    	static void push_Android(string nameStr, int num, int index);
    	static void closePush_Android(int index);
        
        // 分享
        static void showShare();
        static void setShareImage(char const* str);
        
    };
}
#endif /* defined(__tiegao__JNIController__) */
