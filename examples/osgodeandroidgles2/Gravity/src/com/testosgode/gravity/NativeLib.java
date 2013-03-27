package com.testosgode.gravity;

public class NativeLib {
	
	
	private static final NativeLib	m_instance = new NativeLib() ;
	
	private NativeLib() {
		super() ;
	}

	public native void		init(float w, float h) ;
	public native void		frame() ;
	public native void		quit() ;
	public native void		setGravity(float x, float y, float z) ;


	public static NativeLib	instance() {
		return m_instance ;
	}

	
	
	static {
		System.loadLibrary("nativelib") ;
	}

}
