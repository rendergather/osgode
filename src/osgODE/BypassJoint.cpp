/*!
 * @file BypassJoint.cpp
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
#include <osgODE/BypassJoint>
#include <osgODE/StaticWorld>
#include <osgODE/World>
#include <osgODE/Notify>

#include <iostream>
#include <osg/io_utils>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {

static void
dJointSetBypassParam( dJointID j, int param, dReal value )
{
    PS_ASSERT1( j ) ;

    osgODE::BypassJoint*    joint = static_cast<osgODE::Joint*>( dJointGetData(j) )->asBypassJoint() ;
    PS_ASSERT1( joint ) ;


    switch( param )
    {
        case dParamERP1: joint->setERP( 0, value ) ; break ;
        case dParamERP2: joint->setERP( 1, value ) ; break ;
        case dParamERP3: joint->setERP( 2, value ) ; break ;

        case dParamCFM1: joint->setCFM( 0, value ) ; break ;
        case dParamCFM2: joint->setCFM( 1, value ) ; break ;
        case dParamCFM3: joint->setCFM( 2, value ) ; break ;

        default:    break ;
    }
}



static dReal
dJointGetBypassParam( dJointID j, int param )
{
    PS_ASSERT1( j ) ;

    osgODE::BypassJoint*    joint = static_cast<osgODE::Joint*>( dJointGetData(j) )->asBypassJoint() ;
    PS_ASSERT1( joint ) ;


    switch( param )
    {
        case dParamERP1: return joint->getERP( 0 ) ;
        case dParamERP2: return joint->getERP( 1 ) ;
        case dParamERP3: return joint->getERP( 2 ) ;

        case dParamCFM1: return joint->getCFM( 0 ) ;
        case dParamCFM2: return joint->getCFM( 1 ) ;
        case dParamCFM3: return joint->getCFM( 2 ) ;

        default:    return 0.0 ;
    }
}

} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::BypassJoint(void)
{

    m_ODE_joint = dJointCreateBypass(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;



    m_functions.SetParam    = dJointSetBypassParam ;
    m_functions.GetParam    = dJointGetBypassParam ;


    setParam( dParamERP1, 1.0 ) ;
    setParam( dParamERP2, 1.0 ) ;
    setParam( dParamERP3, 1.0 ) ;

    setParam( dParamCFM1, 0.0 ) ;
    setParam( dParamCFM2, 0.0 ) ;
    setParam( dParamCFM3, 0.0 ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::BypassJoint(const BypassJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::~BypassJoint(void)
{
    if(m_ODE_joint) {
        dJointDestroy(m_ODE_joint) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setRelativeRotation( double step_size, const osg::Quat& qrel, int& row, double erp, double cfm, BodyMask mask )
{
    if(  !  ( m_body1.valid() || m_body2.valid() )  ) {
        return ;
    }


    osg::Quat   q1 = m_body1.valid() ? m_body1->getQuaternion()                 : osg::Quat(0,0,0,1) ;

    osg::Quat   q2 = m_body2.valid() ? m_body2->getQuaternion() * q1.inverse()  : osg::Quat(0,0,0,1) ;

    osg::Quat   qe = qrel.inverse() * q2 ;

    osg::Vec3   c = osg::Vec3( qe.x(), qe.y(), qe.z() ) * 2.0 * erp / step_size ;

    c = q1 * c ;

    if( qe.w() < 0.0 ) {
        c *= -1.0 ;
    }


    osg::Vec3   J1ax, J1ay, J1az ;
    osg::Vec3   J2ax, J2ay, J2az ;

    if( m_body1.valid() && (mask & CONSTRAIN_BODY1) ) {
        J1ax = osg::X_AXIS ;
        J1ay = osg::Y_AXIS ;
        J1az = osg::Z_AXIS ;
    }

    if( m_body2.valid() && (mask & CONSTRAIN_BODY2) ) {
        J2ax = -osg::X_AXIS ;
        J2ay = -osg::Y_AXIS ;
        J2az = -osg::Z_AXIS ;
    }


    this->setRow( row++, J1ax, osg::Vec3(), J2ax, osg::Vec3(), c.x(), cfm ) ;
    this->setRow( row++, J1ay, osg::Vec3(), J2ay, osg::Vec3(), c.y(), cfm ) ;
    this->setRow( row++, J1az, osg::Vec3(), J2az, osg::Vec3(), c.z(), cfm ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setRelativePosition( double step_size, const osg::Vec3& prel, int& row, double erp, double cfm, BodyMask mask )
{
    if(  !  ( m_body1.valid() || m_body2.valid() )  ) {
        return ;
    }


    if( mask == CONSTRAIN_NONE ) {
        return ;
    }





    osg::Vec3   p1 = m_body1.valid() ? prel * m_body1->getMatrix() : prel ;
    osg::Vec3   p2 = m_body2.valid() ? m_body2->getPosition() : osg::Vec3() ;



    osg::Vec3   c = (p2 - p1) * erp / step_size ;

//     double      c = dir.normalize() * erp / step_size ;


    if( c.length() == 0.0 ) {
        return ;
    }


    osg::Vec3   J1lx, J1ly, J1lz ;
    osg::Vec3   J2lx, J2ly, J2lz ;

    if( m_body1.valid() && (mask & CONSTRAIN_BODY1) ) {
        J1lx = osg::X_AXIS ;
        J1ly = osg::Y_AXIS ;
        J1lz = osg::Z_AXIS ;
    }

    if( m_body2.valid() && (mask & CONSTRAIN_BODY2) ) {
        J2lx = -osg::X_AXIS ;
        J2ly = -osg::Y_AXIS ;
        J2lz = -osg::Z_AXIS ;
    }


    this->setRow( row++, osg::Vec3(), J1lx, osg::Vec3(), J2lx, c.x(), cfm ) ;
    this->setRow( row++, osg::Vec3(), J1ly, osg::Vec3(), J2ly, c.y(), cfm ) ;
    this->setRow( row++, osg::Vec3(), J1lz, osg::Vec3(), J2lz, c.z(), cfm ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setRow(    unsigned int row,
                        const osg::Vec3& J1a,
                        const osg::Vec3& J1l,
                        const osg::Vec3& J2a,
                        const osg::Vec3& J2l,
                        double rhs,
                        double cfm )
{

    PS_ASSERT1( row < 6 ) ;


    dVector3    a, b, c, d ;

    dOPE(a, =, J1a) ;
    dOPE(b, =, J1l) ;
    dOPE(c, =, J2a) ;
    dOPE(d, =, J2l) ;


    dJointSetBypassRow(m_ODE_joint, row, a, b, c, d, rhs, cfm) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::getRow(    unsigned int row,
                        osg::Vec3& J1a,
                        osg::Vec3& J1l,
                        osg::Vec3& J2a,
                        osg::Vec3& J2l,
                        double& rhs,
                        double& cfm ) const
{

    PS_ASSERT1( row < 6 ) ;

    dVector3    a, b, c, d ;
    dReal   k, l ;


    dJointGetBypassRow(m_ODE_joint, row, a, b, c, d, &k, &l) ;

    J1a.set( a[0], a[1], a[2] ) ;
    J1l.set( b[0], b[1], b[2] ) ;
    J2a.set( c[0], c[1], c[2] ) ;
    J2l.set( d[0], d[1], d[2] ) ;

    rhs = k ;
    cfm = l ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
BypassJoint*
BypassJoint::asBypassJoint(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
BypassJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::BypassJoint::cloneODEJoint(%p, world=%p)", this, world) ;


    dJointID    j = dJointCreateBypass(world, NULL) ;


    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;

    } else {
        dJointDisable(j) ;
    }


    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;


    {
        dVector3    J1a, J1l, J2a, J2l ;
        dReal   cfm, rhs ;


        for( unsigned int i=0; i<6; i++ ) {
            dJointGetBypassRow(m_ODE_joint, i, J1a, J1l, J2a, J2l, &rhs, &cfm) ;
            dJointSetBypassRow(j, i, J1a, J1l, J2a, J2l, rhs, cfm) ;
        }
    }


    {
        unsigned int    max_m, m, nub ;

        dJointGetBypassInfo(m_ODE_joint, &max_m, &m, &nub) ;
        dJointSetBypassInfo(j, max_m, m, nub) ;
    }


    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
