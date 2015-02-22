    /*!
 * @file Generic6DofJoint.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2015 by Rocco Martino                                   *
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
#include <osgODE/Generic6DofJoint>

#include <osgODE/Notify>

#include <iostream>
#include <osg/io_utils>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Generic6DofJoint::Generic6DofJoint(void):
    m_blender_mode  ( true )
{
    this->setSureMaxInfo(6) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Generic6DofJoint::Generic6DofJoint(const Generic6DofJoint& other, const osg::CopyOp& copyop):
    BypassJoint     ( other, copyop ),
    m_blender_mode  ( other.m_blender_mode )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Generic6DofJoint::~Generic6DofJoint(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Generic6DofJoint::update( ooReal step_size )
{

    PS_ASSERT1( m_body1.valid() ) ;

    this->setInfo1(0, 0) ;


    bool    b1 = m_body1.valid() ;
    bool    b2 = m_body2.valid() ;


    if( ! b1 ) {
        return ;
    }


    const osg::Matrix   b1_to_world = m_body1->getMatrix() ;
    const osg::Matrix   world_to_b1 = osg::Matrix::inverse( b1_to_world ) ;



    //
    // the axes in world space
    //
    osg::Vec3   axes[3] ;


    osg::Vec3   j1l[3] ;
    osg::Vec3   j2l[3] ;


    if( b2 ) {

        axes[0] = osg::Matrix::transform3x3( getAxis1(), b1_to_world ) ;
        axes[1] = osg::Matrix::transform3x3( getAxis2(), b1_to_world ) ;
        axes[2] = osg::Matrix::transform3x3( getAxis3(), b1_to_world ) ;


        for( int i=0; i<3; i++ ) {

            j1l[i] = axes[i] ;

            j2l[i] = axes[i] * -1.0 ;
        }




    } else {

        osg::Matrix transform = m_initial_transformation ;

        axes[0] = osg::Matrix::transform3x3( getAxis1(), transform ) ;
        axes[1] = osg::Matrix::transform3x3( getAxis2(), transform ) ;
        axes[2] = osg::Matrix::transform3x3( getAxis3(), transform ) ;



        for( int i=0; i<3; i++ ) {

            j1l[i] = axes[i] ;
        }
    }





    // the anchors in world space
    osg::Vec3   anchor1_world = getAnchor1() * b1_to_world ;


    osg::Vec3   anchor2_world =   b2    ?   getAnchor2() * m_body2->getMatrix()
                                        :   getAnchor2() ;





    osg::Matrix joint_to_world ;

    for( int r=0; r<3; r++ ) {

        joint_to_world(3,r) = anchor1_world[r] ;


        for( int c=0; c<3; c++ ) {
            joint_to_world(r, c) = axes[r][c] ;
        }
    }




    osg::Matrix world_to_joint = osg::Matrix::inverse( joint_to_world ) ;



    // anchors in joint space

    // this is always 0 0 0
    osg::Vec3   anchor1_joint = anchor1_world * world_to_joint ;


    osg::Vec3   anchor2_joint = anchor2_world * world_to_joint ;






    int row = 0 ;


    //
    // linear constraint
    //




    osg::Vec3   j1a[3], j2a[3] ;

    if( m_blender_mode ) {
        osg::Vec3   tmp = anchor1_joint - (m_body1->getPosition() * world_to_joint) ;

        j1a[0] = osg::Matrix::transform3x3( osg::Vec3( 0.0, tmp.z(), -tmp.y() ), joint_to_world) ;
        j1a[1] = osg::Matrix::transform3x3( osg::Vec3( -tmp.z(), 0.0, tmp.x() ), joint_to_world) ;
        j1a[2] = osg::Matrix::transform3x3( osg::Vec3( tmp.y(), -tmp.x(), 0.0 ), joint_to_world) ;


        if( b2 ) {
            osg::Vec3   tmp = anchor2_joint - (m_body2->getPosition() * world_to_joint) ;

            j2a[0] = osg::Matrix::transform3x3( osg::Vec3( 0.0, -tmp.z(), tmp.y() ), joint_to_world ) ;
            j2a[1] = osg::Matrix::transform3x3( osg::Vec3( tmp.z(), 0.0, -tmp.x() ), joint_to_world ) ;
            j2a[2] = osg::Matrix::transform3x3( osg::Vec3( -tmp.y(), tmp.x(), 0.0 ), joint_to_world ) ;
        }
    }





    // the error in joint space
    osg::Vec3   error_joint = anchor2_joint - anchor1_joint ;




    //
    // nub counter
    //

    int nub = 0 ;





    //
    // limits
    //

    bool    linear_limits[3] = { false, false, false } ;

    ooReal  linear_lo[3] = { 0, 0, 0 } ;
    ooReal  linear_hi[3] = { 0, 0, 0 } ;

    for( int i=0; i<3; i++ ) {

        ooReal  lo_stop = getParam( dParamLoStop + dParamGroup * i) ;
        ooReal  hi_stop = getParam( dParamHiStop + dParamGroup * i) ;


        if( error_joint[i] <= lo_stop ) {

            error_joint[i] -= lo_stop ;
            linear_lo[i] = -dInfinity ;

            linear_limits[i] = true ;

        }


        else if( error_joint[i] >= hi_stop ) {

            error_joint[i] -= hi_stop ;
            linear_hi[i] = dInfinity ;

            linear_limits[i] = true ;
        }




        if( linear_limits[i]  &&  lo_stop == hi_stop ) {

            linear_lo[i] = -dInfinity ;
            linear_hi[i] = dInfinity ;

            nub++ ;
        }



        error_joint[i] *= getParam( dParamStopERP + dParamGroup * i ) / step_size ;


    }





    //
    // add the rows
    //
    for( int i=0; i<3; i++ ) {

        if( linear_limits[i] ) {

            setRow( row++, j1a[i], j1l[i], j2a[i], j2l[i], error_joint[i], getParam(dParamStopCFM + dParamGroup * i), linear_lo[i], linear_hi[i] ) ;
        }
    }














    //
    // angular constraint
    //


    //
    // relative rotation
    //
    osg::Quat   qerr ;

    if( b2 ) {

        osg::Matrix     merr = m_body2->getMatrix() * world_to_b1 ;

        merr = merr * osg::Matrix::inverse( m_initial_transformation ) ;

        osg::Vec3   t, s ;
        osg::Quat   so ;

        merr.decompose(t, qerr, s, so) ;


    } else {

        osg::Matrix     merr = m_initial_transformation * world_to_b1  ;

        osg::Vec3   t, s ;
        osg::Quat   so ;

        merr.decompose(t, qerr, s, so) ;
    }




    //
    // get the rotation in axis * angle
    //
    double      angle ;
    osg::Vec3   rotation_axis ;

    {
        double  x, y, z ;

        qerr.getRotate( angle, x, y, z ) ;

        rotation_axis.set( x, y, z ) ;
    }



    //
    // compute the rhs
    //
    osg::Vec3   c = rotation_axis * angle * -1.0 ;

    c = osg::Matrix::transform3x3( c, world_to_joint ) ;




    //
    // limits
    //

    bool    angular_limits[3] = { false, false, false } ;

    ooReal  angular_lo[3] = { 0, 0, 0 } ;
    ooReal  angular_hi[3] = { 0, 0, 0 } ;

    for( int i=0; i<3; i++ ) {

        ooReal  lo_stop = getParam( dParamLoStop4 + dParamGroup * i) ;
        ooReal  hi_stop = getParam( dParamHiStop4 + dParamGroup * i) ;

        if( c[i] <= lo_stop ) {

            c[i] -= lo_stop ;
            angular_lo[i] = -dInfinity ;

            angular_limits[i] = true ;

        }


        else if( c[i] >= hi_stop ) {

            c[i] -= hi_stop ;
            angular_hi[i] = dInfinity ;

            angular_limits[i] = true ;
        }




        if( angular_limits[i]  &&  lo_stop == hi_stop ) {

            angular_lo[i] = -dInfinity ;
            angular_hi[i] = dInfinity ;

            nub++ ;
        }




        c[i] *= getParam( dParamStopERP + dParamGroup4 * i ) / step_size ;

    }





    //
    // add the rows
    //
    for( int i=0; i<3; i++ ) {

        if( angular_limits[i] ) {

            setRow( row++, -axes[i], osg::Vec3(), b2 ? axes[i] : osg::Vec3(), osg::Vec3(), c[i], getParam(dParamStopCFM4 + dParamGroup * i), angular_lo[i], angular_hi[i] ) ;
        }
    }




    this->setInfo1( row, nub ) ;


    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Generic6DofJoint::updateTransformInternal(void)
{

    osg::Vec3   axis1 ;
    osg::Vec3   axis2 ;
    osg::Vec3   axis3 ;

    osg::Vec3   anchor ;




    if( m_body1.valid() && m_body2.valid() ) {

        const osg::Quat     quaternion = m_body1->getQuaternion() ;
        const osg::Matrix   transform = m_body1->getMatrix() ;

        axis1 = quaternion * getAxis1() ;
        axis2 = quaternion * getAxis2() ;
        axis3 = quaternion * getAxis3() ;

        anchor = getAnchor1() * transform ;

    } else {

        axis1 = getAxis1() ;
        axis2 = getAxis2() ;
        axis3 = getAxis3() ;

        anchor = getAnchor2() ;
    }




    osg::Matrix m = osg::Matrix(    axis1[0],   axis1[1],   axis1[2],   0.0,
                                    axis2[0],   axis2[1],   axis2[2],   0.0,
                                    axis3[0],   axis3[1],   axis3[2],   0.0,
                                    anchor[0],  anchor[1],  anchor[2],  1.0
                                ) ;




    this->getMatrixTransform()->setMatrix( m ) ;
    this->getMatrixTransform()->dirtyBound() ;
}
/* ....................................................................... */
/* ======================================================================= */
