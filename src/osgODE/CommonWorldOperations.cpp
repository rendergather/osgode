/*!
 * @file CommonWorldOperations.cpp
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
#include <osgODE/CommonWorldOperations>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
RemoveObjectOperation::RemoveObjectOperation(ODEObject* object, bool acquire_traverse_lock):
    m_object(object),
    m_acquire_traverse_lock(acquire_traverse_lock)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RemoveObjectOperation::RemoveObjectOperation(const RemoveObjectOperation& other):
    World::Operation(other),
    m_object( other.m_object ),
    m_acquire_traverse_lock( other.m_acquire_traverse_lock )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RemoveObjectOperation::~RemoveObjectOperation(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RemoveObjectOperation::operator()(World* world)
{
    if( m_acquire_traverse_lock ) {
        world->traverseLock() ;
        world->removeObject( m_object.get() ) ;
        world->traverseUnlock() ;
    } else {
        world->removeObject( m_object.get() ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AddObjectOperation::AddObjectOperation(ODEObject* object, bool acquire_traverse_lock):
    m_object(object),
    m_acquire_traverse_lock(acquire_traverse_lock)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AddObjectOperation::AddObjectOperation(const AddObjectOperation& other):
    World::Operation(other),
    m_object( other.m_object ),
    m_acquire_traverse_lock( other.m_acquire_traverse_lock )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AddObjectOperation::~AddObjectOperation(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AddObjectOperation::operator()(World* world)
{
    if( m_acquire_traverse_lock ) {
        world->traverseLock() ;
        world->addObject( m_object.get() ) ;
        world->traverseUnlock() ;
    } else {
        world->addObject( m_object.get() ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
