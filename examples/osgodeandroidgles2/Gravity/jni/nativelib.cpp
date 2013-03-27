/*!
 * @file nativelib.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include "Application.hpp"

#include <jni.h>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
extern "C" {
	void	Java_com_testosgode_gravity_NativeLib_init(JNIEnv* env, jobject self, float w, float h) ;
	void	Java_com_testosgode_gravity_NativeLib_frame(JNIEnv* env, jobject self) ;
	void	Java_com_testosgode_gravity_NativeLib_quit(JNIEnv* env, jobject self) ;
	void	Java_com_testosgode_gravity_NativeLib_setGravity(JNIEnv* env, jobject self, float x, float y, float z) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Java_com_testosgode_gravity_NativeLib_init(JNIEnv* env, jobject self, float w, float h)
{
	Application::instance().init(w, h) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Java_com_testosgode_gravity_NativeLib_frame(JNIEnv* env, jobject self)
{
	Application::instance().frame() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Java_com_testosgode_gravity_NativeLib_quit(JNIEnv* env, jobject self)
{
	Application::instance().quit() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Java_com_testosgode_gravity_NativeLib_setGravity(JNIEnv* env, jobject self, float x, float y, float z)
{
	Application::instance().setGravity(x, y, z) ;
}
/* ....................................................................... */
/* ======================================================================= */
