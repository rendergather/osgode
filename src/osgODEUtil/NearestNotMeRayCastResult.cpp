/*!
 * @file NearestNotMeRayCastResult.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2012 by Rocco Martino                            *
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
#include <osgODEUtil/NearestNotMeRayCastResult>

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
NearestNotMeRayCastResult::NearestNotMeRayCastResult(osgODE::Collidable* me):
    m_me(me)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NearestNotMeRayCastResult::NearestNotMeRayCastResult(const NearestNotMeRayCastResult& other):
    NearestRayCastResult(other),
    m_me(other.m_me.get())
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
NearestNotMeRayCastResult::~NearestNotMeRayCastResult(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NearestNotMeRayCastResult::setMe(osgODE::Collidable* me)
{
    m_me = me ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
NearestNotMeRayCastResult::addContact(  osgODE::Collidable* other,
                                        const osg::Vec3& position,
                                        const osg::Vec3& surface_normal,
                                        double distance )
{
    if( other != m_me ) {
        this->NearestRayCastResult::addContact(other, position, surface_normal, distance) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
