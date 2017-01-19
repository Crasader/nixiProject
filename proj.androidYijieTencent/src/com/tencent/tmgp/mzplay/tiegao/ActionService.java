package com.tencent.tmgp.mzplay.tiegao;



import com.tencent.tmgp.mzplay.tiegao.R;

import android.app.Notification;
import android.app.Notification.Builder;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;


public class ActionService extends Service {
	private static int index = 0;
	
	MediaPlayer mediaPlayer = null;
    int pet_id = 0;
    
	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		
		String str = intent.getStringExtra("index");
		int _index = Integer.valueOf(str).intValue();
		if (null != intent.getAction()) {
			if ("naozhong1".equals(intent.getAction())) {
				index = 1;
				mediaPlayer = MediaPlayer.create(this, R.raw.push_1);
			}else if ("naozhong2".equals(intent.getAction())) {
				index = 2;
				mediaPlayer = MediaPlayer.create(this, R.raw.push_1);
			}
			mediaPlayer.start();
			setTuiSong(_index);
		}
		
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		return super.onStartCommand(intent, flags, startId);
	}
	
	public void setTuiSong(final int index){
		String titleStr1 = "";
		String titleStr2 = "";
		if (index == 1) {
			titleStr1 = "免费抽奖";
			titleStr2 = "可以免费抽奖!可以免费抽奖!可以免费抽奖!重要的提示要说三遍~!";
		}else if (index == 2) {
			titleStr1 = "免费抽奖";
			titleStr2 = "可以免费抽奖!可以免费抽奖!可以免费抽奖!重要的提示要说三遍~!";
		}
//		Log.i("main", "<><><><> naozhong4 _ 4");
		
//		//实例化通知栏构造器
//		Notification notification = new Notification();
//		
//		Bitmap btm = BitmapFactory.decodeResource(getResources(), R.drawable.icon);
//		//对Builder进行配置
//		Notification.Builder mBuilder = new Notification.Builder(ActionService.this)
//		.setContentTitle(titleStr1)//设置通知栏标题
//		.setContentText(titleStr1) //设置通知栏显示内容
//		.setTicker(titleStr2); //通知首次出现在通知栏,带上升动画效果的
//		mBuilder.setWhen(System.currentTimeMillis());//通知产生的时间,会在通知信息里显示,一般是系统获取到的时间
//		mBuilder.setPriority(Notification.PRIORITY_DEFAULT); //设置该通知优先级
//		mBuilder.setOngoing(false);//ture,设置他为一个正在进行的通知
//		mBuilder.setDefaults(Notification.DEFAULT_VIBRATE);//向通知添加声音、闪灯和振动效果的最简单、最一致的方式是使用当前的用户默认设置,使用defaults属性,可以组合
//		//Notification.DEFAULT_ALL  Notification.DEFAULT_SOUND 添加声音 // requires VIBRATE permission
//		mBuilder.setLargeIcon(btm);//设置通知小ICON
//		mBuilder.setVibrate(new long[] {0,300,500,700});//设置震动方式
//		
//		
//		//设置通知显示的参数
//		Intent intent = new Intent(ActionService.this,Mm3c.class);
//		PendingIntent m_PendingIntent = PendingIntent.getActivity(ActionService.this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
//		// 设置通知主题的意图
//		mBuilder.setContentIntent(m_PendingIntent);
//		//获取状态通知栏管理
//		NotificationManager notificationManager = (NotificationManager)getSystemService(ActionService.this.NOTIFICATION_SERVICE);
//		//这个可以理解为开始执行这个通知
//		notificationManager.notify(0, mBuilder.build());
		
		Intent intent = new Intent(ActionService.this, Tiegao.class);
		PendingIntent pintent = PendingIntent.getActivity(ActionService.this, 0, intent, 0);
		Builder builder = new Notification.Builder(ActionService.this);
		builder.setSmallIcon(R.drawable.icon);//设置图标
		builder.setTicker(titleStr1);//手机状态栏的提示；
		builder.setWhen(System.currentTimeMillis());//设置时间
		builder.setContentTitle(titleStr1);//设置标题
		builder.setContentText(titleStr2);//设置通知内容
		builder.setContentIntent(pintent);//点击后的意图
		//  builder.setDefaults(Notification.DEFAULT_SOUND);//设置提示声音
		//  builder.setDefaults(Notification.DEFAULT_LIGHTS);//设置指示灯
		//  builder.setDefaults(Notification.DEFAULT_VIBRATE);//设置震动
		builder.setDefaults(Notification.DEFAULT_ALL);//设置震动
		Notification notification = builder.build();//4.1以上
		
		NotificationManager notificationManager = (NotificationManager)getSystemService(ActionService.this.NOTIFICATION_SERVICE);
		notificationManager.notify(0, notification);
	}
	
}
