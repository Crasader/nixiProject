package com.mzplay.tiegao.baidu;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;

import com.snowfish.cn.ganga.helper.SFOnlineHelper;
import com.snowfish.cn.ganga.helper.SFOnlineLoginListener;
import com.snowfish.cn.ganga.helper.SFOnlinePayResultListener;
import com.snowfish.cn.ganga.helper.SFOnlineUser;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;

public class YijieLayer{
	
	public static Activity instance;
	
	private static String productName;
	private static int price;
	private static boolean landBool;
	
	public static int baiduIndex = 0;
	public static int putongIndex = 0;
	public static boolean openProductId;
	
	public YijieLayer(Activity act){
		instance = act;
		
		landBool = false;
		
		baiduIndex = 1;  // baidu
//		baiduIndex = 0;  // other
		
		putongIndex = 1;  // 邮件发送
//		putongIndex = 0;  // 普通发送
		
		openProductId = false;//不需要在易接工具内添加计费点的(乐视 有计费点，但是需要选择false)
//		openProductId = true;//需要在易接工具内添加计费点的(魅族、)
		
		
		initSDK();
	}
	
	/**
	 * initSDK
	 * 初始化SDK
	 */
	private void initSDK() {
		//onCreate方法用于需要在游戏主Activity中的onCreate中调用，只需调用一次
		SFOnlineHelper.onCreate(instance);
	}
	
	/*易接登陆接口*/
	void doLogin(int type){
		SFOnlineHelper.setLoginListener(instance, new SFOnlineLoginListener() {
			@Override
			public void onLoginSuccess(SFOnlineUser user, Object customParams) { //登陆成功回调
//				Log.i("main", "<><> onLoginSuccess" );
//				Log.v("main", "<><> customParams == " + customParams);
//				Log.w("main", "<><> user == " + user);
//				Log.e("main", "<><> landBool == " + landBool);
//				Log.e("main", "<><> baiduIndex == " + baiduIndex);
				if(baiduIndex == 1){
					// 百度
					if (landBool) {
						landBool = false;
						Tiegao.setRestartApplication(1);
					}else{
						LoginCheck(user);
					}
				}else{
					// 其它
					LoginCheck(user);
				}
			}
			@Override
			public void onLoginFailed(String reason, Object customParams) { //登陆失败回调
				Log.e("main", "<><> onLoginFailed: " + reason + ", " + customParams );
				if(baiduIndex == 1){
					// 百度
					if (landBool) {
						Tiegao.setRestartApplication(2);
						
					}else{
						Tiegao.setLandStatus(2);
					}
				}else{
					// 其它
					Tiegao.setLandStatus(2);
				}
//				Toast.makeText(instance, "账户登陆失败", Toast.LENGTH_SHORT).show();
			}
			@Override
			public void onLogout(Object customParams) { //登出回调
//				Log.e("main", "<><> onLogout:" + customParams);
//				Toast.makeText(instance, "账户登出", Toast.LENGTH_SHORT).show();
				Tiegao.setRestartApplication(1);
			}
		});
		if (type == 1) {
			SFOnlineHelper.login(instance, "Login");
		}else if (type == 2) {
			SFOnlineHelper.login(instance, "Loginwx");
		}
	}
	/**
     *  LoginCheck
     *  从服务器端验证用户是否登陆
     * @param user 登陆账户
     */
	public void LoginCheck(final SFOnlineUser user) {
		
//		Log.e("main", "<><> LoginCheck user: " + user.toString());
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					String url = LoginHelper.CP_LOGIN_CHECK_URL + createLoginURL(user);
					String result = LoginHelper.executeHttpGet(url);
// 					Log.e("main", "<><> LoginCheck result:" + result);
 					isAnalyticString(result);
				} catch (Exception e) {
//					Log.e("main", "<><> LoginCheck ERROR: " + e.toString());
				}
			}
		}).start();
	}
	public static void isAnalyticString(String str) {
		String[] tempError = null;
		tempError = str.split(";");
		
//		Log.i("main","<<<<<<<< tempError ===== " + tempError + " >>>>>>>>");
		if (tempError[0].equals("200")) {
			try {
				Tiegao.setSessionid(tempError[1]);
				Tiegao.setLandStatus(1);
				
				if(baiduIndex == 1){
					// 百度
					landBool = true;
				}else{
					// 其它
				}
				
				LoginHelper.showMessage("登录成功", instance);
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
			Tiegao.setLandStatus(2);
			if(baiduIndex == 1){
				// 百度
				landBool = false;
			}else{
				// 其它
			}
			LoginHelper.showMessage("未登录", instance);
			return;
		}
	}
	
	private String createLoginURL(SFOnlineUser user) throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?app=");
		// 易接平台创建的游戏ID
		builder.append(user.getProductCode());
		builder.append("&sdk=");
		// 易接平台标示的渠道SDK ID
		builder.append(user.getChannelId());
		builder.append("&uin=");
		// 渠道SDK标示的用户ID
		builder.append(URLEncoder.encode(user.getChannelUserId(), "utf-8"));
		builder.append("&sess=");
		// 渠道SDK登录完成后的Session ID
		builder.append(URLEncoder.encode(user.getToken(), "utf-8"));
		return builder.toString();
	}
	
	public void ucSdkSubmitExtendData() {
		
		/* 部分渠道如UC渠道，要对游戏人物数据进行统计，而且为接入规范，调用时间：在游戏角色登录成功后调用
		 *  public static void setRoleData(Context context, String roleId，
		 *  	String roleName, String roleLevel, String zoneId, String zoneName)
		 *  
		 *  @param context   上下文Activity
		 *  @param roleId    角色唯一标识
		 *  @param roleName  角色名
		 *  @param roleLevel 角色等级
		 *  @param zoneId    区域唯一标识
		 *  @param zoneName  区域名称
		 * */
		SFOnlineHelper.setRoleData(instance, Tiegao.getUserId(), Tiegao.getPlayerName(), Tiegao.getPlayerLevel(), "1", "女总Android大区");
	}
	
	
	
	
	
	public void pay(int index) {
		productName = null;
		price = 0;
		
		if (openProductId) {
			if (index == 0) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 1) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 2) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 3) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 4) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 5) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 6) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 10) {
				productName = "" + Tiegao.getGoldStatus();
			}else if (index == 9) {
				productName = "月卡";
			}
		}else{
			if (index == 0) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 1) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 2) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 3) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 4) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 5) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 6) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 10) {
				productName = "" + Tiegao.getGoldStatus() + "钻石";
			}else if (index == 9) {
				productName = "月卡";
			}
		}
		
		
		price = Tiegao.getMoneyStatus();
