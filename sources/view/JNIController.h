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

/**  美图  **/
#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/meitu/Tiegao"
#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/meitu/AlarmClock"

/**  百度  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/baidu/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/baidu/AlarmClock"

/**  360  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/qihu360/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/qihu360/AlarmClock"

/**  白包  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/AlarmClock"

extern "C"
{
    
    class JNIController {
        
    public:
    	
    	static void push_Android(string nameStr, int num, int index);
    	static void closePush_Android(int index);
        
        // 登陆
        static void isLanding(int index);
        
        // uuid
        static std::string getOpenId();
        
        // 退出
        static void exitGame(int index);
        
        // 分享
        static void showShare();
        static void setShareImage(char const* str);
        static int getShareStatus();
        static void setShareStatus(int shareStatus);
        static void shareText();
        
        // 网络状态
        static bool getNetworkAvailable();
        
        static std::string getSessionid();
        static std::string getCpOrderId();
        static void setUserId(char const* str);
        // money
        static int getMoneyStatus();
        static void setMoneyStatus(int status);
        // gold
        static int getGoldStatus();
        static void setGoldStatus(int status);
        // name
        static void setPlayerName(char const* str);
        static void setPlayerLevel(char const* str);
        static void setPlayerGold(char const* str);
        
        static int getRestartApplication();
        static void setRestartApplication(int status);
        
        static int  getLandStatus();
        static void setLandStatus(int smsStatus);
        
        // 支付
        static void isGamePay(int smsIndex);
        static int  getSmsStatus();
        static void setSmsStatus(int status);
        static void setProductId(char const*  str);
        static std::string getProductId();
        static void setSidId(char const*  str);
        
        static void isExtendData();
        static void setData(int index);
        
    };
}
#endif /* defined(__tiegao__JNIController__) */
