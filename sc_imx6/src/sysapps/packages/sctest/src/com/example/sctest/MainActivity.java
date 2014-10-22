package com.example.sctest;

import com.bbp.manager.HalTestManager;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.widget.Button;

public class MainActivity extends Activity implements View.OnClickListener {

	private HalTestManager mHalTestManager = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        startService(new Intent("com.bbp.server.HalTestSystemServer"));
        
        Button btn = (Button)findViewById(R.id.hello_btn);
        btn.setOnClickListener(this);
    }

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if (mHalTestManager == null) {
			mHalTestManager = new HalTestManager();
		}
		
		mHalTestManager.HalTestPrint();
	}
    
    
}
