/*!
 * @file AerodynamicDevice.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2013 by Rocco Martino                            *
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
#include <osgODE/AerodynamicDevice>
#include <osgODE/RigidBody>
#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
AerodynamicDevice::AerodynamicDevice(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AerodynamicDevice::AerodynamicDevice(const AerodynamicDevice& other, const osg::CopyOp& copyop):
    ODECallback(other, copyop),
    m_point_list( other.m_point_list )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AerodynamicDevice::~AerodynamicDevice(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
AerodynamicDevice::operator()(ODEObject* object)
{

    RigidBody*  body = object->asRigidBody() ;

    PS_ASSERT1(body != NULL) ;



    World*              world = body->getWorld() ;

    PS_ASSERT1(world != NULL) ;


    const unsigned int  num_points = m_point_list.size() ;

    const osg::Vec3&    wind = world->getCurrentWind() ;
    const ooReal        air_half_density = world->getAirDensity() * 0.5 ;


    for(unsigned int i=0; i<num_points; i++) {

        const osg::Vec4&    current = m_point_list[i] ;
        const osg::Vec3     point( current.x(), current.y(), current.z() ) ;
        const ooReal        drag = current.w() ;

        const osg::Vec3     current_wind = wind - body->getPointVelocity( point, true ) ;

        const osg::Vec3     force = current_wind * current_wind.length() * air_half_density * drag ;

        body->addForce( force, point, false, true ) ;

    }


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
