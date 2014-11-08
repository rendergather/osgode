/*!
 * @file ManagerEventHandler.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 by Rocco Martino                                   *
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/ManagerEventHandler>

#include <osgODE/Notify>
#include <osgODE/Manager>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
ManagerEventHandler::ManagerEventHandler(void):
    m_warp_pointer_requested    ( false ),
    m_motion                    ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerEventHandler::ManagerEventHandler(const ManagerEventHandler& other, const osg::CopyOp& copyop):
    osgGA::GUIEventHandler      ( other, copyop ),
    m_warp_pointer_requested    ( false ),
    m_motion                    ( false )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerEventHandler::~ManagerEventHandler(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
ManagerEventHandler::handle( const osgGA::GUIEventAdapter&  ea,
                             osgGA::GUIActionAdapter&       aa,
                             osg::Object*                   manager_obj,
                             osg::NodeVisitor*              nv )
{

    (void) nv ;

    Manager*    manager = dynamic_cast<Manager*>( manager_obj ) ;
    PS_ASSERT1( manager != NULL ) ;

    World*      world = manager->getWorld() ;
    PS_ASSERT1( world != NULL ) ;



    Events*     events = world->getEvents() ;

    if( ! events ) {
        return false ;
    }





    switch( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::FRAME:
        {
            events->setView( aa.asView() ) ;

            events->incFrameCounterInternal() ;


            events->setMotion( m_motion ) ;
            m_motion = false ;


            osg::Vec2   warp_pointer ;
            bool        normalized ;


            if( events->getWarpPointer( warp_pointer, normalized ) ) {

                if( normalized ) {
                    const float xmin = ea.getXmin() ;
                    const float xmax = ea.getXmax() ;
                    const float ymin = ea.getYmin() ;
                    const float ymax = ea.getYmax() ;

                    // map from [-1,1] to [0,1]
                    warp_pointer = ( warp_pointer + osg::Vec2(1.0, 1.0) ) * 0.5 ;

                    warp_pointer.x() = xmin + (xmax - xmin) * warp_pointer.x() ;
                    warp_pointer.y() = ymin + (ymax - ymin) * warp_pointer.y() ;
                }

                aa.requestWarpPointer( warp_pointer.x(), warp_pointer.y() ) ;

                m_warp_pointer_requested = true ;

                events->setWarpPointer( false ) ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG:
        {
            m_motion = true ;


            osg::Vec2   previous_position = events->getCursorPosition() ;
            osg::Vec2   cursor_position( ea.getX(), ea.getY() ) ;
            osg::Vec2   normalized_cursor_position( ea.getXnormalized(), ea.getYnormalized() ) ;

            events->setCursorPosition( cursor_position ) ;
            events->setNormalizedCursorPosition( normalized_cursor_position ) ;

            if( ! m_warp_pointer_requested ) {

                events->setRelativeCursorPosition( cursor_position - previous_position) ;

            } else {

                events->setRelativeCursorPosition( osg::Vec2(0,0) ) ;
                m_warp_pointer_requested = false ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::PUSH:
        case osgGA::GUIEventAdapter::RELEASE:
        {
            events->setButtonMask( ea.getButtonMask() ) ;


            osg::Vec2   previous_position = events->getCursorPosition() ;
            osg::Vec2   cursor_position( ea.getX(), ea.getY() ) ;
            osg::Vec2   normalized_cursor_position( ea.getXnormalized(), ea.getYnormalized() ) ;

            events->setCursorPosition( cursor_position ) ;
            events->setNormalizedCursorPosition( normalized_cursor_position ) ;

            if( ! m_warp_pointer_requested ) {

                events->setRelativeCursorPosition( cursor_position - previous_position) ;

            } else {

                events->setRelativeCursorPosition( osg::Vec2(0,0) ) ;
                m_warp_pointer_requested = false ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::KEYDOWN:
        {
            int key = ea.getKey() ;

            if( 0 == (key & 0xFFFFFF00) ) {
                events->setKeyPressed( key, true ) ;
            }
        }
        break ;




        case osgGA::GUIEventAdapter::KEYUP:
        {
            int key = ea.getKey() ;

            if( 0 == (key & 0xFFFFFF00) ) {
                events->setKeyPressed( key, false ) ;
            }
        }
        break ;




        default:
        break ;
    }





    return false ;
}
/* ....................................................................... */
/* ======================================================================= */
