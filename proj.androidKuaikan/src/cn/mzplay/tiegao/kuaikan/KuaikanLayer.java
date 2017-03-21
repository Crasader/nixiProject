package cn.mzplay.tiegao.kuaikan;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.MessageDigest;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.UUID;

import org.json.JSONException;
import org.json.JSONObject;

import com.kklibrary.gamesdk.KKAgent;
import com.kklibrary.gamesdk.OauthResultCallBack;
import com.kklibrary.gamesdk.PayResultCallBack;
import com.kklibrary.gamesdk.rest.model.OauthResultModel;
import com.ta.utdid2.android.utils.StringUtils;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;
import sun.misc.BASE64Encoder;

@SuppressLint("NewApi")
public class KuaikanLayer{
	
	public static Activity instance;
	
	private static String wares_id;
	private static String open_id;
	private static String trans_data;
	private static String trans_secret;
	private static int price;
	private static boolean landBool;
		
	public static int baiduIndex = 0;
	public static int putongIndex = 0;
	public static boolean openProductId;
	
	private static final String PREFERENCES_NAME                         = "com_kuaikan_gamesdk_demo_android";
    private static final String KEY_SDK_OPEN_ID                          = "key_sdk_open_id";
	
	static private char[] alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_=".toCharArray();
	
	
	public KuaikanLayer(Activity act){
		instance = act;
				
		landBool = false;
		
//		baiduIndex = 1;  // baidu
		baiduIndex = 0;  // other
		
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
		KKAgent.getInstance().init(instance, "1035", false);
	}
	
	void unLogin(){
		setOpenId("");
		KKAgent.getInstance().removeAccount(instance);
//		Tiegao.setRestartApplication(1);
	}
	
	void doLogin(int type){
		KKAgent.getInstance().addAccount(instance, "1035", new OauthResultCallBack<OauthResultModel>() {

            @Override
            public void onFailure(Exception e, int i) {
                Tiegao.setLandStatus(2);
            }

            @Override
            public void onCancel() {
                Tiegao.setLandStatus(2);
            }

            @Override
            public void onAddAccount(OauthResultModel oauthResultModel) {                
//                Log.i("main", "openId1 == " + oauthResultModel.getOpenId());
                setOpenId(oauthResultModel.getOpenId());
                LoginCheck(oauthResultModel);
            }

            @Override
            public void onRemoveAccount() {                
//                Log.i("main", "openId2 == " + "");
            }

        });
	}
	
	
	public static void setOpenId(String openId) {
        SharedPreferences pref = instance.getSharedPreferences(PREFERENCES_NAME, instance.MODE_PRIVATE);
        Editor editor = pref.edit();
        editor.putString(KEY_SDK_OPEN_ID, openId);
        editor.apply();
    }

    public static String getOpenId() {
        SharedPreferences pref = instance.getSharedPreferences(PREFERENCES_NAME, instance.MODE_PRIVATE);
        return pref.getString(KEY_SDK_OPEN_ID, "");
    }
	
	
	
	/**
     *  LoginCheck
     *  从服务器端验证用户是否登陆
     * @param user 登陆账户
     */
	public void LoginCheck(final OauthResultModel user) {
		
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
					Tiegao.setLandStatus(2);
				}
			}
		}).start();
	}
	public static void isAnalyticString(String str)  throws Exception{
		JSONObject jo = new JSONObject(str);
		String code1 = jo.getString("code");
		int code2 = jo.getInt("code");
		String data = jo.getString("data");
		
		
		JSONObject dataJo = new JSONObject(data);
		open_id = dataJo.getString("open_id");
		
//		Log.i("main", "code1 == " + code1);
//		Log.i("main", "code2 == " + code2);
//		
//		Log.i("main", "data == " + data);
//		Log.i("main","<<<<<<<< tempError ===== " + str + " >>>>>>>>");
		if (code1.equals("200") || code2 == 200) {
			Tiegao.setSessionid(900 + "_" + open_id);
			Tiegao.setLandStatus(1);
			return;
		}else{
			String error = jo.getString("error");
//			Log.i("main", "error == " + error);
			
			Tiegao.setLandStatus(2);
			return;
		}
	}
	
	private String createLoginURL(OauthResultModel user) throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?access_token=");
		// 易接平台创建的游戏ID
		builder.append(user.getAccessToken());
		
		builder.append("&open_id=");
		builder.append(user.getOpenId());
		
