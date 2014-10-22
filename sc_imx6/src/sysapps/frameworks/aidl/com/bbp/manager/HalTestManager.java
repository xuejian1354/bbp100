package com.bbp.manager;

import android.os.RemoteException;
import android.os.ServiceManager;
import android.util.Log;

public class HalTestManager {

	private static final String TAG = "HalTestManager";
	private IHalTestService mHalTestService;
	
	public HalTestManager() {
		mHalTestService = IHalTestService.Stub.asInterface(ServiceManager.getService("haltest"));
	
		if (mHalTestService != null) {
			Log.i(TAG, "The HalTestManager object is ready");
		}
	}
	
	public boolean HalTestPrint() {
		boolean result = false;
		try {
			result = mHalTestService.HalTestPrint();
		} catch (RemoteException e) {
			// TODO: handle exception
			Log.i(TAG, "RemoteException in HalTestManager");
		}
		return result;
	}
	
}
