/*!
 * @file SuspensionJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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
#include <osgODE/SuspensionJoint>
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
SuspensionJoint::SuspensionJoint(void):
    m_spring(1.0),
    m_damp_bound(0.0),
    m_damp_rebound(0.0),
    m_preload(0.0),
    m_displacement(0.0),
    m_displacement_dsdt(0.0)
{
    setParam(dParamERP1, 1.0) ;
    setParam(dParamERP2, 1.0) ;
    setParam(dParamERP3, 1.0) ;

    setParam(dParamCFM1, 0.0) ;
    setParam(dParamCFM2, 0.0) ;
    setParam(dParamCFM3, 0.0) ;


    setParam(dParamStopERP1, 0.1) ;
    setParam(dParamStopERP2, 1.0) ;
    setParam(dParamStopERP3, 1.0) ;

    setParam(dParamStopCFM1, 0.0) ;
    setParam(dParamStopCFM2, 0.0) ;
    setParam(dParamStopCFM3, 0.0) ;


    setParam(dParamSuspensionERP1, 1.0) ;
    setParam(dParamSuspensionERP2, 1.0) ;
    setParam(dParamSuspensionERP3, 1.0) ;

    setParam(dParamSuspensionCFM1, 0.0) ;
    setParam(dParamSuspensionCFM2, 0.0) ;
    setParam(dParamSuspensionCFM3, 0.0) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SuspensionJoint::SuspensionJoint(const SuspensionJoint& other, const osg::CopyOp& copyop):
    Hinge2Joint(other, copyop),
    m_spring( other.m_spring ),
    m_damp_bound( other.m_damp_bound ),
    m_damp_rebound( other.m_damp_rebound ),
    m_preload( other.m_preload ),
    m_displacement( other.m_displacement ),
    m_displacement_dsdt( other.m_displacement_dsdt )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
SuspensionJoint::~SuspensionJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
SuspensionJoint::update(double step_size)
{
    _computeDisplacement(step_size) ;

    _applyPreload() ;


    const double    damp = m_displacement_dsdt < 0.0 ? m_damp_rebound : m_damp_bound ;


    double  K1 = step_size * m_spring ;
    double  K2 = K1 + damp ;
    double  ERP = K1 / K2 ;
    double  CFM = 1.0 / K2 ;

    setParam(dParamSuspensionERP1, ERP) ;
    setParam(dParamSuspensionCFM1, CFM) ;


    this->Hinge2Joint::update(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
SuspensionJoint::postUpdate(double step_size)
{
    osg::Vec3   C = this->getAnchor2() ;
    osg::Vec3   X = this->getAxis1() ;
    osg::Vec3   Y = this->getAxis2() ;

    osg::Vec3   Z = X ^ Y ;
    Z.normalize() ;

    Y = Z ^ X ;


    osg::Matrix m = osg::Matrix(    X[0], X[1], X[2], 0.0,
                                    Y[0], Y[1], Y[2], 0.0,
                                    Z[0], Z[1], Z[2], 0.0,
                                    C[0], C[1], C[2], 1.0
                                ) ;


    this->getMatrixTransform()->setMatrix( m ) ;
    this->getMatrixTransform()->dirtyBound() ;



    this->Hinge2Joint::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
SuspensionJoint::_computeDisplacement(double step_size)
{
    const double    prev_displacement = m_displacement ;


    const osg::Vec3&    anchor1 = this->getAnchor1() ;
    const osg::Vec3&    anchor2 = this->getAnchor2() ;

    osg::Vec3   displacement = anchor1 - anchor2 ;

    const double    cur_abs_displacement = displacement.normalize() ;



    const osg::Vec3&  axis1 = this->getAxis1() ;
    const osg::Vec3&  axis2 = this->getAxis2() ;

    osg::Vec3   Z = axis1 ;
    osg::Vec3   X = axis2 ;
    osg::Vec3   Y = Z ^ X ;
    X = Y ^ Z ;

    osg::Matrix m ;

    m(0,0) = X.x() ;    m(0,1) = X.y() ;    m(0,2) = X.z() ;    m(0,3) = 0.0 ;
    m(1,0) = Y.x() ;    m(1,1) = Y.y() ;    m(1,2) = Y.z() ;    m(1,3) = 0.0 ;
    m(2,0) = Z.x() ;    m(2,1) = Z.y() ;    m(2,2) = Z.z() ;    m(2,3) = 0.0 ;

    m(3,0) = anchor1.x() ;
    m(3,1) = anchor1.y() ;
    m(3,2) = anchor1.z() ;
    m(3,3) = 1.0 ;

    osg::Vec3   p = anchor2 * osg::Matrix::inverse( m ) ;

    m_displacement = p.z() >= 0.0 ? cur_abs_displacement : -cur_abs_displacement ;


    m_displacement_dsdt = (m_displacement - prev_displacement) / step_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
SuspensionJoint::_applyPreload(void)
{
    osg::Vec3   F = getOrCreateJointFeedback()->getF1() ;

    double  load = m_preload - F.normalize() ;

    const osg::Vec3&    Z = this->getAxis1() ;


    if(load > 0.0) {

        const osg::Vec3 PL = Z * load ;
        const osg::Vec4 pos(getBody2()->getPosition(), 0) ;

        getBody1()->addForce( osg::Vec4( -PL, 0 ), pos ) ;
        getBody2()->addForce( osg::Vec4(  PL, 0 ), pos ) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */
