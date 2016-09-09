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
    
    // 分享
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
    
    
}






