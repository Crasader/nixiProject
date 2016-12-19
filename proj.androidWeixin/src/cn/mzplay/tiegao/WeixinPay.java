package cn.mzplay.tiegao;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;

import com.tencent.mm.sdk.modelpay.PayReq;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.util.Log;
import android.widget.Toast;



public class WeixinPay {
	
	public static Activity instance;
	//appid 微信分配的公众账号ID
	public static final String APP_ID = "wxec273f8180ceeffb";
	
	public static IWXAPI msgApi;
	
	private static String productName;
	private static int price;
	
	private static Map<String,String> resultunifiedorder;
	   
	public WeixinPay(Activity act){
		instance = act;
		
		initSDK();
	}
	/**
	 * initSDK
	 * 初始化SDK
	 */
	private void initSDK() {
		msgApi = WXAPIFactory.createWXAPI(instance, null);
		msgApi.registerApp(APP_ID);
	}
	
	public static void LoginCheck(){
        /*建立HTTP Post连线*/
        HttpPost httpRequest = new HttpPost(LoginHelper.CP_ORDER_URL);
        //Post运作传送变数必须用NameValuePair[]阵列储存
        //传参数 服务端获取的方法为request.getParameter("name")
        List <NameValuePair> params = new ArrayList<NameValuePair>();
        Log.i("main", "<><><><><> IP === " + getHostIP());
        params.add(new BasicNameValuePair("ip", "" + getHostIP())); //对应参数 属性， value
        params.add(new BasicNameValuePair("product_id", "" + Tiegao.getProductId()));
        params.add(new BasicNameValuePair("sid", "" + Tiegao.getUserId()));
        Log.i("main","<<<<<<<< params >>>>>>>>");
        try{
        	
        	//发出HTTP request
        	httpRequest.setEntity(new UrlEncodedFormEntity(params,HTTP.UTF_8));
        	//取得HTTP response
        	HttpResponse httpResponse = new DefaultHttpClient().execute(httpRequest);
        	Log.i("main","<<<<<<<< HttpResponse >>>>>>>>");
            if(httpResponse != null){
            	Log.i("main","<<<<<<<< Send the statistics of success >>>>>>>>");
            }else{
            	Log.i("main","<<<<<<<< Send failure statistic data >>>>>>>>");
            }
            InputStream reader = httpResponse.getEntity().getContent();
            if (reader != null) {
            	Log.i("main","<<<<<<<< Receive the server data successfully >>>>>>>>");
            	String aa = convertStreamToString(reader);
            	isAnalyticString(aa);
            }else {
            	
            	Log.i("main","<<<<<<<< Receive data server failure >>>>>>>>");
            }
        }catch(ClientProtocolException e){
        	
        	e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
        	
        	e.printStackTrace();
        } catch (IOException e) {
        	
        	e.printStackTrace();
        }
    }
	public static String convertStreamToString(InputStream is) {
		/**
		 * 将InputStream到字符串我们使用BufferedReader.readLine()
		 * 方法。我们迭代直到BufferedReader返回null,这意味着
		 * 没有更多的数据来读。每一行将附加到一个StringBuilder
		 * 和返回字符串。
		 */
		BufferedReader reader = new BufferedReader(new InputStreamReader(is));
		StringBuilder sb = new StringBuilder();
		String line = null;
		try {
			while ((line = reader.readLine()) != null) {
				sb.append(line);//+ "\n"
			}
		}catch (IOException e) {
			
			e.printStackTrace();
		}finally {
			try {
				is.close();
			}catch (IOException e){
				
				e.printStackTrace();
			}
		}
		String isReader = sb.toString();
		Log.e("main", "<<<<<<<< isReadersb.toString() ======== " + isReader + " >>>>>>>>");
		return isReader;
	}
	
