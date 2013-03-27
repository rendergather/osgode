/*!
 * @file NearestRayCastResult.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
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
#include <osgODEUtil/NearestRayCastResult>

#include <osgODE/Collidable>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
NearestRayCastResult::NearestRayCastResult(void)
{
    reset() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NearestRayCastResult::NearestRayCastResult(const NearestRayCastResult& other):
    osgODE::Space::RayCastResult(other),
    m_collidable( other.m_collidable.get() ),
    m_position( other.m_position ),
    m_normal( other.m_normal ),
    m_distance( other.m_distance )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NearestRayCastResult::~NearestRayCastResult(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NearestRayCastResult::reset(void)
{
    m_collidable = NULL ;
    m_position.set(0,0,0) ;
    m_normal.set(0,0,0) ;
    m_distance = -1 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NearestRayCastResult::addContact(   osgODE::Collidable* other,
                                    const osg::Vec3& position,
                                    const osg::Vec3& surface_normal,
                                    double distance )
{
    if(  distance < m_distance  ||  m_distance < 0 ) {
        m_collidable = other ;
        m_position.set(position) ;
        m_normal.set( surface_normal ) ;
        m_distance = distance ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
