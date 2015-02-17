/*!
 * @file BypassJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 - 2015 by Rocco Martino                            *
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
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/BypassJoint>
#include <osgODE/StaticWorld>
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
BypassJoint::BypassJoint(void)
{

    m_ODE_joint = dJointCreateBypass(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::BypassJoint(const BypassJoint& other, const osg::CopyOp& copyop):
    Joint   ( other, copyop )
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
BypassJoint::setRelativeRotation( ooReal step_size, const osg::Quat& qrel, int& row, ooReal erp, ooReal cfm, BodyMask mask )
{
    if(  !  ( m_body1.valid() || m_body2.valid() )  ) {
        return ;
    }


    osg::Quat   q1 = m_body1.valid() ? m_body1->getQuaternion()                 : osg::Quat(0,0,0,1) ;

    osg::Quat   q2 = m_body2.valid() ? m_body2->getQuaternion() * q1.inverse()  : q1.inverse() ;

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

//     if( ((mask & CONSTRAIN_BODY1) != 0)  ^  ((mask & CONSTRAIN_BODY2) != 0) ) {
//         c *= 2.0 ;
//     }


    this->setRow( row++, J1ax, osg::Vec3(), J2ax, osg::Vec3(), c.x(), cfm ) ;
    this->setRow( row++, J1ay, osg::Vec3(), J2ay, osg::Vec3(), c.y(), cfm ) ;
    this->setRow( row++, J1az, osg::Vec3(), J2az, osg::Vec3(), c.z(), cfm ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setRelativePosition( ooReal step_size, const osg::Vec3& prel, int& row, ooReal erp, ooReal cfm, BodyMask mask )
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

    if( ((mask & CONSTRAIN_BODY1) != 0)  ^  ((mask & CONSTRAIN_BODY2) != 0) ) {
        c *= 2.0 ;
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
                        ooReal rhs,
                        ooReal cfm )
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
                        ooReal& rhs,
                        ooReal& cfm ) const
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
    PS_DBG2("oo::BypassJoint::cloneODEJoint(%p, world=%p)", this, world) ;


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




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setParamImplementation(int param, ooReal value)
{
#define CASE_ALL_PARAMS(X) \
    case dParamLoStop##X:           m_lo_stop           [X-1] = value ;     break ; \
    case dParamHiStop##X:           m_hi_stop           [X-1] = value ;     break ; \
    case dParamVel##X:              m_vel               [X-1] = value ;     break ; \
    case dParamLoVel##X:            m_lo_vel            [X-1] = value ;     break ; \
    case dParamHiVel##X:            m_hi_vel            [X-1] = value ;     break ; \
    case dParamFMax##X:             m_f_max             [X-1] = value ;     break ; \
    case dParamFudgeFactor##X:      m_fudge_factor      [X-1] = value ;     break ; \
    case dParamBounce##X:           m_bounce            [X-1] = value ;     break ; \
    case dParamCFM##X:              m_cfm               [X-1] = value ;     break ; \
    case dParamStopERP##X:          m_stop_erp          [X-1] = value ;     break ; \
    case dParamStopCFM##X:          m_stop_cfm          [X-1] = value ;     break ; \
    case dParamSuspensionERP##X:    m_suspension_erp    [X-1] = value ;     break ; \
    case dParamSuspensionCFM##X:    m_suspension_cfm    [X-1] = value ;     break ; \
    case dParamERP##X:              m_erp               [X-1] = value ;     break ;


    switch( param )
    {
        CASE_ALL_PARAMS(1) ;
        CASE_ALL_PARAMS(2) ;
        CASE_ALL_PARAMS(3) ;
        CASE_ALL_PARAMS(4) ;
        CASE_ALL_PARAMS(5) ;
        CASE_ALL_PARAMS(6) ;


        default:    PS_BREAKPOINT() ;   break ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setAxis1Implementation(const osg::Vec3& axis)
{
    m_axis1 = axis ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setAxis2Implementation(const osg::Vec3& axis)
{
    m_axis2 = axis ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setAxis3Implementation(const osg::Vec3& axis)
{
    m_axis3 = axis ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ooReal
BypassJoint::getParamImplementation(int param)
{
    ooReal  value = 0.0 ;


    switch( param )
    {
        case dParamLoStop:          value = m_lo_stop           [0] ;       break ;
        case dParamHiStop:          value = m_hi_stop           [0] ;       break ;
        case dParamVel:             value = m_vel               [0] ;       break ;
        case dParamLoVel:           value = m_lo_vel            [0] ;       break ;
        case dParamHiVel:           value = m_hi_vel            [0] ;       break ;
        case dParamFMax:            value = m_f_max             [0] ;       break ;
        case dParamFudgeFactor:     value = m_fudge_factor      [0] ;       break ;
        case dParamBounce:          value = m_bounce            [0] ;       break ;
        case dParamCFM:             value = m_cfm               [0] ;       break ;
        case dParamStopERP:         value = m_stop_erp          [0] ;       break ;
        case dParamStopCFM:         value = m_stop_cfm          [0] ;       break ;
        case dParamSuspensionERP:   value = m_suspension_erp    [0] ;       break ;
        case dParamSuspensionCFM:   value = m_suspension_cfm    [0] ;       break ;
        case dParamERP:             value = m_erp               [0] ;       break ;

        case dParamLoStop2:         value = m_lo_stop           [1] ;       break ;
        case dParamHiStop2:         value = m_hi_stop           [1] ;       break ;
        case dParamVel2:            value = m_vel               [1] ;       break ;
        case dParamLoVel2:          value = m_lo_vel            [1] ;       break ;
        case dParamHiVel2:          value = m_hi_vel            [1] ;       break ;
        case dParamFMax2:           value = m_f_max             [1] ;       break ;
        case dParamFudgeFactor2:    value = m_fudge_factor      [1] ;       break ;
        case dParamBounce2:         value = m_bounce            [1] ;       break ;
        case dParamCFM2:            value = m_cfm               [1] ;       break ;
        case dParamStopERP2:        value = m_stop_erp          [1] ;       break ;
        case dParamStopCFM2:        value = m_stop_cfm          [1] ;       break ;
        case dParamSuspensionERP2:  value = m_suspension_erp    [1] ;       break ;
        case dParamSuspensionCFM2:  value = m_suspension_cfm    [1] ;       break ;
        case dParamERP2:            value = m_erp               [1] ;       break ;

        case dParamLoStop3:         value = m_lo_stop           [2] ;       break ;
        case dParamHiStop3:         value = m_hi_stop           [2] ;       break ;
        case dParamVel3:            value = m_vel               [2] ;       break ;
        case dParamLoVel3:          value = m_lo_vel            [2] ;       break ;
        case dParamHiVel3:          value = m_hi_vel            [2] ;       break ;
        case dParamFMax3:           value = m_f_max             [2] ;       break ;
        case dParamFudgeFactor3:    value = m_fudge_factor      [2] ;       break ;
        case dParamBounce3:         value = m_bounce            [2] ;       break ;
        case dParamCFM3:            value = m_cfm               [2] ;       break ;
        case dParamStopERP3:        value = m_stop_erp          [2] ;       break ;
        case dParamStopCFM3:        value = m_stop_cfm          [2] ;       break ;
        case dParamSuspensionERP3:  value = m_suspension_erp    [2] ;       break ;
        case dParamSuspensionCFM3:  value = m_suspension_cfm    [2] ;       break ;
        case dParamERP3:            value = m_erp               [2] ;       break ;


        default:    PS_BREAKPOINT() ;   break ;
    }




    return value ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setAnchor1Implementation(const osg::Vec3& anchor)
{
    m_anchor1 = anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::setAnchor2Implementation(const osg::Vec3& anchor)
{
    m_anchor2 = anchor ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::readAxis1Implementation(osg::Vec3& axis)
{
    axis = m_axis1 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::readAxis2Implementation(osg::Vec3& axis)
{
    axis = m_axis2 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::readAxis3Implementation(osg::Vec3& axis)
{
    axis = m_axis3 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::readAnchor1Implementation(osg::Vec3& anchor)
{
    anchor = m_anchor1 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::readAnchor2Implementation(osg::Vec3& anchor)
{
    anchor = m_anchor2 ;
}
/* ....................................................................... */
/* ======================================================================= */