	public static void isAnalyticString(String str) {
		String[] tempError = null;
		tempError = str.split(";");
		
		Log.i("main","<<<<<<<< tempError ===== " + tempError + " >>>>>>>>");
		Log.i("main", "<><><><><> tempError0 == " + tempError[0]);
		if (tempError[0].equals("200")) {
			try {
//				Tiegao.setSessionid(tempError[1]);
//				Tiegao.setLandStatus(1);
//				
//				LoginHelper.showMessage("登录成功", instance);
				
//				200;
//				"otherId":"yi34mv5n2qgxbrblzafv1bg6hlv9w6ow",
//				"return_code":"SUCCESS",
//				"return_msg":"OK",
//				"appid":"wxec273f8180ceeffb",
//				"mch_id":"1421934502",
//				"nonce_str":"A7PIXmGQm7Q29UnW",
//				"sign":"2459ED36889E9C8F9F03A248A2110519",
//				"result_code":"SUCCESS",
//				"prepay_id":"wx2016121903052715beaaea2e0071969432",
//				"trade_type":"APP"
				
//				"otherId":"yi34mv5n2qgxbrblzafv1bg6hlv9w6ow",
//				"return_code":"SUCCESS",
//				"return_msg":"OK",
//				"appid":"wxec273f8180ceeffb",
//				"mch_id":"1421934502",
//				"nonce_str":"DYmNvJztimO2Swgt",
//				"sign":"E7B7729B5EF43BE3E87A488CDAF3042E",
//				"result_code":"SUCCESS",
//				"prepay_id":"wx20161219170927e5c343154f0810594117",
//				"trade_type":"APP"
				
				resultunifiedorder = new HashMap<String, String>();
				resultunifiedorder = GsonUtil.fromJson(tempError[1], Map.class);
//				String partnerid = maps.get("mch_id");							// partnerid 
//				String prepayid = maps.get("prepay_id");						// prepayid
//				String noncestr = maps.get("nonce_str");						// noncestr
//				String otherId = maps.get("otherId");
//				Log.i("main", "<><><><><> partnerid == " + partnerid);
//				Log.i("main", "<><><><><> prepayid == " + prepayid);
//				Log.i("main", "<><><><><> noncestr == " + noncestr);
//				Log.i("main", "<><><><><> otherId == " + otherId);
				
				weChatPay();
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
//			Tiegao.setLandStatus(2);
//			LoginHelper.showMessage("未登录", instance);
			return;
		}
	}
	
	
	
	
	

	/** 
	 * 获取ip地址 
	 * @return 
	 */  
	public static String getHostIP() {  
	  
	    String hostIp = "127.0.0.1";
	    try {  
	        Enumeration nis = NetworkInterface.getNetworkInterfaces();  
	        InetAddress ia = null;  
	        while (nis.hasMoreElements()) {  
	            NetworkInterface ni = (NetworkInterface) nis.nextElement();  
	            Enumeration<InetAddress> ias = ni.getInetAddresses();  
	            while (ias.hasMoreElements()) {  
	                ia = ias.nextElement();  
	                if (ia instanceof Inet6Address) {  
	                    continue;// skip ipv6  
	                }  
	                String ip = ia.getHostAddress();  
	                if (!"127.0.0.1".equals(ip)) {  
	                    hostIp = ia.getHostAddress();  
	                    break;  
	                }  
	            }  
	        }  
	    } catch (SocketException e) {  
	        Log.i("yao", "SocketException");  
	        e.printStackTrace();  
	    }  
	    return hostIp;  
	  
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
		Log.e("main", "<><> productName == " + productName);
		
//		String mzStr = Tiegao.getProductId() + ";" + Tiegao.getSidId();
//		String mzStr = Tiegao.getProductId() + "";
		
		LoginCheck();
	}
	
	
	private static long genTimeStamp() {
		return System.currentTimeMillis() / 1000;
	}
	public static void weChatPay(){		
		PayReq payRequest = new PayReq();
		payRequest.appId = APP_ID;
		if (resultunifiedorder != null) {
            payRequest.nonceStr = resultunifiedorder.get("nonce_str");
			payRequest.partnerId = resultunifiedorder.get("mch_id");
			payRequest.prepayId = resultunifiedorder.get("prepay_id");
			payRequest.packageValue = "Sign=WXPay";
//			payRequest.packageValue = "prepay_id=" + resultunifiedorder.get("prepay_id");
			
		}else {
			Toast.makeText(instance, "请重新支付", Toast.LENGTH_SHORT).show();
		}
				
		payRequest.timeStamp = String.valueOf(genTimeStamp());;
		Log.i("main", "<><><><><> payRequest.timeStamp == " + payRequest.timeStamp);
		
		
		// 本地再次签名
        List<NameValuePair> signParams = new LinkedList<NameValuePair>();
        signParams.add(new BasicNameValuePair("appid", payRequest.appId));
        signParams.add(new BasicNameValuePair("noncestr", payRequest.nonceStr));
        signParams.add(new BasicNameValuePair("package", payRequest.packageValue));
        signParams.add(new BasicNameValuePair("partnerid", payRequest.partnerId));
        signParams.add(new BasicNameValuePair("prepayid", payRequest.prepayId));
        signParams.add(new BasicNameValuePair("timestamp", payRequest.timeStamp));
//        payRequest.sign = genAppSign(signParams, resultunifiedorder.get("otherId"));
//        APPKey:				yi34mv5n2qgxbrblzafv1bg6hlv9w6ow
        payRequest.sign = genAppSign(signParams, "yi34mv5n2qgxbrblzafv1bg6hlv9w6ow");
        payRequest.extData = "app data";
        
		// 最后发起请求就可以了
        msgApi.registerApp(APP_ID);  
        msgApi.sendReq(payRequest);
	}
	private static String genAppSign(List<NameValuePair> params, String otherId) {
		StringBuilder sb = new StringBuilder();
		
		for (int i = 0; i < params.size(); i++) {
			sb.append(params.get(i).getName());
			sb.append('=');
			sb.append(params.get(i).getValue());
			sb.append('&');
		}
		sb.append("key=");
		sb.append(otherId);
		
		sb.append("sign str\n" + sb.toString() + "\n\n");
		String appSign = MD5.getMessageDigest(sb.toString().getBytes());
		Log.e("orion", appSign);
		return appSign;
	}
	
	
}
