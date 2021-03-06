package com.tencent.tmgp.mzplay.tiegao;

import java.util.Calendar;

import android.R.string;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class AlarmClock {
	public static Activity instance;
	
	private static final int INTERVAL = 1000 * 60 * 60 * 24;// 24h
	
	static String _nameStr;
	
	static int _hour = 0;
	static int _minute = 0;
	static int _second = 0;
	
	public AlarmClock(Activity act){
		instance = act;
		
	}
	
	public static void push_Android(String str, int num, int index) {
		Message msg = new Message();
		msg.arg1 = num;
		msg.arg2 = index;
		_nameStr = str;
		consumeHandler.sendMessage(msg);
	}
	static Handler consumeHandler = new Handler(){
		public void handleMessage(Message msg){
			setTime(msg.arg1);
			push_methods(_nameStr, msg.arg1, msg.arg2);
		}
	};
	
	public static void push_methods(String nameStr, int num, int index){
		Log.i("main", "<><><> == " + nameStr);
		if (index == 1) {
			creatAlarmClock1(nameStr, index);
		}else if (index == 2) {
			creatAlarmClock2(nameStr, index);
		}else if (index == 3) {
			creatAlarmClock3(nameStr, index);
		}
	}
	
	public static void creatAlarmClock1(String nameStr, int index){
		Calendar calend = Calendar.getInstance();
		AlarmManager alarmManager = (AlarmManager)instance.getSystemService(instance.ALARM_SERVICE);
		Intent intent = new Intent(instance, ActionService.class);
		intent.setAction("naozhong1");
		intent.setFlags(index);
		intent.putExtra("index", ""+index);
		PendingIntent pendingIntent = PendingIntent.getService(instance, index, intent, Intent.FLAG_ACTIVITY_NEW_TASK);
		
		calend.setTimeInMillis(System.currentTimeMillis());
		
		calend.set(Calendar.HOUR_OF_DAY, _hour);
		
		calend.set(Calendar.MINUTE, _minute);
		calend.set(Calendar.SECOND, 0);
		calend.set(Calendar.MILLISECOND, 0);
		if (System.currentTimeMillis() > calend.getTimeInMillis()) {
			calend.set(Calendar.DAY_OF_YEAR, calend.get(Calendar.DAY_OF_YEAR) + 1);
		}
		alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, calend.getTimeInMillis(), INTERVAL, pendingIntent);
	}
	
	
	
	public static void creatAlarmClock2(String nameStr, int index){
		if (index != 0) {
			Calendar calend = Calendar.getInstance();
			AlarmManager alarmManager = (AlarmManager)instance.getSystemService(instance.ALARM_SERVICE);
			Intent intent = new Intent(instance, ActionService.class);
			intent.setAction("naozhong2");
			intent.setFlags(index);
			intent.putExtra("index", ""+index);
			PendingIntent pendingIntent = PendingIntent.getService(instance, index, intent, Intent.FLAG_ACTIVITY_NEW_TASK);
			
			calend.setTimeInMillis(System.currentTimeMillis());
			
			calend.set(Calendar.HOUR_OF_DAY, _hour);
			
			calend.set(Calendar.MINUTE, _minute);
			calend.set(Calendar.SECOND, 0);
			calend.set(Calendar.MILLISECOND, 0);
			if (System.currentTimeMillis() > calend.getTimeInMillis()) {
				calend.set(Calendar.DAY_OF_YEAR, calend.get(Calendar.DAY_OF_YEAR) + 1);
			}
			alarmManager.setRepeating(AlarmManager.RTC_WAKEUP, calend.getTimeInMillis(), INTERVAL, pendingIntent);
		}
	}
	
	
	public static void creatAlarmClock3(String nameStr, int index){
		if (index != 0) {
			Calendar calend = Calendar.getInstance();
			AlarmManager alarmManager = (AlarmManager)instance.getSystemService(instance.ALARM_SERVICE);
			Intent intent = new Intent(instance, ActionService.class);
			intent.setAction("naozhong3");
			intent.setFlags(index);
			intent.putExtra("index", ""+index);
			PendingIntent pendingIntent = PendingIntent.getService(instance, index, intent, 0);
			
			calend.setTimeInMillis(System.currentTimeMillis());
			calend.add(Calendar.SECOND, _minute);
			alarmManager.set(AlarmManager.RTC_WAKEUP, calend.getTimeInMillis(), pendingIntent);
		}
	}
	
	public static void closePush_Android(int index) {
		Message msg = new Message();
		msg.arg1 = index;
		closePushHandler.sendMessage(msg);
	}
	static Handler closePushHandler = new Handler(){
		public void handleMessage(Message msg){
			closePush_Methods(msg.arg1);
		}
	};
	public static void closePush_Methods(int index){
		if(index == 1){
			closeAlarmClock1(index);
		}else if (index == 2){
			closeAlarmClock2(index);
		}
	}
	
	public static void closeAlarmClock1(int index){
		if (index != 0) {
			AlarmManager alarmManager = (AlarmManager)instance.getSystemService(instance.ALARM_SERVICE);
			Intent intent = new Intent(instance, ActionService.class);
			PendingIntent pendingIntent = null;
			
			intent.setAction("naozhong1");
			pendingIntent = PendingIntent.getService(instance, index, intent, Intent.FLAG_ACTIVITY_NEW_TASK);
			alarmManager.cancel(pendingIntent);
		}
	}
	public static void closeAlarmClock2(int index){
		if (index != 0) {
			AlarmManager alarmManager = (AlarmManager)instance.getSystemService(instance.ALARM_SERVICE);
			Intent intent = new Intent(instance, ActionService.class);
			PendingIntent pendingIntent = null;
			
			intent.setAction("naozhong2");
			pendingIntent = PendingIntent.getService(instance, index, intent, Intent.FLAG_ACTIVITY_NEW_TASK);
			alarmManager.cancel(pendingIntent);
		}
	}
    
	
	public static void setTime(int num){
//		  long days = mss / ( 60 * 60 * 24);
//		  long hours = (mss % ( 60 * 60 * 24)) / (60 * 60);
//		  long minutes = (mss % ( 60 * 60)) /60;
//		  long seconds = mss % 60;
		_hour = (num % ( 60 * 60 * 24)) / (60 * 60);
		_minute = (num % ( 60 * 60)) /60;
		_second = num % 60;
	}
	
}
