-dontoptimize
-dontpreverify
-dontusemixedcaseclassnames
-dontskipnonpubliclibraryclasses

-verbose
-optimizations !code/simplification/arithmetic,!field/*,!class/merging/*

-keepattributes Exceptions,InnerClasses,Signature
-keepattributes *Annotation*
-keepattributes SourceFile,LineNumberTable

-dontwarn butterknife.internal.**
-keep class butterknife.** { *; }
-keep class **$$ViewBinder { *; }
-keep class **$$ViewInjector { *; }
-keepclasseswithmembernames class * {
    @butterknife.* <fields>;
}
-keepclasseswithmembernames class * {
    @butterknife.* <methods>;
}
-keepnames class * {
    @butterknife.InjectView *;
}






#-----------keep-------------------

-keep public class * extends android.app.Activity
-keep public class * extends android.app.Application
-keep public class * extends android.app.Service
-keep public class * extends android.content.BroadcastReceiver
-keep public class * extends android.content.ContentProvider
-keep public class * extends android.app.backup.BackupAgentHelper
-keep public class * extends android.preference.Preference
-keep public class com.android.vending.licensing.ILicensingService

-keepattributes Exceptions,InnerClasses
-keep public class com.alipay.android.app.** {
    public <fields>;
    public <methods>;
}

# Keep names - Native method names. Keep all native class/method names.
-keepclasseswithmembers,allowshrinking class * {
    native <methods>;
}

-keepclasseswithmembers,allowshrinking class * {
    public <init>(android.content.Context,android.util.AttributeSet);
}

-keepclasseswithmembers,allowshrinking class * {
    public <init>(android.content.Context,android.util.AttributeSet,int);
}

-keepclassmembers enum  * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

-keep class * extends android.os.Parcelable {
    public static final android.os.Parcelable$Creator *;
}

-ignorewarning
-keep public class * extends android.widget.TextView



#-----------keep httpclient -------------------
-keep class org.apache.** {
    public <fields>;
    public <methods>;
}

#--------------unionpay 3.3.0--------------
-keep class com.unionpay.** {*;}
-keep class com.UCMobile.PayPlugin.** {*;}
-keep class cn.gov.pbc.tsm.client.mobile.android.bank.service.** {*;}


-keepattributes *Annotation*
-keepattributes *JavascriptInterface*


#--------------alipay-------------
-keep class com.ta.utdid2.** {
    public <fields>;
    public <methods>;
}
-keep class com.ut.device.** {
    public <fields>;
    public <methods>;
}
-keep class com.alipay.android.app.** {
    public <fields>;
    public <methods>;
}
-keep class com.alipay.sdk.** {
    public <fields>;
    public <methods>;
}
-keep class com.alipay.mobilesecuritysdk.** {
    public <fields>;
    public <methods>;
}
-keep class HttpUtils.** { 
    public <fields>;
    public <methods>;
}
-keep class org.json.alipay.** {
    public <fields>;
    public <methods>;
}

#-----------keep iapppay -------------------
-keep class com.iapppay.tool {*;}
-keep class com.iapppay.service {*;}
-keep class com.iapppay.provider {*;}
-keep class com.iapppay.apppaysystem {*;}
-keep class com.iapppay.account.channel.ipay.IpayAccountApi {*;}
-keep class com.iapppay.account.channel.ipay.IpayOpenidApi {*;}
-keep class com.iapppay.account.channel.ipay.view.** {
    public <fields>;
    public <methods>;
}
-keep public class com.iapppay.fastpay.ui.** {
    public <fields>;
    public <methods>;
}
-keep public class com.iapppay.fastpay.view.** {
    public <fields>;
    public <methods>;
}
#-----------keep iapppay -------------------
-keep class com.iapppay.utils.RSAHelper {*;}
-keep class com.iapppay.network.** { 
    public <fields>;
    public <methods>;
}
-keep class com.iapppay.sdk.main.** { 
    public <fields>;
    public <methods>;
}
#iapppay_sub_pay
-keep public class com.iapppay.pay.channel.** { 
    public <fields>;
    public <methods>;
}
-keep class com.iapppay.interfaces.callback.** {*;}
-keep class com.iapppay.interfaces.** {
    public <fields>;
    public <methods>;
}

#iapppay UI
-keep public class com.iapppay.ui.activity.** {
    public <fields>;
    public <methods>;
}
# View
-keep public class com.iapppay.ui.widget.**{ 
    public <fields>;
    public <methods>;
}
-keep public class com.iapppay.ui.view.**{ 
    public <fields>;
    public <methods>;
}




-keep class **.R$* { *; }

# kk lib
-keep class com.kklibrary.gamesdk.** { *; }

# retrofit2
-dontwarn com.squareup.okhttp.**
-keep class com.squareup.okhttp.** { *; }
-keep interface com.squareup.okhttp.** { *; }
-dontwarn rx.**
-dontwarn okio.**
-keep class okio.** { *; }
-dontwarn retrofit2.**
-keep class retrofit2.** { *; }
-keepclasseswithmembers class * {
    @retrofit2.http.* <methods>;
}
# Platform calls Class.forName on types which do not exist on Android to determine platform.
-dontnote retrofit2.Platform
# Platform used when running on RoboVM on iOS. Will not be used at runtime.
-dontnote retrofit2.Platform$IOS$MainThreadExecutor
# Platform used when running on Java 8 VMs. Will not be used at runtime.
-dontwarn retrofit2.Platform$Java8
# Retain generic type information for use by reflection by converters and adapters.
-keepattributes Signature
# Retain declared checked exceptions for use by a Proxy instance.
-keepattributes Exceptions

### OKHTTP
# Platform calls Class.forName on types which do not exist on Android to determine platform.
-dontnote okhttp3.internal.Platform
-dontnote com.squareup.okhttp.internal.Platform

### OKIO
# java.nio.file.* usage which cannot be used at runtime. Animal sniffer annotation.
-dontwarn okio.Okio
# JDK 7-only method which is @hide on Android. Animal sniffer annotation.
-dontwarn okio.DeflaterSink

-dontwarn sun.misc.**
-keep class sun.misc.Unsafe { *; }
-dontwarn com.google.**
-keep class com.google.gson.stream.** { *; }
-keep class com.google.gson.examples.android.model.** { *; }
-keep class com.google.gson.** { *; }
-keep class com.google.protobuf.** { *; }
-keep class com.google.gson.GsonBuilder.** { *; }
-keep class **$Properties

-keepclasseswithmembernames class * {
    native <methods>;
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet);
}

-keepclasseswithmembers class * {
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

-keepclassmembers class * extends android.app.Activity {
    public void *(android.view.View);
}

-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}