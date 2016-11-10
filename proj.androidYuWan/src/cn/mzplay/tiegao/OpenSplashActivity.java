package cn.mzplay.tiegao;

import java.util.Set;

import com.yuwan8.middleware.SplashActivity;

import android.content.Intent;
import android.os.Bundle;

public class OpenSplashActivity extends SplashActivity {

	private boolean flag = true;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
    	//注：需要把该方法中的代码复制到游戏继承闪屏的Activity的onCreate方法中，且该Activity为游戏的LaunchActivity（游戏入口）。
    	super.onCreate(savedInstanceState);
    	Intent intent = this.getIntent();
    	Set<String> set = intent.getCategories();
    	if(null == set){
    		finish();
    		return;
    	}

    	for (String category : set) {
    		if("android.intent.category.LAUNCHER".equals(category)){
    			flag = false;
    		}
    	}

    	if(flag){
    		finish();                     
    	}
	}

	@Override
	protected void onSplashStop() {
		// TODO Auto-generated method stub
		Intent intent = new Intent(OpenSplashActivity.this, Tiegao.class);
		startActivity(intent);
		finish(); 
	}

}