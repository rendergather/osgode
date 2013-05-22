/*!
 * @file UniversalJoint.cpp
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
#include <osgODE/UniversalJoint>
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
UniversalJoint::UniversalJoint(void)
{
    m_ODE_joint = dJointCreateUniversal(StaticWorld::instance()->getODEWorld(), NULL) ;

    dJointSetData( m_ODE_joint, this ) ;

    m_functions.SetAxis1    = dJointSetUniversalAxis1 ;
    m_functions.GetAxis1    = dJointGetUniversalAxis1 ;

    m_functions.SetAxis2    = dJointSetUniversalAxis2 ;
    m_functions.GetAxis2    = dJointGetUniversalAxis2 ;

    m_functions.SetAnchor1  = dJointSetUniversalAnchor ;
    m_functions.GetAnchor1  = dJointGetUniversalAnchor ;

    m_functions.GetAnchor2  = dJointGetUniversalAnchor2 ;

    m_functions.SetParam    = dJointSetUniversalParam ;
    m_functions.GetParam    = dJointGetUniversalParam ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
UniversalJoint::UniversalJoint(const UniversalJoint& other, const osg::CopyOp& copyop):
    Joint(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
UniversalJoint::~UniversalJoint(void)
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
UniversalJoint::postUpdate(double step_size)
{
    osg::Vec3   C = this->getAnchor2() ;
    osg::Vec3   X = this->getAxis1() ;
    osg::Vec3   Y = this->getAxis2() ;
    osg::Vec3   Z = X ^ Y ;


    osg::Matrix m = osg::Matrix(    X[0], X[1], X[2], 0.0,
                                    Y[0], Y[1], Y[2], 0.0,
                                    Z[0], Z[1], Z[2], 0.0,
                                    C[0], C[1], C[2], 1.0
                                ) ;


    this->getMatrixTransform()->setMatrix( m ) ;



    this->Joint::postUpdate(step_size) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
dJointID
UniversalJoint::cloneODEJoint(dWorldID world) const
{
    PS_DBG2("osgODE::UniversalJoint::cloneODEJoint(%p, world=%p)", this, world) ;

    dJointID    j = dJointCreateUniversal(world, NULL) ;

    if(dJointIsEnabled(m_ODE_joint)) {
        dJointEnable(j) ;
    } else {
        dJointDisable(j) ;
    }

    dJointSetFeedback(j, dJointGetFeedback(m_ODE_joint)) ;



    {
        dVector3    v ;
        dJointGetUniversalAnchor(m_ODE_joint, v) ;
        dJointSetUniversalAnchor(j, v[0], v[1], v[2]) ;
    }

    {
        dVector3    v ;
        dJointGetUniversalAxis1(m_ODE_joint, v) ;
        dJointSetUniversalAxis1(j, v[0], v[1], v[2]) ;
    }

    {
        dVector3    v ;
        dJointGetUniversalAxis2(m_ODE_joint, v) ;
        dJointSetUniversalAxis2(j, v[0], v[1], v[2]) ;
    }



    return j ;
}
/* ....................................................................... */
/* ======================================================================= */
