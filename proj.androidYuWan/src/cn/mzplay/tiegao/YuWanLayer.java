package cn.mzplay.tiegao;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

import org.json.JSONObject;

import com.yuwan8.middleware.Null;
import com.yuwan8.middleware.Order;
import com.yuwan8.middleware.PlayerInfo;
import com.yuwan8.middleware.Response;
import com.yuwan8.middleware.YW;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class YuWanLayer{
	
	public static Activity instance;
	
	private static String productName;
	private static String tokenStr;
	private static int price;
	static private char[] alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=".toCharArray();
	public static Order order;
	public static String mzStr = "";
	public static String _UUID22 = "";
	public static String _gameTempId = "";
	public static Boolean _payBool = false;
	
	public YuWanLayer(Activity act){
		instance = act;
		
		initSDK();
	}
	
	/**
	 * initSDK
	 * 初始化SDK
	 */
	private void initSDK() {
		//onCreate方法用于需要在游戏主Activity中的onCreate中调用，只需调用一次
		String appKey = "YnsI0M6MC3LkVse0YJciSV6s";  	// appKey向“鱼丸”申请。
	 	YW.getInstance().init(instance, appKey); 		// 调用处
	 	YW.getInstance().onActivityCreate(instance);
	 	YW.getInstance().wxLoginCallBack(instance.getIntent(), instance);
        YW.getInstance().setExitStrategy(mIExitStrategy);
        
        
//        Log.i("mian", "<><><><> 初始化完成");
	}
	
	/*易接登陆接口*/
	@SuppressWarnings("serial")
	void doLogin(int type){
		// 调用处
		YW.getInstance().login(new Response<String>() {
			
			@Override
			public void onFailure(String msg) {
				Toast.makeText(instance, "login msg = " + msg, Toast.LENGTH_LONG).show();
				Tiegao.setLandStatus(2);
			}
			
			@Override
			public void onSuccess(String data) {
				tokenStr = data;
				LoginCheck(tokenStr);
				Log.i("main", "<><><><><> 登陆成功： tokenStr === " + tokenStr);
				Log.i("main", "<><><><><> 登陆成功： data === " + data);
			}
		});
	}
        
	/**
     *  LoginCheck
     *  从服务器端验证用户是否登陆
     * @param user 登陆账户
     */
	public void LoginCheck(final String token) {
		
		Log.e("main", "<><> LoginCheck token: " + token.toString());
		new Thread(new Runnable() {
			@Override
			public void run() { 
				try {
					String url = LoginHelper.CP_LOGIN_CHECK_URL + createLoginURL(token);
					String result = LoginHelper.executeHttpGet(url);
 					Log.e("main", "<><> LoginCheck result:" + result);
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
		
		Log.i("main","<<<<<<<< tempError ===== " + tempError + " >>>>>>>>");
		if (tempError[0].equals("200")) {
			try {
				Tiegao.setSessionid(tempError[1]);
				Tiegao.setCreatTime(Integer.parseInt(tempError[2]));
				Tiegao.setLandStatus(1);
				
				LoginHelper.showMessage("登录成功", instance);
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
			Tiegao.setLandStatus(2);
			
			LoginHelper.showMessage("未登录", instance);
			return;
		}
	}
	
	private String createLoginURL(String token) throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?token=");
		// 渠道SDK登录完成后的Session ID
		builder.append(URLEncoder.encode(token, "utf-8"));
		return builder.toString();
	}
	
	
	public void ucSdkSubmitExtendData() {
	}
	
	
	public void pay(int index) {
		productName = null;
		price = 0;
		_UUID22 = Tiegao.getUserId() + getUUID22();
		Tiegao.setCpOrderId(_UUID22);
		
		if (index == 0) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 1) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 2) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 3) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 4) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 5) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 6) {
			_payBool = true;
			productName = "" + Tiegao.getGoldStatus() + "钻石";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + Tiegao.getGoldStatus() + ";" + 1;
		}else if (index == 10) {
			_payBool = false;
			productName = "Vip剧情";
			mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId() + ";" + "Vip" + ";" + 2;
		}
//		Log.e("main", "<><> index == " + index +"    productName == " + productName);
		price = Tiegao.getMoneyStatus();
