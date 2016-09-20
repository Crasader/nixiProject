package cn.mzplay.tiegao.meitu;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;

import com.weedong.gamesdk.base.WdGameSDK;
import com.weedong.gamesdk.base.WdResultCode;
import com.weedong.gamesdk.bean.OrderInfo;
import com.weedong.gamesdk.bean.PayInfo;
import com.weedong.gamesdk.bean.UserInfo;
import com.weedong.gamesdk.listener.WdLoginListener;
import com.weedong.gamesdk.listener.WdPayListener;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class MeituLayer {
	public static Activity instance;
	
	
	private static String productName;
	private static String uid;
	private static String sessionid;
	private static String cp_order_id;
	private static int price;
	
	public MeituLayer(Activity act){
		instance = act;
		
		initSDK();
	}
	
	/**
	 * initSDK
	 * 初始化SDK
	 */
	private void initSDK() {
		WdGameSDK.getInstance().initialize(instance, "745", "yWpx3hWQHFhSnTCj#745#6KuRKuaAjLJ5sYRy", WdGameSDK.PORTRAIT);
	}
	
	/*易接登陆接口*/
	void doLogin(int type){
		// 调用登录接口。注意,该方法必须在应用的主线程中调用 
		WdGameSDK.getInstance().login(instance, new WdLoginListener(){

			@Override
			public void onCallBack(int paramInt, UserInfo paramResponse) {
				// TODO Auto-generated method stub
				if (paramInt == WdResultCode.LOGIN_SUCCESS) {
//					Log.d("main", "登录成功! 用户名:"+ paramResponse.getUname());
					
					WdGameSDK.getInstance().createGameBar(instance);
					WdGameSDK.getInstance().uploadGameInfo("美图android大区");
					
					uid = paramResponse.getUid();
					sessionid = paramResponse.getSessionid();
//					Log.i("main", "uid === " + uid);
//					Log.i("main", "sessionid === " + sessionid);
					getSendEvent();
					
				} else if(paramInt == WdResultCode.LOGIN_CANCEL){
					Log.d("main", "登录取消"); 
					Tiegao.setLandStatus(2);
				}else {
					Log.d("main", "登录失败"); 
					Tiegao.setLandStatus(2);
				}
			}
		});
	}
	
	
	
	/**
     *  LoginCheck
     *  从服务器端验证用户是否登陆
     * @param user 登陆账户
     */
	public void getSendEvent() {
		
//		Log.e("main", "<<<<<<<< LoginCheck user 1 >>>>>>>> ");
		new Thread(new Runnable() {
			@Override
			public void run() { 
				try {
					String url = LoginHelper.CP_LOGIN_CHECK_URL + createLoginURL();
					String result = LoginHelper.executeHttpGet(url);
// 					Log.e("main", "<<<<<<<< LoginCheck result:" + result);
 					isAnalyticString(result);
				} catch (Exception e) {
//					Log.e("main", "<<<<<<<< LoginCheck ERROR: " + e.toString());
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
				
				LoginHelper.showMessage("登录成功", instance);
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
			Tiegao.setSmsStatus(2);
			
			LoginHelper.showMessage("未登录", instance);
			return;
		}
	}
	private String createLoginURL() throws UnsupportedEncodingException {
		StringBuilder builder = new StringBuilder();
		builder.append("?uid=");
		// 易接平台创建的游戏ID
		builder.append(uid);
		builder.append("&sessionid=");
		// 易接平台标示的渠道SDK ID
		builder.append(sessionid);
		return builder.toString();
	}
	
	
	
	
	public void pay(int index) {
		productName = null;
		price = 0;
		
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
		}
		
		price = Tiegao.getMoneyStatus();
//		price = 1;
//		Log.e("main", "<><> productName == " + productName);
		
		cp_order_id = Tiegao.getProductId() + ";" + Tiegao.getSidId();
//		Log.e("main", "<><> cp_order_id == " + cp_order_id);
		
		
		PayInfo orderInfo = new PayInfo(); 
		orderInfo.setCpOrderId(cp_order_id); // cp_order_id(不能为空)
		orderInfo.setCpServerId("1"); // 默认值
		orderInfo.setAmount(price); // amount 参数的单位是游戏货币单位,我方服务器 配置的默认比例为 10:1,
		WdGameSDK.getInstance().payment(instance, orderInfo, new WdPayListener<OrderInfo>() {
			
			@Override
			public void onCallBack(int paramInt, OrderInfo paramResponse) {
				// TODO Auto-generated method stub
				if (paramInt == WdResultCode.PAY_SUCCESS) {
					// 如果支付成功,接下去需要到自己的服务器查询订单结果
					Log.d("main", "支付成功");
					
					Tiegao.setCpOrderId(paramResponse.getOrderid());
					Tiegao.setSmsStatus(1);
					
				} else if(paramInt == WdResultCode.PAY_CANCEL){ 
					Log.d("main", "支付取消");
					Tiegao.setSmsStatus(2);
				}else if(paramInt == WdResultCode.PAY_DEBUG){ 
					Log.d("main", "测试环境下,支付成功");
					
//					Log.d("main", "<><><><> paramResponse === " + paramResponse);
					
					Tiegao.setCpOrderId(paramResponse.getOrderid());
					Tiegao.setSmsStatus(1);
				}else{
					Log.d("main", "支付失败");
					Tiegao.setSmsStatus(2);
				}
			}
		});
	}
	
	// 随机生成 ， cp请自己生成订单号
	String getCpOrderId() {
		UUID guid = UUID.randomUUID();
		String cooOrderSerial = guid.toString();
		cooOrderSerial = cooOrderSerial.replace("-", "".trim());
		Log.d("main", "" + cooOrderSerial);
		return cooOrderSerial;
	}
	
}
