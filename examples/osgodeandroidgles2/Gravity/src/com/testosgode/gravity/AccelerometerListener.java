package com.testosgode.gravity;

import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class AccelerometerListener implements SensorEventListener {
	
	private SensorManager	m_sensor_manager ;
	
	
	static final AccelerometerListener	m_instance = new AccelerometerListener() ;
	
	
	private AccelerometerListener() {
	}
	
	
	public static AccelerometerListener	instance() {
		return m_instance ;
	}
	
	
	public void onCreate(Activity activity) {
		m_sensor_manager = (SensorManager) activity.getSystemService(Activity.SENSOR_SERVICE) ;
	}
	
	
	
	
	public void onResume() {
		Sensor	accelerometer = m_sensor_manager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) ;
		m_sensor_manager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_GAME) ;
	}
	
	
	
	
	public void onPause() {
		m_sensor_manager.unregisterListener(this) ;
	}
	
	

	public void onAccuracyChanged(Sensor sensor, int accuracy) {
		
	}

	
	
	public void onSensorChanged(SensorEvent event) {

		if( event.sensor.getType() == Sensor.TYPE_ACCELEROMETER ) {

			float	x = - event.values[0] ;
			float	y = - event.values[1] ;
			float	z = - event.values[2] ;
			
			NativeLib.instance().setGravity(x, y, z) ;
		}
		
	}

}
