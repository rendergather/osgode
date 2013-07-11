/*!
 * @file CharacterBase.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/CharacterBase>
#include <osgODE/Collidable>
#include <osgODE/AMotorJoint>
#include <osgODE/LMotorJoint>
#include <osgODE/Space>
#include <osgODE/CommonRayCastResults>
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
CharacterBase::CharacterBase(void):
    m_foot_ray_cast_result  ( new NearestNotMeRayCastResult() ),
    m_up_versor             ( osg::Z_AXIS ),
    m_side_versor           ( osg::X_AXIS ),
    m_front_versor          ( -osg::Z_AXIS ),
    m_yaw                   ( 0.0 ),
    m_pitch                 ( osg::PI * 0.5 ),
    m_current_yaw           ( 0.0 ),
    m_current_pitch         ( osg::PI * 0.5 ),
    m_avg_side_velocity     ( 0.0 ),
    m_footstep_time         ( 0.0 ),
    m_height                ( 1.75 ),
    m_elasticity            ( 5 ),
    m_motion_fmax           ( 0.0),
    m_jump_res_time         ( -1.0 ),
    m_foot_contact_joint    ( NULL ),
    m_is_on_ground          ( false ),
    m_ground_contact_normal ( osg::Z_AXIS ),
    m_footstep_derivative   ( 0.0 )
{

    m_footstep_info.Magnitude       = 250.0 ;
    m_footstep_info.PowerFactor     = 0.25 ;
    m_footstep_info.TimeMultiplier  = 0.5 ;
    m_footstep_info.SpeedThreshold  = 0.1 ;

    m_foot_contact_info.Spring      = 1.0e4 ;
    m_foot_contact_info.Damper      = 1.0e3 ;


    setInteractingSphere( osg::Vec3(), 1.0 ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterBase::CharacterBase(const CharacterBase& other, const osg::CopyOp& copyop):
    Container               ( other, copyop ),
    m_body                  ( other.m_body ),
    m_amotor                ( other.m_amotor ),
    m_lmotor                ( other.m_lmotor ),
    m_foot_ray_cast_result  ( other.m_foot_ray_cast_result ),
    m_up_versor             ( other.m_up_versor ),
    m_side_versor           ( other.m_side_versor ),
    m_front_versor          ( other.m_front_versor ),
    m_yaw                   ( other.m_yaw ),
    m_pitch                 ( other.m_pitch ),
    m_current_yaw           ( other.m_current_yaw ),
    m_current_pitch         ( other.m_current_pitch ),
    m_avg_side_velocity     ( other.m_avg_side_velocity ),
    m_footstep_time         ( other.m_footstep_time ),
    m_height                ( other.m_height ),
    m_elasticity            ( other.m_elasticity ),
    m_motion_velocity       ( other.m_motion_velocity ),
    m_motion_fmax           ( other.m_motion_fmax ),
    m_jump_force            ( other.m_jump_force ),
    m_jump_res_time         ( other.m_jump_res_time ),
    m_foot_contact_joint    ( NULL ),
    m_is_on_ground          ( false ),
    m_ground_contact_normal ( osg::Z_AXIS ),
    m_foot_contact_info     ( other.m_foot_contact_info ),
    m_footstep_info         ( other.m_footstep_info ),
    m_footstep_derivative   ( other.m_footstep_derivative )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterBase::~CharacterBase(void)
{
    if( m_foot_contact_joint ) {

        dJointDestroy( m_foot_contact_joint ) ;

        m_foot_contact_joint = NULL ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::update(double step_size)
{
    _updateOrientation( step_size ) ;

    _collideAgainstGround( step_size ) ;

    _move( step_size ) ;

    _jump( step_size ) ;


    if( m_body.valid() ) {
        setInteractingSphere( m_body->getInteractingSphere() ) ;
    }



    this->Container::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::_updateOrientation(double step_size)
{
    (void) step_size ;



    const osg::Vec3 side_versor_world = m_body->getQuaternion() * m_side_versor ;

    m_current_pitch = (m_current_pitch * m_elasticity + m_pitch) / (m_elasticity + 1) ;
    m_current_yaw = (m_current_yaw * m_elasticity + m_yaw) / (m_elasticity + 1) ;



    osg::Quat   quat = osg::Quat(m_current_pitch, m_side_versor) * osg::Quat(m_current_yaw, m_up_versor) ;






    // experimental
    if( true ) {
        const osg::Vec3 front = m_body->getQuaternion() * m_front_versor ;
        const double    side_vel = side_versor_world * m_body->getLinearVelocity() ;

        m_avg_side_velocity = (m_avg_side_velocity * m_elasticity + side_vel) / (m_elasticity + 1) ;

        quat = quat * osg::Quat( m_avg_side_velocity * -5.0e-3, front ) ;
    }


    m_body->setQuaternion( quat ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::_move(double step_size)
{

    (void) step_size ;

    if( ! m_body.valid() ) {
        return ;
    }

    if( ! m_is_on_ground ) {
        m_lmotor->setParam(dParamVel, 0.0) ;
        m_lmotor->setParam(dParamFMax, 0.0) ;
        m_lmotor->setParam(dParamVel2, 0.0) ;
        m_lmotor->setParam(dParamFMax2, 0.0) ;
        m_lmotor->setParam(dParamVel3, 0.0) ;
        m_lmotor->setParam(dParamFMax3, 0.0) ;
        return ;
    }



    osg::Vec3   direction = m_motion_velocity ;

    direction = m_body->getQuaternion() * direction ;

    const double    speed = direction.normalize() ;

    direction = direction ^ m_ground_contact_normal ;
    direction = m_ground_contact_normal ^ direction ;
    direction.normalize() ;

    const osg::Vec3 force = direction * m_motion_fmax ;
    const osg::Vec3 velocity = direction * speed ;


    m_lmotor->setParam(dParamVel,   velocity.x()        ) ;
    m_lmotor->setParam(dParamVel2,  velocity.y()        ) ;
    m_lmotor->setParam(dParamVel3,  velocity.z()        ) ;

    m_lmotor->setParam(dParamFMax,  fabs( force.x() )   ) ;
    m_lmotor->setParam(dParamFMax2, fabs( force.y() )   ) ;
    m_lmotor->setParam(dParamFMax3, fabs( force.z() )   ) ;




    if( m_jump_res_time <= 0.0 ) {
        const double    body_speed      = m_body->getLinearVelocity().length() ;

        const osg::Vec3 down_versor     = m_up_versor * -1.0 ;

        const double    step_speed      = body_speed >= m_footstep_info.SpeedThreshold ;

        const double    time_multiplier = body_speed * m_footstep_info.TimeMultiplier ;
        const double    power_factor    = body_speed * m_footstep_info.PowerFactor ;
        const double    magnitude       = body_speed * m_footstep_info.Magnitude ;


        m_footstep_time += step_size * time_multiplier ;


        const double    sin_arg = m_footstep_time * 2.0 * osg::PI ;

        double  strength = sin( sin_arg )  * 0.5  +  0.5 ;

        m_footstep_derivative = cos( sin_arg ) * osg::PI ;

        strength = pow( strength, power_factor ) ;



        m_body->addForce( down_versor * magnitude * strength * step_speed ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::_jump(double step_size)
{

    if(     ! ( m_body.valid() && m_is_on_ground )    ) {
        return ;
    }


    if( m_jump_res_time >= 0.0 ) {

        m_body->addForce( m_jump_force ) ;
        m_jump_force.set(0.0, 0.0, 0.0) ;
    }


    m_jump_res_time -= step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase:: _collideAgainstGround(double step_size)
{
    (void) step_size ;


    //
    // Destroy contact joint
    //
    if( m_foot_contact_joint ) {

        dJointDestroy( m_foot_contact_joint ) ;

        m_foot_contact_joint = NULL ;

    }




    if( ! m_body.valid() ) {
        return ;
    }






    PS_ASSERT1( m_body->getWorld() ) ;
    PS_ASSERT1( m_body->getWorld()->asSpace() ) ;




    Space*  space = m_body->getWorld()->asSpace() ;




    const osg::Vec3     ray_from = m_body->getPosition() ;
    const osg::Vec3     ray_to = ray_from - m_up_versor * m_height ;





    m_foot_ray_cast_result->reset() ;
    m_foot_ray_cast_result->setMe( m_body->asCollidable() ) ;

    space->rayCast( ray_from, ray_to, m_foot_ray_cast_result.get(), 4, false, true, true ) ;

    m_is_on_ground = m_foot_ray_cast_result->hasHit() ;



    if( m_is_on_ground ) {


        const Collidable*   collidable = m_foot_ray_cast_result->getCollidable() ;
        PS_ASSERT1( collidable != NULL ) ;




        dContact    contact ;

        contact.surface.mode = dContactApprox1 | dContactFDir1 | dContactMu2 | dContactSoftERP | dContactSoftCFM ;


        // ??
        contact.surface.mu = 0.0 ;
        contact.surface.mu2 = m_body->getMass() * 2.0e-2 ;


        if( m_motion_velocity.length2() < 1.0e-6 ) {
            contact.surface.mu = contact.surface.mu2 ;
        }


        // The contact joint acts as a spring and damper system
        {
            const float h = step_size ;
            const float kp = m_foot_contact_info.Spring ;
            const float kd = m_foot_contact_info.Damper ;
            const float hkp = h * kp ;
            const float hkpkd = hkp + kd ;

            contact.surface.soft_erp = hkp / hkpkd ;
            contact.surface.soft_cfm = 1.0 / hkpkd ;
        }


        // fdir1
        {
            osg::Vec3 fdir1 = m_body->getQuaternion() * m_motion_velocity ;
            fdir1.normalize() ;

            const osg::Vec3&    point = m_foot_ray_cast_result->getPosition() ;
            const osg::Vec3     v1 = m_body->getPointVelocity( point, false ) + fdir1 ;
            const osg::Vec3     v2 = collidable->getPointVelocity( point, false ) ;

            fdir1 = v1 - v2 ;
            fdir1.normalize() ;

            const dReal     up_length = m_up_versor * fdir1 ;
            fdir1 = fdir1 - m_up_versor * up_length ;


            if( fdir1.normalize() >= 1.0e-4 ) {
                dOPE(contact.fdir1, =, fdir1) ;

            } else {
                contact.surface.mode &= ~dContactFDir1 ;
            }
        }


        dOPE(contact.geom.pos, =, ray_to) ;


        // compenetration
        contact.geom.depth = m_height - m_foot_ray_cast_result->getDistance() ;
        PS_ASSERT1(contact.geom.depth >= 0.0) ;

        // normal
        dOPE(contact.geom.normal, =, m_up_versor) ;

        {
            const osg::Vec3 n = m_foot_ray_cast_result->getNormal() ;
            dOPE(m_ground_contact_normal, =, n) ;
        }

        // geoms
        const Collidable*   body = m_body->asCollidable() ;

        contact.geom.g1 = body ? body->getODEGeom() : NULL ;
        contact.geom.g2 = collidable->getODEGeom() ;


//         PS_DBG3("osgODE::CharacterBase::_collideAgainstGround(%p): generating contact", this) ;


        m_foot_contact_joint = dJointCreateContact(space->getODEWorld(), NULL, &contact) ;
        dJointAttach(m_foot_contact_joint, m_body->getODEBody(), collidable->getKinematic() ? NULL : collidable->getODEBody()) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::init(void)
{
    this->Container::removeObject(   getObjectIDX( m_body )    ) ;
    this->Container::removeObject(   getObjectIDX( m_amotor )  ) ;
    this->Container::removeObject(   getObjectIDX( m_lmotor )  ) ;


    //
    // Body
    //
    if( ! m_body.valid() ) {
        m_body = new RigidBody() ;
        m_body->setMass( 75.0 ) ;
    }




    //
    // Angular motor
    //
    if( ! m_amotor.valid() ) {
        m_amotor = new AMotorJoint() ;
    }



    {
        m_amotor->setMotorMode( dAMotorUser ) ;
        m_amotor->setNumAxes( 3 ) ;
        m_amotor->setAxis1Anchor( osgODE::AMotorJoint::BODY1 ) ;
        m_amotor->setAxis2Anchor( osgODE::AMotorJoint::BODY1 ) ;
        m_amotor->setAxis3Anchor( osgODE::AMotorJoint::BODY1 ) ;
        m_amotor->setAxis1( osg::X_AXIS ) ;
        m_amotor->setAxis2( osg::Y_AXIS ) ;
        m_amotor->setAxis3( osg::Z_AXIS ) ;
        m_amotor->setParam( dParamLoStop, 0 ) ;
        m_amotor->setParam( dParamHiStop, 0 ) ;
        m_amotor->setParam( dParamLoStop2, 0 ) ;
        m_amotor->setParam( dParamHiStop2, 0 ) ;
        m_amotor->setParam( dParamLoStop3, 0 ) ;
        m_amotor->setParam( dParamHiStop3, 0 ) ;


        m_amotor->setBody1( m_body ) ;
        m_amotor->setBody2( NULL ) ;
    }




    //
    // Linear motor
    //
    if( ! m_lmotor.valid() ) {
        m_lmotor = new LMotorJoint() ;
    }



    {
        m_lmotor->setNumAxes( 3 ) ;
        m_lmotor->setAxis1Anchor( osgODE::LMotorJoint::WORLD ) ;
        m_lmotor->setAxis2Anchor( osgODE::LMotorJoint::WORLD ) ;
        m_lmotor->setAxis3Anchor( osgODE::LMotorJoint::WORLD ) ;
        m_lmotor->setAxis1( osg::X_AXIS ) ;
        m_lmotor->setAxis2( osg::Y_AXIS ) ;
        m_lmotor->setAxis3( osg::Z_AXIS ) ;
        m_lmotor->setParam( dParamLoStop1, -dInfinity ) ;
        m_lmotor->setParam( dParamHiStop1,  dInfinity ) ;
        m_lmotor->setParam( dParamLoStop2, -dInfinity ) ;
        m_lmotor->setParam( dParamHiStop2,  dInfinity ) ;
        m_lmotor->setParam( dParamLoStop3, -dInfinity ) ;
        m_lmotor->setParam( dParamHiStop3,  dInfinity ) ;


        m_lmotor->setBody1( m_body ) ;
        m_lmotor->setBody2( NULL) ;
    }





    this->Container::addObject( m_body ) ;
    this->Container::addObject( m_amotor ) ;
    this->Container::addObject( m_lmotor ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::clear(void)
{
    m_body = NULL ;
    m_amotor = NULL ;
    m_lmotor = NULL ;


    this->Container::clear() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::setBody(RigidBody* body)
{
    m_body = body ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::setAngularMotor(AMotorJoint* joint)
{
    m_amotor = joint ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterBase::setLinearMotor(LMotorJoint* joint)
{
    m_lmotor = joint ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterBase*
CharacterBase::asCharacterBase(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */
