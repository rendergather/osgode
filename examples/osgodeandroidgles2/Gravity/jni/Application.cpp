/*!
 * @file Application.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include "Application.hpp"
#include "Scene.hpp"
#include "Viewer.hpp"

#include <osgViewer/Viewer>

#include <osgODE/Manager>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */









/* ======================================================================= */
/* ....................................................................... */
Application::Application(void)
{
	osg::initNotifyLevel() ;

#ifdef ANDROID
    psInstallAndroidNotifyHandler() ;
#endif
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Application::Application(const Application& other)
{
    if( other.m_viewer.valid() ) {

        m_viewer = new osgViewer::Viewer( *other.m_viewer ) ;

        m_manager = other.m_manager.get() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Application::~Application(void)
{
    m_viewer = NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Application::init(float w, float h)
{
    PS_DBG("Application::init(%p)", this) ;

    m_viewer = new osgViewer::Viewer() ;
    PS_ASSERT1( m_viewer.valid() ) ;


    // Scene.cpp
    osgODE::Manager*    manager = createScene() ;
    PS_ASSERT1(manager) ;

    // Viewer.cpp
    setupViewer(m_viewer.get(), manager, w, h) ;

    // adesso posso associare il manager all'observer perché ha un
    // riferimento come scene data del viewer
    m_manager = manager ;

    // sono paranoico...
    PS_ASSERT1(m_manager.valid()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Application::frame(void)
{
    PS_DBG3("Application::frame(%p)", this) ;

    PS_ASSERT1( m_viewer.valid() ) ;
    m_viewer->frame() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Application::quit(void)
{
    PS_DBG("Application::quit(%p)", this) ;

    m_viewer = NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Application::done(void)
{
    PS_ASSERT1( m_viewer.valid() ) ;

    return m_viewer->done() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Application::setGravity(float x, float y, float z)
{
    PS_DBG3("Application::setGravity(%p, x=%f, y=%f, z=%f", this, x, y, z) ;

//    PS_ASSERT1( m_manager.valid() ) ;

	if( m_manager.valid() ) {
		m_manager->getWorld()->writeLock() ;
		m_manager->getWorld()->setGravity( osg::Vec3(x, y, z) ) ;
		m_manager->getWorld()->writeUnlock() ;
	} else {
		PS_FATAL("Application::setGravity(%p, ...): m_manager.valid() == false", this) ;
	}
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Application&
Application::instance(void)
{
    static Application  application ;

    return application ;
}
/* ....................................................................... */
/* ======================================================================= */
