/*!
 * @file ShockWave.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
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
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/ShockWave>
#include <osgODE/ShockWaveCollisionCallback>
#include <osgODE/CommonWorldOperations>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
ShockWave::ShockWave(void):
    m_direction             ( osg::Z_AXIS * -1.0 ),
    m_angle                 ( osg::PI ),
    m_speed                 ( 100.0 ),
    m_impulse               ( 100.0 ),
    m_linear_attenuation    ( 1.0 ),
    m_quadratic_attenuation ( 0.0 ),
    m_threshold             ( 10.0 ),
    m_wave_size             ( 5.0 ),
    m_current_radius        ( 0.0 ),
    m_current_force         ( 0.0 )
{

    this->RigidBody::setKinematic( true ) ;
    this->RigidBody::setBodyEnabled( false ) ;


    CollisionParameters*    cp = new CollisionParameters() ;
    cp->setCollisionsEnabled( false ) ;

    this->Sphere::setCollisionParameters( cp ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ShockWave::ShockWave(const ShockWave& other, const osg::CopyOp& copyop):
    Sphere                  ( other, copyop ),
    m_direction             ( other.m_direction ),
    m_angle                 ( other.m_angle ),
    m_speed                 ( other.m_speed ),
    m_impulse               ( other.m_impulse ),
    m_linear_attenuation    ( other.m_linear_attenuation ),
    m_quadratic_attenuation ( other.m_quadratic_attenuation ),
    m_threshold             ( other.m_threshold ),
    m_wave_size             ( other.m_wave_size ),
    m_current_radius        ( other.m_current_radius ),
    m_current_force         ( other.m_current_force )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ShockWave::~ShockWave(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ShockWave::postUpdate(double step_size)
{
    m_current_radius += m_speed * step_size ;

    const double        diameter = 2.0 * m_current_radius ;




     const double   current_impulse =
                                        m_impulse /
            ( 1.0 + m_current_radius * ( m_linear_attenuation + m_current_radius * m_quadratic_attenuation ) ) ;


    if( current_impulse >= m_threshold ) {

        m_current_force = current_impulse / step_size ;

        this->Sphere::setSize( osg::Vec3( diameter, diameter, diameter ) ) ;

        this->Sphere::postUpdate( step_size ) ;


    } else {
        m_current_force = 0.0 ;

        PS_DBG2("osgODE::ShockWave(%p): removing", this) ;
        this->ODEObject::getWorld()->addOperation( new RemoveObjectOperation(this) ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ShockWave::setupCallbacks(void)
{
    addCollisionCallback( new ShockWaveCollisionCallback() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ShockWave*
ShockWave::asShockWave(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
