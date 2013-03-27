/*!
 * @file AerodynamicDevice.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2012 by Rocco Martino                            *
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
AerodynamicDevice::AerodynamicDevice(double cx):
    m_cx(cx)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
AerodynamicDevice::AerodynamicDevice(const AerodynamicDevice& other, const osg::CopyOp& copyop):
    ODECallback(other, copyop),
    m_cx(other.m_cx)
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


    osg::Vec3       wind = world->getCurrentWind() - body->getLinearVelocity() ;

    const osg::Vec3 K = wind * 0.5 * wind.length() * world->getAirDensity() ;



    // F = 1/2 CX VEL^2 DENSITY

    osg::Vec3   F = K * m_cx ;


    // add the force in global co (fourth component = 0)
    body->addForce( osg::Vec4(F, 0.0) ) ;


    traverse(object) ;
}
/* ....................................................................... */
/* ======================================================================= */
