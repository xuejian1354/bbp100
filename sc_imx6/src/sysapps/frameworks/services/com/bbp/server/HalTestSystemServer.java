package com.bbp.server;

import android.app.Service;
import android.content.Intent;
import android.os.ServiceManager;
import android.os.IBinder;

public class HalTestSystemServer extends Service{

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onStart(Intent intent, int startId) {
		// TODO Auto-generated method stub
		super.onStart(intent, startId);
		try {
			ServiceManager.addService("haltest", new HalTestService());
		} catch (RuntimeException e) {
			// TODO: handle exception
		}
	}

}
