/*!
 * @file ControllerBase.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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
#include <osgODEUtil/ControllerBase>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
ControllerBase::ControllerBase(void):
    m_on_key_pressed(   new Signal3<const int, const int, bool>() ),
    m_on_key_released(  new Signal3<const int, const int, bool>() )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ControllerBase::ControllerBase(const ControllerBase& other, const osg::CopyOp& copyop):
    osgGA::GUIEventHandler(other, copyop),

    m_on_key_pressed(   other.m_on_key_pressed  ),
    m_on_key_released(  other.m_on_key_released )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ControllerBase::~ControllerBase(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
ControllerBase::handle( const osgGA::GUIEventAdapter&   ea,
                        osgGA::GUIActionAdapter&        aa,
                        osg::Object*                    obj,
                        osg::NodeVisitor*               nv)
{
	(void) nv ;
	(void) obj ;
	(void) aa ;


    const int   key = ea.getKey() ;
    const int   mod_key = ea.getModKeyMask() ;
    bool        handled = false ;

    switch( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
            if( ! m_key_down[key] ) {
                m_on_key_pressed->emit( key, mod_key, handled ) ;
                m_key_down[key] = true ;
            }
        break ;




        case osgGA::GUIEventAdapter::KEYUP:
            m_on_key_released->emit( key, mod_key, handled ) ;
            m_key_down[key] = false ;
        break ;




        default:
        break ;
    }



    return handled ;
}
/* ....................................................................... */
/* ======================================================================= */
