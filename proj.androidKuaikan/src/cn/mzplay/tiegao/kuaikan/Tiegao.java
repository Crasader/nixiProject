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
package cn.mzplay.tiegao.kuaikan;

import java.util.UUID;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.tendcloud.tenddata.TalkingDataGA;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

public class Tiegao extends Cocos2dxActivity {
	public static Tiegao instance;
	
	private static AlarmClock alarmClock = null;
	private static KuaikanLayer kuaikanLayer = null;
	
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
	
	static HandlerThread handlerThread;
	
	public static int landStatus = 0;
	public static int restartApplication = 0;
	public static int moneyStatus = 0;
	public static int smsStatus = 0;
	public static int goldStatus = 0;
	public static int shareStatus = 0;
	public static int payIndex = 0;
	public static int channelId = 0;
	
	
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		instance = this;
		
		alarmClock = new AlarmClock(instance);
		kuaikanLayer = new KuaikanLayer(instance);
		
		// 3B6C85B55D277722EE4A800FCC596B17		金山云
		
		
		// 49AC41ECB2C5B4A961E00B2380C21E9F		美图
		// 87EB2A02216D41878FAAA9493CA83984		美图 修正后
		
		// C5F4CD1E92B6718262FBF57950D8829D		百度
		// DB61C0FF7AD0549C91DD4FB203139909		360
		// 03BF303202380DA28B046607C2A79ABF		小米
		// C59DC7B087EEE431A985F76BBEEDC021		新浪
		// 77E611BA3283B34C2124D21F0C99632F		vivo
		// 390F4A5CC1E745A18A5FD5A0FD2B8BB2		应用宝
		// F267A73DD42E495CA22458F1B3877DF4		UC
		// 16366125DB1F4C8F8D0D2FB72AB03F5A		华为
		// E16E462BDA5642D7B47D28E24779DBDA		oppo
		// DE07CD19F8CF42B191BC300FB25268FB		乐视
		// AD4E23F681D342929E53042A6D833ADA		魅族
		// 2FE94C00154F41A383268CBA6D7BFF01		爱奇艺
		// E40DBB6DB5324C0898168BF55C4728F3		东东
		// F2535A634D60400796233DA475293730		当乐
		
		
		
		// 3812FAF07E794E8FA24F1C8B5F1D8554 	鱼丸
		// AF0BCB0920DE4FE3A663D840DC389E71 	鱼丸1.4.1
		
		
		
		// 1E05E5E92119499A8AF5C19F3FB47BAF		快看
		
		
		// 85C506A249F4A97CD676DE2A6D7C652B		测试1
		// 9C58D9E0BC45489B8920B2712A35AE35		测试2
		
		
		TalkingDataGA.init(instance, "1E05E5E92119499A8AF5C19F3FB47BAF", "mzplay");
	}
	
	
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return glSurfaceView;
	}
	
	
    static {
    	System.loadLibrary("game");
    }
    
    
    
    @Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		Log.i("", "===============onStop()");
	}
    
    @Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		Log.i("", "===============onRestart()");
	}
    
    @Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		TalkingDataGA.onResume(instance);
		Log.i("", "===============onResume()");
	}
    
    @Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		TalkingDataGA.onPause(instance);
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
		Log.i("", "================onDestroy()");
	}
	
	
	public static void showShare(int type, int num) {
//		ShareSDK.initSDK(instance);
//		OnekeyShare oks = new OnekeyShare();
//		Log.i("main", "" + getShareImage());
//		// imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
//		oks.setImagePath(getShareImage());//确保SDcard下面存在此张图片
//		// 启动分享GUI
//		oks.show(instance);
		
		
//		ShareSDK.initSDK(instance);
		OnekeyShare oks = new OnekeyShare();
		//关闭sso授权
		oks.disableSSOWhenAuthorize();
		
		// 分享时Notification的图标和文字  2.5.9以后的版本不调用此方法
		//oks.setNotification(R.drawable.ic_launcher, getString(R.string.app_name));
		// title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
		if (type == 1) {
			oks.setTitle("我在《女总裁的贴身高手》里已经收集" + num + "件衣服了，快来和我一起玩吧！");
		}else if (type == 2){
			oks.setTitle("我在《女总裁的贴身高手》获得满星通关，一起创建商业帝国吧！");
		}else if (type == 3){
			oks.setTitle("我在《女总裁的贴身高手》抽到了极品服饰，来试试你的人品吧！");
		}else if (type == 4){
			oks.setTitle("我在《女总裁的贴身高手》搭配比拼中完胜对手，你敢来挑战我么？");
		}
		// text是分享文本，所有平台都需要这个字段
//		oks.setText("女总裁的贴身高手");
		// imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
		oks.setImagePath(getShareImage());//确保SDcard下面存在此张图片
		// url仅在微信（包括好友和朋友圈）中使用
//		oks.setUrl("http://sharesdk.cn");
		
		// 启动分享GUI
		oks.show(instance);
		
//		ShareParams wechat = new ShareParams();
//		wechat.setTitle("女总裁的贴身高手");
//		wechat.setText("最爱女总裁的贴身高手");
//		wechat.setImageUrl("http://f1.sharesdk.cn/imgs/2014/02/26/owWpLZo_638x960.jpg");
//		wechat.setUrl("http://mob.com");
//		wechat.setShareType(Platform.SHARE_WEBPAGE);
//		
//		Platform weixin = ShareSDK.getPlatform(instance, Wechat.NAME);
//		weixin.setPlatformActionListener(instance);
//		weixin.share(wechat);
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
	
	
	// 获取channelId
	public static int getChannelId(){
		return channelId;
	}
	public static void setChannelId(int index){
		channelId = index;
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
	
	public static String getOpenId() {
        String s = UUID.randomUUID().toString();
        setSessionid(s);
        return s;
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
		landingHandler.sendMessage(msg);
	}
 	static Handler landingHandler = new Handler(){
    	public void handleMessage(Message msg){
    		kuaikanLayer.doLogin(msg.arg1);
    	}
 	};
 	
 	// 注销接口
 	public static void isUnLanding(){
		Message msg = new Message();
		msg.arg1 = 1;
		unLandingHandler.sendMessage(msg);
	}
 	static Handler unLandingHandler = new Handler(){
    	public void handleMessage(Message msg){
    		kuaikanLayer.unLogin();
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
// 				yijieLayer.ucSdkSubmitExtendData();
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
// 			yijieLayer.setData(msg.arg1);
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
 			kuaikanLayer.pay(msg.arg1);
 		}
 	};
	
	
}