//		price = 1;
		
		order = getOrder1(mzStr, price);
		
		new Thread(new Runnable() {
			@Override
			public void run() { 
				try {
					String url = LoginHelper.CP_PAY_CHECK_URL + createPayURL();
					String result = LoginHelper.executeHttpGet(url);
// 					Log.e("main", "<><> Pay result:" + result);
 					isPayAnalyticString(result);
				} catch (Exception e) {
					Log.e("main", "<><> Pay ERROR: " + e.toString());
				}
			}
		}).start();
	}
	private String createPayURL() throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?GameName=" + URLEncoder.encode(order.getGameName(), "utf-8"));
		builder.append("&money=" + order.getMoney());
		builder.append("&orderID=" + order.getOrderID());
		builder.append("&productId=" + order.getProductId());
		if (_payBool) {
			builder.append("&productName=" + Tiegao.getGoldStatus());
		}else{
			builder.append("&productName=" + "Vip");
		}
		
		builder.append("&uid=" + order.getUid());
//		Log.e("main", "<><> Pay builder:" + builder);
//		Log.i("main", "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>");
		return builder.toString();
	}
	
	
	
	public static void isPayAnalyticString(String str) {
		String[] tempError = null;
		tempError = str.split(";");
		
//		Log.i("main","<<<<<<<< Pay tempError ===== " + tempError + " >>>>>>>>");
		if (tempError[0].equals("200")) {
			try {
				Tiegao.setSign(tempError[1]);
				order = getOrder2();
				yuwanPay();
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
			Tiegao.setSmsStatus(2);
			return;
		}
	}
	public static void yuwanPay(){
		Message msg = new Message();
		msg.arg1 = 1;			
		yuwanPayHandler.sendMessage(msg);
	}
 	static Handler yuwanPayHandler = new Handler(){
    	public void handleMessage(Message msg){
    		YW.getInstance().pay(order, new Response<Null>() {
    			@Override
    			public void onSuccess(Null aNull) {
    				// Logic for pay success.
    		     	Tiegao.setSmsStatus(1);
    		     	Log.e("main", "<><> onSuccess remain == " + aNull);
    			}
    		
    			@Override
    			public void onFailure(String reason) {
    				// Logic for pay failure.
    				if (Tiegao.getSmsStatus() == 1) {
    					Tiegao.setSmsStatus(1);
    				}else{
    					Tiegao.setSmsStatus(2);
    				}
    				Toast.makeText(instance, "支付失败", Toast.LENGTH_LONG).show();	
//    				Log.e("main", "<><> onFailed reason == " + reason);
    			}
    		});
    	}
 	};
	
	private static Order getOrder1(String mzStr, int _price) {
		Order order = new Order();
		order.setProductId(Tiegao.getProductId());					// 商品id。最大 16 字符。
		order.setProductName(productName);							// 商品名称
		
		order.setMoney("" + _price);								// 商品支付总金额。以分为单位，整型值，不能为小数
		order.setRatio("10");										// 兑换比例
		order.setCoinNum(Tiegao.getPlayerGold());					// 用户金钱数量
        // 测试的uid
		String[] tempUserId = null;
		tempUserId = Tiegao.getUserId().split("_");
		order.setUid(tempUserId[1]);								// 登录验证时游戏服务端从中间件服务端获取到的uid
		order.setGameName("女总裁的贴身高手");							// 游戏名称。最大16个中文字符
		order.setRoleName(Tiegao.getUserId());						// 应用内用户id，如角色名称。最大16个中文字符
		order.setRoleId(Tiegao.getPlayerName());					// 角色名称
		
		int level = 0;
		try{
			level = Integer.parseInt(Tiegao.getPlayerLevel());
		}catch(NumberFormatException e) {
		    e.printStackTrace();
		}
		order.setRoleLevel(level);									// 角色等级
		order.setOrderID(_UUID22);									// 订单id
		order.setExtension(mzStr);									// 扩展字段
		order.setNotify_url("http://tgtest.yunlookgame.cn:9765/purchase/yuwan/callback");// 支付回调参数，接入时测试使用
		
		return order;
	}
	private static Order getOrder2(){
		order.setSign(Tiegao.getSign());
		return order;
	}
	
	public static void setData(int type){
		PlayerInfo playerInfo = new PlayerInfo();
		if (type == 1) {// 选择服务器
			playerInfo.setDataType(PlayerInfo.TYPE_SELECT_SERVER);	// 信息类型，以上5种参数中选择  		必填
		}else if (type == 2) {// 创建角色
			playerInfo.setDataType(PlayerInfo.TYPE_CREATE_ROLE);	// 信息类型，以上5种参数中选择  		必填
		}else if (type == 3) {// 进入游戏
			playerInfo.setDataType(PlayerInfo.TYPE_ENTER_GAME);		// 信息类型，以上5种参数中选择  		必填
		}else if (type == 4) {// 等级提升
			playerInfo.setDataType(PlayerInfo.TYPE_LEVEL_UP);		// 信息类型，以上5种参数中选择  		必填
		}else if (type == 5) {// 退出游戏
			playerInfo.setDataType(PlayerInfo.TYPE_EXIT_GAME);		// 信息类型，以上5种参数中选择  		必填
		}else{
			playerInfo.setDataType(PlayerInfo.TYPE_ENTER_GAME);		// 信息类型，以上5种参数中选择  		必填
		}
		playerInfo.setRoleID(Tiegao.getUserId());					// 角色id 		必填
		playerInfo.setRoleName(Tiegao.getPlayerName());				// 角色名称   	必填
		playerInfo.setRoleLevel(Tiegao.getPlayerLevel());			// 角色等级   	必填
		playerInfo.setServerID(1);									// 服务器id   	必填
		playerInfo.setServerName("女总鱼丸大区");						// 服务器区服     	必填
		int gold = 0;
		try{
			gold = Integer.parseInt(Tiegao.getPlayerGold());
		}catch(NumberFormatException e) {
		    e.printStackTrace();
		}
		playerInfo.setMoneyNum(gold);								// 玩家金钱数量  	选填
		playerInfo.setVip("0");		 								// 玩家的VIP等级  	必填
		playerInfo.setRoleCTime(Tiegao.getCreatTime()); 			// 游戏角色创建时间 ——毫秒值(10位数)	必填
		
		
		YW.getInstance().submitPlayerInfo(playerInfo, new Response<String>() {
			
			@Override
			public void onFailure(String arg0) {
				Toast.makeText(instance, "游戏信息提交失败", Toast.LENGTH_LONG).show();
				
			}
			
			@Override
			public void onSuccess(String msg) {
				
			}
		});
	}
	
	public YW.IExitStrategy mIExitStrategy = new YW.IExitStrategy() {
		@Override
		public void onShowExitDialog() {
			new Handler(instance.getMainLooper()).post(new Runnable() {

				@Override
				public void run() {
					final AlertDialog.Builder builder = new AlertDialog.Builder(instance);
					builder.setTitle("提示");
					builder.setMessage("您确定要退出吗？");
					builder.setPositiveButton("确定", new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
		                	instance.finish();
							System.exit(0);
						}
					});
					builder.setNegativeButton("取消", new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							dialog.dismiss();
						}
					});
					builder.create().show();
				}
			});
		}
		@Override
		public void onExitDirectly() {
			instance.finish();
			System.exit(0);
		}
	};
	
	
	
	public static String getUUID22() {  
        UUID uuid = UUID.randomUUID();  
        long msb = uuid.getMostSignificantBits();  
        long lsb = uuid.getLeastSignificantBits();  
        char[] out = new char[24];  
        int tmp = 0, idx = 0;  
        
        // 循环写法  
        int bit = 0, bt1 = 8, bt2 = 8;  
        int mask = 0x00, offsetm = 0, offsetl = 0;  
        
        for(; bit < 16; bit += 3, idx += 4) {  
            offsetm = 64 - (bit + 3) * 8;  
            offsetl = 0;  
            tmp = 0;  
            
            if(bt1 > 3) {  
                mask = (1 << 8 * 3) - 1;  
            } else if(bt1 >= 0) {  
                mask = (1 << 8 * bt1) - 1;  
                bt2 -= 3 - bt1;  
            } else {  
                mask = (1 << 8 * ((bt2 > 3) ? 3 : bt2)) - 1;  
                bt2 -= 3;  
            }  
            if(bt1 > 0) {  
                bt1 -= 3;  
                tmp = (int) ((offsetm < 0) ? msb : (msb >>> offsetm) & mask);  
                if(bt1 < 0) {  
                    tmp <<= Math.abs(offsetm);  
                    mask = (1 << 8 * Math.abs(bt1)) - 1;  
                }  
            }  
            if(offsetm < 0) {  
                offsetl = 64 + offsetm;  
                tmp |= ((offsetl < 0) ? lsb : (lsb >>> offsetl)) & mask;  
            }  
            
            if(bit == 15) {  
                out[idx + 3] = alphabet[64];  
                out[idx + 2] = alphabet[64];  
                tmp <<= 4;  
            } else {  
                out[idx + 3] = alphabet[tmp & 0x3f];  
                tmp >>= 6;  
                out[idx + 2] = alphabet[tmp & 0x3f];  
                tmp >>= 6;  
            }  
            out[idx + 1] = alphabet[tmp & 0x3f];  
            tmp >>= 6;  
            out[idx] = alphabet[tmp & 0x3f];  
        }  
        
        return new String(out, 0, 22);  
    } 
}







