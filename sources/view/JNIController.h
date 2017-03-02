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
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/meitu/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/meitu/AlarmClock"

/**  百度  **/
//#define  SDK_TIEGAO_CLASS "com/mzplay/tiegao/baidu/Tiegao"
//#define  SDK_CLOCK_CLASS "com/mzplay/tiegao/baidu/AlarmClock"

/**  360  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/qihu360/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/qihu360/AlarmClock"

/**  小米  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/mi/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/mi/AlarmClock"

/**  新浪  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/wyx/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/wyx/AlarmClock"

/**  vivo  **/
//#define  SDK_TIEGAO_CLASS "com/mzplay/tiegao/vivo/Tiegao"
//#define  SDK_CLOCK_CLASS "com/mzplay/tiegao/vivo/AlarmClock"

/**  金山云  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/ksyun/mi/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/ksyun/mi/AlarmClock"

/**  应用宝  **/
//#define  SDK_TIEGAO_CLASS "com/tencent/tmgp/mzplay/tiegao/Tiegao"
//#define  SDK_CLOCK_CLASS "com/tencent/tmgp/mzplay/tiegao/AlarmClock"

/**  UC  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/uc/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/uc/AlarmClock"

/**  华为  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/huawei/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/huawei/AlarmClock"

/**  oppo  **/
//#define  SDK_TIEGAO_CLASS "com/mzplay/tiegao/nearme/gamecenter/Tiegao"
//#define  SDK_CLOCK_CLASS "com/mzplay/tiegao/nearme/gamecenter/AlarmClock"

/**  乐视  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/leshi/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/leshi/AlarmClock"

/**  魅族  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/mz/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/mz/AlarmClock"

/**  爱奇艺  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/PPS/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/PPS/AlarmClock"

/**  东东  **/
//#define  SDK_TIEGAO_CLASS "com/iddgame/mzplay/tiegao/Tiegao"
//#define  SDK_CLOCK_CLASS "com/iddgame/mzplay/tiegao/AlarmClock"

/**  当乐  **/
//#define  SDK_TIEGAO_CLASS "com/mzplay/tiegao/downjoy/Tiegao"
//#define  SDK_CLOCK_CLASS "com/mzplay/tiegao/downjoy/AlarmClock"




/**  单独支付Taptap  **/
//#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/Tiegao"
//#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/AlarmClock"



/**  快看  **/
#define  SDK_TIEGAO_CLASS "cn/mzplay/tiegao/kuaikan/Tiegao"
#define  SDK_CLOCK_CLASS "cn/mzplay/tiegao/kuaikan/AlarmClock"


/**  白包 鱼丸母包 **/
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
        // 注销
        static void isUnLanding();
        
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
        static void setChannelId(int channelId);
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
