/*!
 * @file AllRayCastResult.cpp
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
#include <osgODEUtil/AllRayCastResult>

#include <osgODE/Collidable>

#include <algorithm>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
AllRayCastResult::AllRayCastResult(void)
{
    reset() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AllRayCastResult::AllRayCastResult(const AllRayCastResult& other):
    osgODE::Space::RayCastResult(other),
    m_collision_list( other.m_collision_list )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AllRayCastResult::~AllRayCastResult(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AllRayCastResult::reset(void)
{
    m_collision_list.clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    struct comp_dist_class {
        bool    operator()(AllRayCastResult::Collision a, AllRayCastResult::Collision b) {
            return (a.Distance < b.Distance ) ;
        }
    } comp_dist_obj ;
}

void
AllRayCastResult::sortByDistance(void)
{
    std::sort(m_collision_list.begin(), m_collision_list.end(), comp_dist_obj) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AllRayCastResult::addContact(   osgODE::Collidable* other,
                                const osg::Vec3& position,
                                const osg::Vec3& surface_normal,
                                double distance )
{
    Collision   collision ;

    collision.Collidable = other ;
    collision.Position = position ;
    collision.Normal = surface_normal ;
    collision.Distance = distance ;


    m_collision_list.push_back(collision) ;
}
/* ....................................................................... */
/* ======================================================================= */
