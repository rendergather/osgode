/*!
 * @file CharacterBase.inl
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _OSGODE_CHARACTERBASE_INL
#define _OSGODE_CHARACTERBASE_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::RigidBody*
osgODE::CharacterBase::getBody(void)
{
    return m_body.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::RigidBody*
osgODE::CharacterBase::getBody(void) const
{
    return m_body.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::AMotorJoint*
osgODE::CharacterBase::getAngularMotor(void)
{
    return m_amotor.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::AMotorJoint*
osgODE::CharacterBase::getAngularMotor(void) const
{
    return m_amotor.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::LMotorJoint*
osgODE::CharacterBase::getLinearMotor(void)
{
    return m_lmotor.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::LMotorJoint*
osgODE::CharacterBase::getLinearMotor(void) const
{
    return m_lmotor.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setHeight(double height)
{
    m_height = height ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getHeight(void) const
{
    return m_height ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setMotion(const osg::Vec3& velocity, double fmax)
{
    m_motion_velocity = velocity ;
    m_motion_fmax = fmax ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setUpVersor(const osg::Vec3& up)
{
    m_up_versor = up ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::CharacterBase::getUpVersor(void) const
{
    return m_up_versor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setSideVersor(const osg::Vec3& rhs)
{
    m_side_versor = rhs ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::CharacterBase::getSideVersor(void) const
{
    return m_side_versor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setFrontVersor(const osg::Vec3& front)
{
    m_front_versor = front ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODE::CharacterBase::getFrontVersor(void) const
{
    return m_front_versor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setYaw(double yaw)
{
    m_yaw = yaw ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getYaw(void) const
{
    return m_yaw ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getCurrentYaw(void) const
{
    return m_current_yaw ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setPitch(double pitch)
{
    m_pitch = pitch ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getPitch(void) const
{
    return m_pitch ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getCurrentPitch(void) const
{
    return m_current_pitch ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setElasticity(double elasticity)
{
    m_elasticity = elasticity ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getElasticity(void) const
{
    return m_elasticity ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setJump(const osg::Vec3& force, double res_time)
{
    if( m_jump_res_time <= 0.0 ) {
        m_jump_force = force ;
        m_jump_res_time = res_time ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::CharacterBase::isOnGround(void) const
{
    return m_is_on_ground ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setFootstepInfo( const CharacterBase::FootstepInfo& info )
{
    m_footstep_info = info ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::CharacterBase::FootstepInfo&
osgODE::CharacterBase::getFootstepInfo(void) const
{
    return m_footstep_info ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterBase::setFootContactInfo( const CharacterBase::FootContactInfo& info )
{
    m_foot_contact_info = info ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::CharacterBase::FootContactInfo&
osgODE::CharacterBase::getFootContactInfo(void) const
{
    return m_foot_contact_info ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline double
osgODE::CharacterBase::getFootstepDerivative(void) const
{
    return m_footstep_derivative ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_CHARACTERBASE_INL */
