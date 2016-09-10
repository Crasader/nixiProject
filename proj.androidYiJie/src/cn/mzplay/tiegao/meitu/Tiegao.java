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
package cn.mzplay.tiegao.meitu;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.snowfish.cn.ganga.helper.SFOnlineExitListener;
import com.snowfish.cn.ganga.helper.SFOnlineHelper;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import cn.mzplay.tiegao.meitu.YijieLayer;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

public class Tiegao extends Cocos2dxActivity {
	public static Tiegao instance;
	
	private static AlarmClock alarmClock = null;
	private static YijieLayer yijieLayer = null;
	
	public static String shareImage = "";
	public static String session = "";
	public static String userId = "";
	public static String playerLevel = "";
	public static String playerGold = "";
	public static String productId = "";
	public static String sidId = "";
	public static String cpOrderId = "";
	public static String playerName = "";
	public static String shareText = "";
	
	
	public static int landStatus = 0;
	public static int restartApplication = 0;
	public static int moneyStatus = 0;
	public static int smsStatus = 0;
	public static int goldStatus = 0;
	public static int shareStatus = 0;
	
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		instance = this;
		
		alarmClock = new AlarmClock(instance);
		yijieLayer = new YijieLayer(instance);
		
		// 055A2E7D7DB33E6E318D0D2263C57F2B 测试
//		TalkingDataGA.init(instance, "B9AF9FCF7CD821D4EB526416E2BD24FC", "mzplay");
		
	}

    static {
    	System.loadLibrary("game");
    }
    
    
    
    @Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		SFOnlineHelper.onStop(instance);
		Log.i("", "===============onStop()");
	}
    
    @Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		SFOnlineHelper.onRestart(instance);
		Log.i("", "===============onRestart()");
	}
    
    @Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		SFOnlineHelper.onResume(instance);
//		TalkingDataGA.onResume(instance);
		Log.i("", "===============onResume()");
	}
    
    @Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		SFOnlineHelper.onPause(instance);
