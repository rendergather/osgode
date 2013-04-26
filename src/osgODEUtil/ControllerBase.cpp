/*!
 * @file ControllerBase.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2013 by Rocco Martino                            *
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
    m_on_key_released(  new Signal3<const int, const int, bool>() ),

    m_on_left_button(   new Signal2<const bool, bool>() ),
    m_on_middle_button( new Signal2<const bool, bool>() ),
    m_on_right_button(  new Signal2<const bool, bool>() ),

    m_on_mouse_moved( new Signal2<const osg::Vec2, bool>() ),

    m_on_frame( new Signal3<bool, osg::Vec2, bool>() ),

    m_button_mask(0),

    m_requesting_warp(false)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ControllerBase::ControllerBase(const ControllerBase& other, const osg::CopyOp& copyop):
    osgGA::GUIEventHandler(other, copyop),

    m_on_key_pressed(   other.m_on_key_pressed  ),
    m_on_key_released(  other.m_on_key_released ),

    m_on_left_button(   other.m_on_left_button ),
    m_on_middle_button( other.m_on_middle_button ),
    m_on_right_button(  other.m_on_right_button ),

    m_on_mouse_moved( other.m_on_mouse_moved ),

    m_on_frame( other.m_on_frame ),

    m_key_down( other.m_key_down ),
    m_button_mask( other.m_button_mask ),

    m_requesting_warp( other.m_requesting_warp )
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




        case osgGA::GUIEventAdapter::PUSH:
        case osgGA::GUIEventAdapter::RELEASE:
        {
            const unsigned int  mask = ea.getButtonMask() ;
            const unsigned int  trans = mask ^ m_button_mask ;

            m_button_mask = mask ;


            if( trans & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ) {

                const bool  pressed =  0 != (mask & osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) ;

                m_on_left_button->emit( pressed, handled ) ;
            }



            if( trans & osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON ) {

                const bool  pressed =  0 != (mask & osgGA::GUIEventAdapter::MIDDLE_MOUSE_BUTTON) ;

                m_on_middle_button->emit( pressed, handled ) ;
            }



            if( trans & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON ) {

                const bool  pressed =  0 != (mask & osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON) ;

                m_on_right_button->emit( pressed, handled ) ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG:
        {
            if( ! m_requesting_warp ) {

                m_on_mouse_moved->emit( osg::Vec2( ea.getXnormalized(), ea.getYnormalized() ), handled ) ;

            } else {
#ifdef WIN32
                m_on_mouse_moved->emit( osg::Vec2( ea.getXnormalized(), ea.getYnormalized() ), handled ) ;
#endif

                m_requesting_warp = false ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::FRAME:
        {
            bool        request_warp = false ;
            osg::Vec2   warp ;

            m_on_frame->emit( request_warp, warp, handled ) ;

            if( request_warp ) {
                const float xmin = ea.getXmin() ;
                const float xmax = ea.getXmax() ;
                const float ymin = ea.getYmin() ;
                const float ymax = ea.getYmax() ;

                warp = ( warp + osg::Vec2(1.0, 1.0) ) * 0.5 ;

                warp.x() = xmin + (xmax - xmin) * warp.x() ;
                warp.y() = ymin + (ymax - ymin) * warp.y() ;

                m_requesting_warp = true ;

                aa.requestWarpPointer( warp.x(), warp.y() ) ;
            }
        }
        break ;




        default:
        break ;
    }



    return handled ;
}
/* ....................................................................... */
/* ======================================================================= */
