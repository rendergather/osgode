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
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
BypassJoint::BypassJoint(void)
{
    m_ODE_joint = dJointCreateBypass(StaticWorld::instance()->getODEWorld(), NULL) ;
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