//		price = 1;
//		Log.e("main", "<><> price == " + price);
		
		String channelStr = "" + Tiegao.getChannelId();
		
		String mzStr;
		if (putongIndex == 1) {// 邮件发送
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + channelStr;
		}else{// 普通发送
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId();
		}
//		String mzStr = Tiegao.getProductId() + "";
		
		if (Tiegao.getChannelId() != 7) {
			// 其它
			SFOnlineHelper.pay(instance, price, productName, 1, mzStr, LoginHelper.CP_PAY_SYNC_URL, new SFOnlinePayResultListener() 
			{
				
				@Override
				public void onFailed(String remain) {
					// TODO Auto-generated method stub
					Tiegao.setSmsStatus(2);
					Toast.makeText(instance, "支付失败", Toast.LENGTH_LONG).show();
//					Log.e("main", "<><> onFailed remain == " + remain);
				}
				
				@Override
				public void onOderNo(String orderNo) {
					// TODO Auto-generated method stub
					Tiegao.setCpOrderId(orderNo);
					if (Tiegao.getSmsStatus() == 1) {
						Tiegao.setSmsStatus(1);
					}else{
						Tiegao.setSmsStatus(2);
					}
//					Log.e("main", "<><> onOderNo == " + orderNo + ";" + "<><> getSmsStatus == " + Tiegao.getSmsStatus());
				}
				
				@Override
				public void onSuccess(String remain) {
					// TODO Auto-generated method stub
					Tiegao.setSmsStatus(1);
//					Log.e("main", "<><> onSuccess remain == " + remain);
				}
				
			});
		}else{
			SFOnlineHelper.payExtend(instance, price, productName, null, "false", 1, mzStr, LoginHelper.CP_PAY_SYNC_URL, new SFOnlinePayResultListener(){
				
				@Override
				public void onFailed(String remain) {
					// TODO Auto-generated method stub
					Tiegao.setSmsStatus(2);
					Toast.makeText(instance, "支付失败", Toast.LENGTH_LONG).show();
//					Log.e("main", "<><> onFailed remain == " + remain);
				}
				
				@Override
				public void onOderNo(String orderNo) {
					// TODO Auto-generated method stub
					Tiegao.setCpOrderId(orderNo);
					if (Tiegao.getSmsStatus() == 1) {
						Tiegao.setSmsStatus(1);
					}else{
						Tiegao.setSmsStatus(2);
					}
//					Log.e("main", "<><> onOderNo == " + orderNo + ";" + "<><> getSmsStatus == " + Tiegao.getSmsStatus());
				}
				
				@Override
				public void onSuccess(String remain) {
					// TODO Auto-generated method stub
					Tiegao.setSmsStatus(1);
//					Log.e("main", "<><> onSuccess remain == " + remain);
				}
				
			});
		}
		
	}
	
	public static void setData(int type){
		JSONObject roleInfo = createJSONObject();
		if (type == 1) {// 创建新角色时调用
			SFOnlineHelper.setData(instance, "createrole", roleInfo.toString());
		}else if (type == 2) {// 玩家升级角色时调用
			SFOnlineHelper.setData(instance, "levelup", roleInfo.toString());
		}else if (type == 3){// 进入游戏
			SFOnlineHelper.setData(instance, "gamestart", roleInfo.toString());
		}else if (type == 4){// 退出游戏
			SFOnlineHelper.setData(instance, "gameexit", roleInfo.toString());
		}else if (type == 5){// 选择服务器进入接口
			SFOnlineHelper.setData(instance, "enterServer", roleInfo.toString());
		}
		
		
	}
    //创建JSONObject对象
    private static JSONObject createJSONObject(){
        JSONObject roleInfo = new  JSONObject();
        try {
        	// 当前登录的玩家角色ID。必须为数字
        	roleInfo.put("roleId", Tiegao.getUserId());
        	// 当前登录的玩家角色名，不能为空，不能为null
        	roleInfo.put("roleName", Tiegao.getPlayerName());
        	// 当前登录的玩家角色等级，必须为数字，且不能为0，若无传入1
        	roleInfo.put("roleLevel", Tiegao.getPlayerLevel());
        	// 当前登录的游戏区服ID，必须为数字，且不能为0.若无传入1
        	roleInfo.put("zoneId", "1");
        	// 当前登录的游戏区服名称，不能为空，不能为null
        	roleInfo.put("zoneName", "女总Android大区");
        	// 用户游戏币余额，必须为数字，若无，传入0
        	roleInfo.put("balance", Tiegao.getPlayerGold());
        	// 当前用户VIP等级，必须为数字，若无传入1
        	roleInfo.put("vip", "1");
        	// 当前角色所属帮派，不能为空，不能为null，若无传入“无帮派”
        	roleInfo.put("partyName", "无帮派");
        	
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        return  roleInfo;
    }
	
	
}







