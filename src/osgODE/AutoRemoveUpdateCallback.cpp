/*!
 * @file AutoRemoveUpdateCallback.cpp
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
#include <osgODE/AutoRemoveUpdateCallback>
#include <osgODE/Notify>
#include <osgODE/World>
#include <osgODE/CommonWorldOperations>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
AutoRemoveUpdateCallback::AutoRemoveUpdateCallback( unsigned int life_time ):
    m_life_time     ( life_time ),
    m_counter       ( 0 )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AutoRemoveUpdateCallback::AutoRemoveUpdateCallback(const AutoRemoveUpdateCallback& other, const osg::CopyOp& copyop):
    ODECallback     ( other, copyop ),
    m_life_time     ( other.m_life_time ),
    m_counter       ( other.m_counter )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AutoRemoveUpdateCallback::~AutoRemoveUpdateCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AutoRemoveUpdateCallback::operator()( ODEObject* object )
{
    PS_ASSERT1( object != NULL ) ;

    World*  world = object->getWorld() ;
    PS_ASSERT1( world != NULL ) ;



    ++m_counter ;

    if( m_counter >= m_life_time ) {
        world->addOperation( new RemoveObjectOperation(object) ) ;
    }


    traverse( object ) ;
}
/* ....................................................................... */
/* ======================================================================= */
