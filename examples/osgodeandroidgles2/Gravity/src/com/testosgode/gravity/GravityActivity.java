package com.testosgode.gravity;

import android.app.Activity;
import android.os.Bundle;

public class GravityActivity extends Activity {

	
	MyGLView	m_view ;
	
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
 
        m_view = new MyGLView(this) ;
 
        setContentView(m_view) ;

        AccelerometerListener.instance().onCreate(this); 
        
    }
    
    
    @Override
    protected void onPause() {
        super.onPause() ;
        m_view.onPause() ;
        AccelerometerListener.instance().onPause(); 
    }
    
    
    @Override
    protected void onResume() {
        super.onResume() ;
        m_view.onResume() ;
        AccelerometerListener.instance().onResume() ;
    }
    
    
    @Override
    protected void onDestroy() {
    	super.onDestroy() ;
    	NativeLib.instance().quit() ;
    }
}