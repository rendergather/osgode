package com.testosgode.gravity;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

public class MyGLRenderer implements GLSurfaceView.Renderer {
	
	public void onDrawFrame(GL10 gl) {
		NativeLib.instance().frame();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		NativeLib.instance().init(w,  h) ;
	} 

	public void onSurfaceCreated(GL10 gl, EGLConfig conf) {
	}
}