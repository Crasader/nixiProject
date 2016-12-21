//
//  JNIController.cpp
//  tiegao
//
//  Created by mac on 16-9-8.
//
//

#include "JNIController.h"


extern "C"
{
    void JNIController::push_Android(string nameStr, int num, int index){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_CLOCK_CLASS, "push_Android", "(Ljava/lang/String;II)V")){
    		return;
    	}
    	jstring strArg = jni_methodInfo.env->NewStringUTF(nameStr.c_str());
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,strArg,num,index);
    	jni_methodInfo.env->DeleteLocalRef(strArg);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
	}
    void JNIController::closePush_Android(int index){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_CLOCK_CLASS, "closePush_Android", "(I)V")) {
            return;
        }
        jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,index);
        jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    // 登陆
    void JNIController::isLanding(int index){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo jni_methodInfo;
    if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "isLanding", "(I)V")){
        return;
    }
    jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, index);
    jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    std::string JNIController::getOpenId(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"getOpenId","()Ljava/lang/String;");
        if(ret)
        {
            jstr = (jstring)jni_methodInfo.env->CallStaticObjectMethod(jni_methodInfo.classID,jni_methodInfo.methodID);
            return JniHelper::jstring2string(jstr);
        }
#endif
    }
    
    // 退出
    void JNIController::exitGame(int index){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "exitGame", "(I)V")){
            return;
        }
        jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, index);
        jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }

    // 分享
    void JNIController::showShare(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "showShare", "()V") ){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID,  jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    void JNIController::setShareImage(char const* str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setShareImage","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    int  JNIController::getShareStatus(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getShareStatus", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setShareStatus(int shareStatus){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setShareStatus", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,shareStatus);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    void JNIController::shareText(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "shareText", "()V") ){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID,  jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    
    
    // 网络状态
    bool JNIController::getNetworkAvailable() {
        bool ret = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getNetworkAvailable", "()I") ){
            return ret;
        }
        ret = jni_methodInfo.env->CallStaticBooleanMethod(jni_methodInfo.classID,  jni_methodInfo.methodID);
        jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    
    std::string JNIController::getSessionid(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"getSessionid","()Ljava/lang/String;");
        if(ret)
        {
            jstr = (jstring)jni_methodInfo.env->CallStaticObjectMethod(jni_methodInfo.classID,jni_methodInfo.methodID);
            return JniHelper::jstring2string(jstr);
        }
#endif
    }
    
    std::string JNIController::getCpOrderId(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"getCpOrderId","()Ljava/lang/String;");
        if(ret)
        {
            jstr = (jstring)jni_methodInfo.env->CallStaticObjectMethod(jni_methodInfo.classID,jni_methodInfo.methodID);
            return JniHelper::jstring2string(jstr);
        }
#endif
    }
    
    void JNIController::setUserId(char const* str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setUserId","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    
    // money
    int JNIController::getMoneyStatus(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getMoneyStatus", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setMoneyStatus(int status){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setMoneyStatus", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, status);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    // gold
    int JNIController::getGoldStatus(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getGoldStatus", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setGoldStatus(int status){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setGoldStatus", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, status);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    // name
    void JNIController::setPlayerName(char const* str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setPlayerName","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    void JNIController::setPlayerLevel(char const* str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setPlayerLevel","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    void JNIController::setPlayerGold(char const* str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setPlayerGold","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    
    int JNIController::getRestartApplication(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getRestartApplication", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setRestartApplication(int status){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setRestartApplication", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, status);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    int  JNIController::getLandStatus(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getLandStatus", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setLandStatus(int smsStatus){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setLandStatus", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,smsStatus);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    // 支付
    void JNIController::isGamePay(int smsIndex){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "isGamePay", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,smsIndex);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    void JNIController::setChannelId(int channelId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setChannelId", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,channelId);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    int  JNIController::getSmsStatus(){
        int ret = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "getSmsStatus", "()I")){
    		return ret;
    	}
    	ret = jni_methodInfo.env->CallStaticIntMethod(jni_methodInfo.classID, jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
        return ret;
    }
    void JNIController::setSmsStatus(int status){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setSmsStatus", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID, status);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    void JNIController::setProductId(char const*  str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setProductId","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    std::string JNIController::getProductId(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"getProductId","()Ljava/lang/String;");
        if(ret)
        {
            jstr = (jstring)jni_methodInfo.env->CallStaticObjectMethod(jni_methodInfo.classID,jni_methodInfo.methodID);
            return JniHelper::jstring2string(jstr);
        }
#endif
    }
    void JNIController::setSidId(char const*  str){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        JniMethodInfo jni_methodInfo;
        bool ret;
        jstring jstr = NULL;
        ret = JniHelper::getStaticMethodInfo(jni_methodInfo,SDK_TIEGAO_CLASS,"setSidId","(Ljava/lang/String;)V");
        if (ret) {
            jstring jTitle = jni_methodInfo.env->NewStringUTF(str);
            jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,jTitle);
            jni_methodInfo.env->DeleteLocalRef(jTitle);
        }
#endif
    }
    
    void JNIController::isExtendData(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "isExtendData", "()V") ){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID,  jni_methodInfo.methodID);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    void JNIController::setData(int index){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    	JniMethodInfo jni_methodInfo;
    	if (! JniHelper::getStaticMethodInfo(jni_methodInfo, SDK_TIEGAO_CLASS, "setData", "(I)V")){
    		return;
    	}
    	jni_methodInfo.env->CallStaticVoidMethod(jni_methodInfo.classID, jni_methodInfo.methodID,index);
    	jni_methodInfo.env->DeleteLocalRef(jni_methodInfo.classID);
#endif
    }
    
    
    
    
}






