package com.mzplay.tiegao.baidu;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.Toast;

import com.snowfish.cn.ganga.helper.SFOnlineUser;
/*  Class LoginHelper 该类用来管理登陆状态以及登陆账户信息
*/
public class LoginHelper {
	
	public class OderId {
		public int retry;
		public String orderId;
	}
    //自建服务器
	static public final String CP_LOGIN_CHECK_URL = "http://nixi.yunlookgame.cn:9765/yijie/login"; // 59
//	static public final String CP_LOGIN_CHECK_URL = "http://tiegao.yunlookgame.cn:9765/yijie/login"; // 170
	
	
	/*CP服务器地址，支付结果同步地址
	 * 如果客户端不设置，将以在易接后台创建游戏时设置的数据同步地址进行同步
	 * */
	static public final String CP_PAY_SYNC_URL = "http://nixi.yunlookgame.cn:9765/purchase/yijie/callback"; // 59
//	static public final String CP_PAY_SYNC_URL = "http://tiegao.yunlookgame.cn:9765/purchase/yijie/callback"; // 170
	
	
	public static final boolean DEBUG	= false;
    private static LoginHelper instance = null;
    static SFOnlineUser user = null;
    static boolean bLogined = false;
    boolean isQuerying = false;
    Object lock = new Object();
	List<OderId> orderIds = new ArrayList<OderId>();
	static Handler handler  = null;
	static String message = "";
	
	public static LoginHelper instance() {
		if (instance == null) {
			try {
				instance = new LoginHelper();
			} catch (Throwable e) {
				new Exception("don't find the necessary configuration file").printStackTrace();
			}
		}
		return instance;
	}
	public boolean isLogin(){
		return bLogined;
	}
	public void setLogin(boolean login){
		bLogined = login;
	}
	public SFOnlineUser getOnlineUser(){
		return user;
	}
	public void setOnlineUser(SFOnlineUser u){
		user = u;
	}
	public boolean isDebug(){
		return DEBUG;
	}
	
	static public void showMessage(String msg, final Context context) {
		message = msg;
		prepareHandler(context);
		handler.sendEmptyMessage(1);
	}
	
	static void prepareHandler(final Context context) {
		if (handler == null) {
			handler = new Handler() {
				@Override
				public void handleMessage(Message msg) {
					if (msg.what == 1) {
						Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
					}
				}
			};
		}
	}
	
	static public String executeHttpGet(String str) {
		String result = null;
		URL url = null;
		HttpURLConnection connection = null;
		InputStreamReader in = null;
		try {
			url = new URL(str);
			connection = (HttpURLConnection) url.openConnection();
			connection.setReadTimeout(5000);
			in = new InputStreamReader(connection.getInputStream());
			BufferedReader bufferedReader = new BufferedReader(in);
			StringBuffer strBuffer = new StringBuffer();
			String line = null;
			while ((line = bufferedReader.readLine()) != null) {
				strBuffer.append(line);
			}
			result = strBuffer.toString();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (connection != null) {
				connection.disconnect();
			}
			if (in != null) {
				try {
					in.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			
		}
		return result;
	}
	public Handler getHandler(Context context){
		prepareHandler(context);
		return handler;
	}
}
