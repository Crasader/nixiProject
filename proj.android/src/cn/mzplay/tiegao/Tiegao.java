/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package cn.mzplay.tiegao;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.util.Log;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

public class Tiegao extends Cocos2dxActivity {
	public static Tiegao instance;
	
	private static AlarmClock alarmClock = null;
	
	public static String shareImage = "";
	
	
	
	
	
	
	
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		instance = this;
		
		alarmClock = new AlarmClock(instance);
		
		
		// 055A2E7D7DB33E6E318D0D2263C57F2B 测试
//		TalkingDataGA.init(instance, "B9AF9FCF7CD821D4EB526416E2BD24FC", "mzplay");
		
	}

    static {
    	System.loadLibrary("game");
    }
    
    
    
    
    @Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		Log.i("", "===============onResume()");
	}
    
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		Log.i("", "================onPause()");
	}
	
	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		Log.i("", "================onBackPressed()");
		super.onBackPressed();
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		Log.i("", "================onDestroy()");
		super.onDestroy();
	}
	
	
	public static void showShare() {
//		ShareSDK.initSDK(instance);
//		OnekeyShare oks = new OnekeyShare();
//		Log.i("main", "" + getShareImage());
//		// imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
//		oks.setImagePath(getShareImage());//确保SDcard下面存在此张图片
//		// 启动分享GUI
//		oks.show(instance);
		
		
		ShareSDK.initSDK(instance);
		OnekeyShare oks = new OnekeyShare();
		//关闭sso授权
		oks.disableSSOWhenAuthorize(); 
		 
		// 分享时Notification的图标和文字  2.5.9以后的版本不调用此方法
		//oks.setNotification(R.drawable.ic_launcher, getString(R.string.app_name));
		// title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
//		oks.setTitle("男神日记");
		// text是分享文本，所有平台都需要这个字段
//		oks.setText("我是分享文本");
		// imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
		oks.setImagePath(getShareImage());//确保SDcard下面存在此张图片
		// url仅在微信（包括好友和朋友圈）中使用
//		oks.setUrl("http://sharesdk.cn");
		 
		// 启动分享GUI
		oks.show(instance);
	}
	// 获取用户信息
	public static String getShareImage(){
		return shareImage;
	}
	public static void setShareImage(String str){
		shareImage = str;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
