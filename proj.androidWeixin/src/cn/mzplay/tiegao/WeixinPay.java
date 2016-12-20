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
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;

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
				resultunifiedorder = new HashMap<String, String>();
				resultunifiedorder = GsonUtil.fromJson(tempError[1], Map.class);
				
				weChatPay();
            } catch (Exception e) {
                e.printStackTrace();
            }
			return;
		}else{
			if (Tiegao.getSmsStatus() == 1) {
				Tiegao.setSmsStatus(1);
			}else{
				Tiegao.setSmsStatus(2);
			}
			Toast.makeText(instance, "支付失败", Toast.LENGTH_LONG).show();
			
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
		}else {
			Toast.makeText(instance, "请重新支付", Toast.LENGTH_SHORT).show();
		}
				
		payRequest.timeStamp = String.valueOf(genTimeStamp());
		
		Log.i("main", "<><><><><> payRequest.timeStamp == " + payRequest.timeStamp);
		
		// 本地再次签名
		SortedMap<Object,Object> signParams = new TreeMap<Object,Object>();
		signParams.put("appid", "" + payRequest.appId);  
		signParams.put("noncestr", "" + payRequest.nonceStr);  
		signParams.put("package", "" + payRequest.packageValue);  
		signParams.put("partnerid", "" + payRequest.partnerId);  
		signParams.put("prepayid", "" + payRequest.prepayId);
		signParams.put("timestamp", "" + payRequest.timeStamp);
        payRequest.sign = createSign(signParams, "UTF-8", resultunifiedorder.get("otherId"));
        payRequest.extData = "app data";
        Log.e("main", "<><><> sign === " + payRequest.sign);
        
		// 最后发起请求就可以了
        msgApi.registerApp(APP_ID);
        msgApi.sendReq(payRequest);
	}
	
	/**
     * 微信支付签名算法sign
     * @param characterEncoding
     * @param parameters
     * @return
     */
    @SuppressWarnings("unchecked")
    public static String createSign(SortedMap<Object,Object> parameters, String characterEncoding, String otherId){
        StringBuffer sb = new StringBuffer();
        Set es = parameters.entrySet();//所有参与传参的参数按照accsii排序（升序）
        Iterator it = es.iterator();
        while(it.hasNext()) {
            Map.Entry entry = (Map.Entry)it.next();
            String k = (String)entry.getKey();
            Object v = entry.getValue();
            if(null != v && !"".equals(v)
                    && !"sign".equals(k) && !"key".equals(k)) {
                sb.append(k + "=" + v + "&");
            }
        }
        sb.append("key=" + otherId);
        Log.e("main", "<><><> sb === " + sb.toString());
        String sign = MD5Util.MD5Encode(sb.toString(), characterEncoding).toUpperCase();  
        
        return sign;
    }
	
	
}
