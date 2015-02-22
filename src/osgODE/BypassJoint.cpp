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
BypassJoint::BypassJoint(void):
    m_initial_transformation_set    ( false )
{

    m_ODE_joint = dJointCreateBypass(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::BypassJoint(const BypassJoint& other, const osg::CopyOp& copyop):
    Joint                           ( other, copyop ),
    m_initial_transformation        ( other.m_initial_transformation ),
    m_initial_transformation_set    ( other.m_initial_transformation_set )
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


    this->setRow( row++, J1ax, osg::Vec3(), J2ax, osg::Vec3(), c.x(), cfm, -dInfinity, dInfinity ) ;
    this->setRow( row++, J1ay, osg::Vec3(), J2ay, osg::Vec3(), c.y(), cfm, -dInfinity, dInfinity ) ;
    this->setRow( row++, J1az, osg::Vec3(), J2az, osg::Vec3(), c.z(), cfm, -dInfinity, dInfinity ) ;
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


    this->setRow( row++, osg::Vec3(), J1lx, osg::Vec3(), J2lx, c.x(), cfm, -dInfinity, dInfinity ) ;
    this->setRow( row++, osg::Vec3(), J1ly, osg::Vec3(), J2ly, c.y(), cfm, -dInfinity, dInfinity ) ;
    this->setRow( row++, osg::Vec3(), J1lz, osg::Vec3(), J2lz, c.z(), cfm, -dInfinity, dInfinity ) ;
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
                        ooReal cfm,
                        ooReal lo,
                        ooReal hi
                   )
{

    PS_ASSERT1( row < 6 ) ;


    dVector3    a, b, c, d ;

    dOPE(a, =, J1a) ;
    dOPE(b, =, J1l) ;
    dOPE(c, =, J2a) ;
    dOPE(d, =, J2l) ;


    dJointSetBypassRow(m_ODE_joint, row, a, b, c, d, rhs, cfm, lo, hi) ;
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
                        ooReal& cfm,
                        ooReal& lo,
                        ooReal& hi
                   ) const
{

    PS_ASSERT1( row < 6 ) ;

    dVector3    a, b, c, d ;
    dReal   k, l, m, n ;


    dJointGetBypassRow(m_ODE_joint, row, a, b, c, d, &k, &l, &m, &n) ;

    J1a.set( a[0], a[1], a[2] ) ;
    J1l.set( b[0], b[1], b[2] ) ;
    J2a.set( c[0], c[1], c[2] ) ;
    J2l.set( d[0], d[1], d[2] ) ;

    rhs = k ;
    cfm = l ;
    lo = m ;
    hi = n ;
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
        dReal   cfm, rhs, lo, hi ;


        for( unsigned int i=0; i<6; i++ ) {
            dJointGetBypassRow(m_ODE_joint, i, J1a, J1l, J2a, J2l, &rhs, &cfm, &lo, &hi) ;
            dJointSetBypassRow(j, i, J1a, J1l, J2a, J2l, rhs, cfm, lo, hi) ;
        }
    }


    {
        unsigned int    max_m, m, nub ;

        dJointGetBypassInfo1(m_ODE_joint, &m, &nub) ;
        dJointSetBypassInfo1(j, m, nub) ;

        dJointGetBypassSureMaxInfo(m_ODE_joint, &max_m) ;
        dJointSetBypassSureMaxInfo(j, max_m) ;
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


#undef CASE_ALL_PARAMS
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


#define CASE_ALL_PARAMS( X ) \
        case dParamLoStop##X:           value = m_lo_stop           [X-1] ;     break ; \
        case dParamHiStop##X:           value = m_hi_stop           [X-1] ;     break ; \
        case dParamVel##X:              value = m_vel               [X-1] ;     break ; \
        case dParamLoVel##X:            value = m_lo_vel            [X-1] ;     break ; \
        case dParamHiVel##X:            value = m_hi_vel            [X-1] ;     break ; \
        case dParamFMax##X:             value = m_f_max             [X-1] ;     break ; \
        case dParamFudgeFactor##X:      value = m_fudge_factor      [X-1] ;     break ; \
        case dParamBounce##X:           value = m_bounce            [X-1] ;     break ; \
        case dParamCFM##X:              value = m_cfm               [X-1] ;     break ; \
        case dParamStopERP##X:          value = m_stop_erp          [X-1] ;     break ; \
        case dParamStopCFM##X:          value = m_stop_cfm          [X-1] ;     break ; \
        case dParamSuspensionERP##X:    value = m_suspension_erp    [X-1] ;     break ; \
        case dParamSuspensionCFM##X:    value = m_suspension_cfm    [X-1] ;     break ; \
        case dParamERP##X:              value = m_erp               [X-1] ;     break ;


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


#undef CASE_ALL_PARAMS




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




/* ======================================================================= */
/* ....................................................................... */
void
BypassJoint::finalize(void)
{
    PS_DBG2("oo::BypassJoint::finalize(%p)", this) ;

    this->Joint::finalize() ;




    if( m_initial_transformation_set ) {
        return ;
    }



    if( m_body1.valid() && m_body2.valid() ) {

        m_initial_transformation = m_body2->getMatrix() * osg::Matrix::inverse( m_body1->getMatrix() ) ;



    } else if( m_body1.valid() ) {

        m_initial_transformation = m_body1->getMatrix() ;



    } else if( m_body2.valid() ) {

        m_initial_transformation = m_body2->getMatrix() ;



    } else {

        m_initial_transformation = osg::Matrix::identity() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
