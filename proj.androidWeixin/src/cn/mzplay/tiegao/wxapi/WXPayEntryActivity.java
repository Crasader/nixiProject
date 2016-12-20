package cn.mzplay.tiegao.wxapi;


import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import cn.mzplay.tiegao.R;
import cn.mzplay.tiegao.WeixinPay;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler{
	
	private static final String TAG = "MicroMsg.SDKSample.WXPayEntryActivity";
	
    private IWXAPI api;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.pay_result);
        
        Log.i("main", "WXPayEntryActivity  onCreate");
    	api = WXAPIFactory.createWXAPI(this, WeixinPay.APP_ID);
        api.handleIntent(getIntent(), this);
    }

	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		Log.i("main", "WXPayEntryActivity  onNewIntent, onCreate");
		setIntent(intent);
        api.handleIntent(intent, this);
	}

	@Override
	public void onReq(BaseReq req) {
		Log.i("main", "WXPayEntryActivity  onReq, onCreate");
		Log.i("main", "WXPayEntryActivity  onReq  req == " + req);
	}

	@Override
	public void onResp(BaseResp resp) {
		Log.i("main", "WXPayEntryActivity  onPayFinish, errCode = " + resp.errCode);
		Log.i("main", "WXPayEntryActivity  onPayFinish, resp = " + resp);
		
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			AlertDialog.Builder builder = new AlertDialog.Builder(this);
			builder.setTitle(R.string.app_tip);
			builder.setMessage(getString(R.string.pay_result_callback_msg, String.valueOf(resp.errCode)));
			builder.show();
		}
	}
}