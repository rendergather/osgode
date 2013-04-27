/*!
 * @file Joint.cpp
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
#include <osgODE/Joint>
#include <osgODE/World>
#include <osgODE/StaticWorld>
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
Joint::Joint(void):
    m_ensure_two_bodies( false )
{
    _initParams() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint::Joint(const Joint& other, const osg::CopyOp& copyop):
    Transformable(other, copyop),
    m_ensure_two_bodies( other.m_ensure_two_bodies ),
    m_functions( other.m_functions ),
    m_anchor1(other.m_anchor1),
    m_anchor2(other.m_anchor2),
    m_axis1(other.m_axis1),
    m_axis2(other.m_axis2),
    m_axis3(other.m_axis3),
    m_param_map(other.m_param_map)
{
    m_ODE_joint = other.cloneODEJoint(StaticWorld::instance()->getODEWorld()) ;

    dJointSetData(m_ODE_joint, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint::~Joint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::postUpdate(double step_size)
{
    // update local data

    readAxis1Implementation(m_axis1) ;
    readAxis2Implementation(m_axis2) ;
    readAxis3Implementation(m_axis3) ;

    readAnchor1Implementation(m_anchor1) ;
    readAnchor2Implementation(m_anchor2) ;


    this->Transformable::postUpdate( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Joint::addToWorldInternal(World* world)
{
    PS_DBG2("osgODE::Joint::addToWorldInternal(%p, world=%p)", this, world) ;



    PS_ASSERT1( world != NULL ) ;



    if(world != getWorld()) {


        //
        // clone the joint in the new world
        //
        {
            // Set the world right here because the *MotorJoint classes
            // check it in their finalize method
            setWorldInternal(world) ;

            dJointID    j = cloneODEJoint( world->getODEWorld() ) ;

            dJointSetData(j, this) ;


            dJointDestroy(m_ODE_joint) ;

            m_ODE_joint = j ;
        }



        // call the finalize to give effect to the joint
        finalize() ;



        setJointFeedback(m_feedback.get()) ;

        return true ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Joint::removeFromWorldInternal(World* world)
{
    PS_DBG2("osgODE::Joint::removeFromWorldInternal(%p, world=%p)", this, world) ;



    PS_ASSERT1( world != NULL ) ;


    if( world == getWorld()  ||  ! getWorld() ) {


        //
        // clone the joint in the static world
        //
        {
            dJointID    j = cloneODEJoint(StaticWorld::instance()->getODEWorld()) ;


            dJointSetData(j, this) ;

            dJointDestroy(m_ODE_joint) ;

            m_ODE_joint = j ;
        }




        setJointFeedback(m_feedback.get()) ;

        return true ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::finalize(void)
{
    PS_DBG2("osgODE::Joint::finalize(%p)", this) ;



    dBodyID b1 = NULL ;
    dBodyID b2 = NULL ;



    if( m_body1.valid() ) {
        b1 = m_body1->getWorld() ? m_body1->getODEBody() : NULL ;
    }



    if( m_body2.valid() ) {
        b2 = m_body2->getWorld() ? m_body2->getODEBody() : NULL ;
    }



    if(    ( ! m_ensure_two_bodies )     ||     ( b1 && b2 )    ) {

        dJointAttach(m_ODE_joint, b1, b2) ;


        _restoreParams() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::setBody1(osgODE::RigidBody* body)
{
    PS_DBG2("osgODE::Joint::setBody1(%p, body=%p)", this, body) ;

    if( m_body1.valid() ) {
        m_body1->removeJointInternal(this) ;
    }

    m_body1 = body ;

    if(body) {
        body->addJointInternal(this) ;
    }

    checkBodiesInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::setBody2(osgODE::RigidBody* body)
{
    PS_DBG2("osgODE::Joint::setBody2(%p, body=%p)", this, body) ;

    if( m_body2.valid() ) {
        m_body2->removeJointInternal(this) ;
    }

    m_body2 = body ;

    if(body) {
        body->addJointInternal(this) ;
    }

    checkBodiesInternal() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Joint*
Joint::asJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::_initParams(void)
{
    setAxis1( osg::X_AXIS ) ;
    setAxis2( osg::Y_AXIS ) ;
    setAxis3( osg::Z_AXIS ) ;

    setAnchor1( osg::Vec3(0,0,0) ) ;
    setAnchor2( osg::Vec3(0,0,0) ) ;

    m_param_map[dParamLoStop]           = -FLT_MAX ;
    m_param_map[dParamHiStop]           =  FLT_MAX ;
    m_param_map[dParamVel]              =  0.0 ;
    m_param_map[dParamFMax]             =  0.0 ;
    m_param_map[dParamFudgeFactor]      =  0.0 ;
    m_param_map[dParamBounce]           =  0.0 ;
    m_param_map[dParamERP]              =  1.0 ;
    m_param_map[dParamCFM]              =  0.0 ;
    m_param_map[dParamStopERP]          =  1.0 ;
    m_param_map[dParamStopCFM]          =  0.0 ;
    m_param_map[dParamSuspensionERP]    =  1.0 ;
    m_param_map[dParamSuspensionCFM]    =  0.0 ;
    m_param_map[dParamERP]              =  1.0 ;

    m_param_map[dParamLoStop2]          = -FLT_MAX ;
    m_param_map[dParamHiStop2]          =  FLT_MAX ;
    m_param_map[dParamVel2]             =  0.0 ;
    m_param_map[dParamFMax2]            =  0.0 ;
    m_param_map[dParamFudgeFactor2]     =  0.0 ;
    m_param_map[dParamBounce2]          =  0.0 ;
    m_param_map[dParamERP2]             =  1.0 ;
    m_param_map[dParamCFM2]             =  0.0 ;
    m_param_map[dParamStopERP2]         =  1.0 ;
    m_param_map[dParamStopCFM2]         =  0.0 ;
    m_param_map[dParamSuspensionERP2]   =  1.0 ;
    m_param_map[dParamSuspensionCFM2]   =  0.0 ;
    m_param_map[dParamERP2]             =  1.0 ;

    m_param_map[dParamLoStop3]          = -FLT_MAX ;
    m_param_map[dParamHiStop3]          =  FLT_MAX ;
    m_param_map[dParamVel3]             =  0.0 ;
    m_param_map[dParamFMax3]            =  0.0 ;
    m_param_map[dParamFudgeFactor3]     =  0.0 ;
    m_param_map[dParamBounce3]          =  0.0 ;
    m_param_map[dParamERP3]             =  1.0 ;
    m_param_map[dParamCFM3]             =  0.0 ;
    m_param_map[dParamStopERP3]         =  1.0 ;
    m_param_map[dParamStopCFM3]         =  0.0 ;
    m_param_map[dParamSuspensionERP3]   =  1.0 ;
    m_param_map[dParamSuspensionCFM3]   =  0.0 ;
    m_param_map[dParamERP3]             =  1.0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Joint::_restoreParams(void)
{
    setAxis1( m_axis1 ) ;
    setAxis2( m_axis2 ) ;
    setAxis3( m_axis3 ) ;

    setAnchor1(m_anchor1) ;
    setAnchor2(m_anchor2) ;


    for( ParamMap::iterator itr = m_param_map.begin();
         itr != m_param_map.end();
         itr++ ) {

        setParamImplementation( itr->first, itr->second ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
