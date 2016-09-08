package cn.mzplay.tiegao;

import java.util.Calendar;

import android.R.string;
import android.app.Activity;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Intent;
import android.util.Log;

public class AlarmClock {
	public static Activity instance;
	
	private static final int INTERVAL = 1000 * 60 * 60 * 24;// 24h
	
	static int _hour = 0;
	static int _minute = 0; 
	static int _second = 0;
	
	public AlarmClock(Activity act){
		instance = act;
		
	}
	
	
	public static void push_Android(string nameStr, int num, int index){
		setTime(num);
		if (index == 1) {
			creatAlarmClock1(nameStr, index);
		}else if (index == 2) {
			creatAlarmClock2(nameStr, index);
		}
		
	}
	
	public static void creatAlarmClock1(string nameStr, int index){
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
	

	
	public static void creatAlarmClock2(string nameStr, int index){
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
	
	public static void closePush_Android(int index){
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
