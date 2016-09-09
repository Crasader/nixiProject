package cn.mzplay.malestar.anzhi;

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
	private static int baiduIndex;
	
	public YijieLayer(Activity act){
		instance = act;
		
		landBool = false;
//		baiduIndex = 1;  // baidu
		baiduIndex = 0;  // other
		
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
				Log.e("main", "<><> onLoginSuccess" );
				Log.e("main", "<><> customParams == " + customParams);
				if(baiduIndex == 1){
					// 百度
					if (landBool) {
						landBool = false;
						Mm3c.setRestartApplication(1);
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
//				Log.e("main", "<><> onLoginFailed: " + reason + ", " + customParams );
				if(baiduIndex == 1){
					// 百度
					if (landBool) {
						Mm3c.setRestartApplication(2);
						
					}else{
						Mm3c.setLandStatus(2);
					}
				}else{
					// 其它
					Mm3c.setLandStatus(2);
				}
//				Toast.makeText(instance, "账户登陆失败", Toast.LENGTH_SHORT).show();
			}
			@Override
			public void onLogout(Object customParams) { //登出回调
//				Log.e("main", "<><> onLogout:" + customParams);
//				Toast.makeText(instance, "账户登出", Toast.LENGTH_SHORT).show();
				Mm3c.setRestartApplication(1);
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
		
		Log.e("main", "<><> LoginCheck user: " + user.toString());
		new Thread(new Runnable() {
			@Override
			public void run() { 
				try {
					String url = LoginHelper.CP_LOGIN_CHECK_URL + createLoginURL(user);
					String result = LoginHelper.executeHttpGet(url);
 					Log.e("main", "<><> LoginCheck result:" + result);
 					isAnalyticString(result);
				} catch (Exception e) {
					Log.e("main", "<><> LoginCheck ERROR: " + e.toString());
				}
			}
		}).start();
	}
	public static void isAnalyticString(String str) {
		String[] tempError = null;
		tempError = str.split(";");
		
		Log.i("main","<<<<<<<< tempError ===== " + tempError + " >>>>>>>>");
		if (tempError[0].equals("200")) {
			try {
				Mm3c.setSessionid(tempError[1]);
				Mm3c.setLandStatus(1);
				
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
			Mm3c.setSmsStatus(2);
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
		SFOnlineHelper.setRoleData(instance, Mm3c.getUserId(), Mm3c.getPlayerName(), Mm3c.getPlayerLevel(), "1", "男神大区");
	}
	
	
	
	
	
	public void pay(int index) {
		productName = null;
		price = 0;
		
		if (index == 0) {
			productName = "" + Mm3c.getGoldStatus() + "钻石";
		}else if (index == 1) {
			productName = "" + Mm3c.getGoldStatus() + "钻石";
		}else if (index == 2) {
			productName = "" + Mm3c.getGoldStatus() + "钻石";
		}else if (index == 3) {
			productName = "" + Mm3c.getGoldStatus() + "钻石";
		}else if (index == 4) {
			productName = "" + Mm3c.getGoldStatus() + "钻石";
		}
		else if (index == 10) {
			productName = "一元购";
		}
		else if (index == 30) {
			productName = "VIP包月";
		}
		price = Mm3c.getMoneyStatus();
//		price = 1;
//		Log.e("main", "<><> productName == " + productName);
		
		
		// 其它
		SFOnlineHelper.pay(instance, price, productName, 1, Mm3c.getProductId(), LoginHelper.CP_PAY_SYNC_URL, new SFOnlinePayResultListener() 
		{

			@Override
			public void onFailed(String remain) {
				// TODO Auto-generated method stub
				Mm3c.setSmsStatus(2);
				Toast.makeText(instance, "支付失败", Toast.LENGTH_LONG).show();
				Log.e("main", "<><> onFailed remain == " + remain);
			}

			@Override
			public void onOderNo(String orderNo) {
				// TODO Auto-generated method stub
				Mm3c.setCpOrderId(orderNo);
				if (Mm3c.getSmsStatus() == 1) {
					Mm3c.setSmsStatus(1);
				}else{
					Mm3c.setSmsStatus(2);
				}
				Log.e("main", "<><> onOderNo == " + orderNo + ";" + "<><> getSmsStatus == " + Mm3c.getSmsStatus());
			}

			@Override
			public void onSuccess(String remain) {
				// TODO Auto-generated method stub
				Mm3c.setSmsStatus(1);
				Log.e("main", "<><> onSuccess remain == " + remain);
			}
			
		});
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
        	roleInfo.put("roleId", Mm3c.getUserId());
        	// 当前登录的玩家角色名，不能为空，不能为null
        	roleInfo.put("roleName", Mm3c.getPlayerName());
        	// 当前登录的玩家角色等级，必须为数字，且不能为0，若无传入1
        	roleInfo.put("roleLevel", Mm3c.getPlayerLevel());
        	// 当前登录的游戏区服ID，必须为数字，且不能为0.若无传入1
        	roleInfo.put("zoneId", "1");
        	// 当前登录的游戏区服名称，不能为空，不能为null
        	roleInfo.put("zoneName", "男神大区");
        	// 用户游戏币余额，必须为数字，若无，传入0
        	roleInfo.put("balance", Mm3c.getPlayerGold());
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