//		TalkingDataGA.onPause(instance);
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
		super.onDestroy();
		SFOnlineHelper.onDestroy(instance);
		Log.i("", "================onDestroy()");
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
	public static void setShareText(String str){
		shareText = str;
	}
	public static void shareText() {
		shareTextHandler.sendEmptyMessage(0);
	}
	static Handler shareTextHandler = new Handler(){
		public void handleMessage(Message msg){ 
			Toast.makeText(instance, shareText, Toast.LENGTH_SHORT).show();
		}
    };
	public static int getShareStatus(){
		return shareStatus;
	}
	public static void setShareStatus(int status){
		shareStatus = status;
	}
	
	
	
	// 获取用户信息
	public static String getShareImage(){
		return shareImage;
	}
	public static void setShareImage(String str){
		shareImage = str;
	}
	
	public static int getLandStatus(){
		return landStatus;
	}
	public static void setLandStatus(int status){
		landStatus = status;
	}
	
	public static int getRestartApplication(){
		return restartApplication;
	}
	public static void setRestartApplication(int status){
		restartApplication = status;
	}
	
	public static String getSessionid(){
		return session;
	}
	public static void setSessionid(String _sessionid){
		session = _sessionid;
	}
	
	// userid
	public static String getUserId(){
		return userId;
	}
	public static void setUserId(String str){
		userId = str;
	}
	// 玩家等级
	public static String getPlayerLevel(){
		return playerLevel;
	}
	public static void setPlayerLevel(String level){
		playerLevel = level;
	}
	// 玩家钻石
	public static String getPlayerGold(){
		return playerGold;
	}
	public static void setPlayerGold(String gold){
		playerGold = gold;
	}
	// money
	public static int getMoneyStatus(){
		return moneyStatus;
	}
	public static void setMoneyStatus(int status){
		moneyStatus = status;
	}
	
	public static String getProductId(){
		return productId;
	}
	public static void setProductId(String str){
		productId = str;
	}
	
	public static String getSidId(){
		return sidId;
	}
	public static void setSidId(String str){
		sidId = str;
	}
	
	public static int getSmsStatus(){
		return smsStatus;
	}
	public static void setSmsStatus(int status){
		smsStatus = status;
	}
	
	// 获取用户信息
	public static String getCpOrderId(){
		return cpOrderId;
	}
	public static void setCpOrderId(String str){
		cpOrderId = str;
	}
	
	// 获取用户信息
	public static String getPlayerName(){
		return playerName;
	}
	public static void setPlayerName(String str){
		playerName = str;
	}
	// gold
	public static int getGoldStatus(){
		return goldStatus;
	}
	public static void setGoldStatus(int status){
		goldStatus = status;
	}
	
	
	
	// 判断网络
	public static Boolean getNetworkAvailable(){
		ConnectivityManager connectivityManager = (ConnectivityManager)instance.getSystemService(CONNECTIVITY_SERVICE);  
		if (connectivityManager == null) {
			return false;
		}else {
			NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();  
			if(networkInfo == null || !networkInfo.isAvailable())  
			{  
			    //当前有可用网络  
				Log.i("main", "<><><><><><>当前有可用网络");
				return true;
			}  
			else   
			{  
			    //当前无可用网络  
				Log.i("main", "<><><><><><>当前无可用网络");
				return false;
			}
		}
	}
	
	
	// 登陆接口
	public static void isLanding(int index){
		Message msg = new Message();
		msg.arg1 = index;			
		moreHandler1.sendMessage(msg);
	}
 	static Handler moreHandler1 = new Handler(){
    	public void handleMessage(Message msg){
    		yijieLayer.doLogin(msg.arg1);
    	}
 	};
	
	
 	// 退出游戏
 	public static void exitGame(int index){
 		Message msg = new Message();
 		msg.arg1 = index;			
 		exitHandler.sendMessage(msg);
 	}
 	static Handler exitHandler = new Handler(){
 		public void handleMessage(Message msg){
 			if (msg.arg1 == 0) {
 				instance.finish();
				System.exit(0);
 			}else if (msg.arg1 == 1){
                 SFOnlineHelper.exit(instance, new SFOnlineExitListener() {
                 	
 					@Override
 					public void onNoExiterProvide() {
 						AlertDialog.Builder builder = new AlertDialog.Builder(instance);
 		                builder.setTitle("是否退出游戏?");
 		                builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
 		                    public void onClick(DialogInterface dialog, int whichButton) {
 		                        // 这里添加点击确定后的逻辑
 		                        dialog.dismiss();
 		                        instance.finish();
 								System.exit(0);
 		                    }
 		                });
 		                builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
 		                    public void onClick(DialogInterface dialog, int whichButton) {
 		                        // 这里添加点击取消后的逻辑
 		                        dialog.dismiss();
 		                    }
 		                });
 		                builder.create().show();
 					}
 					
 					@Override
 					public void onSDKExit(boolean bool) {
 						if (bool){
 							//apk退出函数，demo中也有使用System.exit()方法；但请注意360SDK的退出使用exit（）会导致游戏退出异常
 							instance.finish();
 							System.exit(0);
 						}
 					}
                 });
 			}
 		}
 	};
	
	
 	// UC玩家信息接口
 	public static void isExtendData(){
 		Message msg = new Message();
 		msg.arg1 = 1;			
 		extendDataHandler.sendMessage(msg);
 	}
 	static Handler extendDataHandler = new Handler(){
 		public void handleMessage(Message msg){
 			if (msg.arg1 == 1){
 				yijieLayer.ucSdkSubmitExtendData();
 			}
 	 	}
 	};
 	// UC玩家信息接口
 	public static void setData(int index){
 		Message msg = new Message();
 		msg.arg1 = index;			
 		setDataHandler.sendMessage(msg);
 	}
 	static Handler setDataHandler = new Handler(){
 		public void handleMessage(Message msg){
 			yijieLayer.setData(msg.arg1);
 		}
 	};
	
	
 	// 支付接口
 	public static void isGamePay(int index) {
 		Log.i("main", "isGamePay<><><><>"+index+"");
 		Message msg = new Message();
 		msg.arg1 = index;
 		payHandler.sendMessage(msg);
 	}
 	static Handler payHandler = new Handler(){
 		public void handleMessage(Message msg){ 
 			yijieLayer.pay(msg.arg1);
 		}
 	};
	
	
	
	
}
