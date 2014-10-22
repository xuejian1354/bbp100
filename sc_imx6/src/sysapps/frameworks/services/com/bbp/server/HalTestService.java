package com.bbp.server;

import com.bbp.manager.IHalTestService;
import android.os.RemoteException;

public class HalTestService extends IHalTestService.Stub{

	static {
		System.load("/system/lib/libhardware_bbp.so");
	}
	
	@Override
	public boolean HalTestPrint() throws RemoteException {
		// TODO Auto-generated method stub
		return haltest_print();
	}
	
	private static native boolean haltest_print();
}