//		Log.i("main", "<><> access_token == " + user.getAccessToken());
//		Log.i("main", "<><> open_id == " + user.getOpenId());
		
		return builder.toString();
	}
	
	
	
	
	
	// pay
	public void pay(int index) {
		String productId;
		if (index == 1) {
			wares_id = "1";
			productId = "tiegao_1";
		}else if (index == 2) {
			wares_id = "2";
			productId = "tiegao_2";
		}else if (index == 3) {
			wares_id = "3";
			productId = "tiegao_3";
		}else if (index == 4) {
			wares_id = "4";
			productId = "tiegao_4";
		}else if (index == 5) {
			wares_id = "5";
			productId = "tiegao_5";
		}else if (index == 6) {
			wares_id = "6";
			productId = "tiegao_6";
		}else if (index == 9) {
			wares_id = "9";
			productId = "tiegao_9";
		}
		else {
			wares_id = "1";
			productId = "tiegao_1";
		}
		
		// 测试用
//		wares_id = "7";
//		productId = "tiegao_7";
		
		Tiegao.setCpOrderId(productId + getUUID20());
		orderCheck(wares_id, Tiegao.getSidId(), 900 + "_" + open_id);
	}
	void orderCheck(final String wares_id, final String sid, final String uuid){
		Log.e("main", "<><> orderCheck wares_id: " + wares_id + ";sid: " + sid + ";uuid: " + uuid);
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					String url = LoginHelper.CP_PAY_SYNC_URL + createOrderURL(wares_id, sid, uuid);
					String result = LoginHelper.executeHttpGet(url);
 					Log.e("main", "<><> orderCheck result:" + result);
 					isOrderAnalyticString(result);
				} catch (Exception e) {
					Log.e("main", "<><> orderCheck ERROR: " + e.toString());
				}
			}
		}).start();
	}
	private String createOrderURL(String wares_id, String sid, String uuid) throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?wares_id=");
		builder.append(wares_id);
		
		builder.append("&sid=");
		builder.append(sid);
		
		builder.append("&uuid=");
		builder.append(uuid);
		
		return builder.toString();
	}
	public static void isOrderAnalyticString(String str)  throws Exception{
		JSONObject jo = new JSONObject(str);
		String code = jo.getString("code");
		trans_data = jo.getString("trans_data");//  "trans_data=" + 
		trans_secret = jo.getString("trans_secret");
		
//		Log.i("main","code == " + code);
//		Log.i("main","trans_data == " + trans_data);
//		Log.i("main","trans_secret == " + trans_secret);
//		
//		Log.i("main","<<<<<<<< tempError ===== " + str + " >>>>>>>>");
		if (code.equals("200")) {
			ispay();
			return;
		}else{
			
			return;
		}
		
	}
	
	public static void ispay(){
		Message msg = new Message();
		msg.arg1 = 1;
		payHandler.sendMessage(msg);
	}
	static Handler payHandler = new Handler(){
    	public void handleMessage(Message msg){
    		KKAgent.getInstance().startPay(instance, trans_data, trans_secret, new PayResultCallBack() {
	            @Override
	            public void onPayResult(int resultCode, String signvalue, String resultInfo) {
	            	if (resultCode == 0) {// 成功
	            		Tiegao.setSmsStatus(1);
//	            		Log.i("main", "支付成功");
	            	}else if (resultCode == 2) {// 取消
	            		Tiegao.setSmsStatus(2);
//	            		Log.i("main", "取消失败");
	            	}else{// 失败
	            		Tiegao.setSmsStatus(2);
//	            		Log.i("main", "支付失败");
	            	}
	            }
	        });
    	}
 	};

	
 	public static String getUUID20() {  
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
        
        return new String(out, 0, 20);  
    } 
	
	
}







