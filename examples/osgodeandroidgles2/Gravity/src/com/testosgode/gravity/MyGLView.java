package com.testosgode.gravity;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLView extends GLSurfaceView {
	
	MyGLRenderer	m_renderer ;

	public MyGLView(Context context) {
		super(context);
		setEGLContextClientVersion(2) ;
		m_renderer = new MyGLRenderer() ;
		setRenderer(m_renderer) ;
	}
}
