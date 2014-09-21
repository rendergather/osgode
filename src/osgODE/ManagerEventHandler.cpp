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
ManagerEventHandler::ManagerEventHandler(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ManagerEventHandler::ManagerEventHandler(const ManagerEventHandler& other, const osg::CopyOp& copyop):
    osgGA::GUIEventHandler      ( other, copyop )
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
        }
        break ;




        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG:
        {
            events->setCursorPosition( osg::Vec2( ea.getX(), ea.getY() ) ) ;
            events->setNormalizedCursorPosition( osg::Vec2( ea.getXnormalized(), ea.getYnormalized() ) ) ;
        }
        break ;




        case osgGA::GUIEventAdapter::PUSH:
        case osgGA::GUIEventAdapter::RELEASE:
        {
            events->setButtonMask( ea.getButtonMask() ) ;
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
