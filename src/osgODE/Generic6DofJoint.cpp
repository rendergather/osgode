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
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
Generic6DofJoint::Generic6DofJoint(void):
    m_blender_mode  ( true )
{
    this->setInfo(6, 6, 0) ;
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

    this->setInfo(6, 0, 0) ;


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

        osg::Matrix transform = m_blender_mode  ?   b1_to_world : m_initial_transformation ;

        axes[0] = osg::Matrix::transform3x3( getAxis1(), transform ) ;
        axes[1] = osg::Matrix::transform3x3( getAxis2(), transform ) ;
        axes[2] = osg::Matrix::transform3x3( getAxis3(), transform ) ;



        for( int i=0; i<3; i++ ) {
            j1l[i] = axes[i] ;
        }
    }






    int row = 0 ;


    //
    // linear constraint
    //



    // the anchors in world space
    osg::Vec3   anchor1 = getAnchor1() * b1_to_world ;


    osg::Vec3   anchor2 =   b2      ?   getAnchor2() * m_body2->getMatrix()
                                    :
                                        getAnchor2() ;



    // the error in world space
    osg::Vec3   world_error = anchor2 - anchor1 ;



    // the error in local space
    osg::Vec3   joint_error =   m_blender_mode  ?   anchor2 * world_to_b1 - getAnchor1()
                                                :
                                                    osg::Vec3(      world_error * axes[0],
                                                                    world_error * axes[1],
                                                                    world_error * axes[2]
                                                            ) ;




    osg::Vec3   j1a[3], j2a[3] ;

    if( m_blender_mode ) {
        osg::Vec3   tmp1 = getAnchor1() ;

        j1a[0] = osg::Matrix::transform3x3( osg::Vec3( 0.0, tmp1.z(), -tmp1.y() ), b1_to_world ) ;
        j1a[1] = osg::Matrix::transform3x3( osg::Vec3( -tmp1.z(), 0.0, tmp1.x() ), b1_to_world ) ;
        j1a[2] = osg::Matrix::transform3x3( osg::Vec3( tmp1.y(), -tmp1.x(), 0.0 ), b1_to_world ) ;


        if( b2 ) {
            osg::Vec3   tmp2 = getAnchor2() ;

            osg::Quat   q = m_body2->getQuaternion() ;

            j2a[0] = q * osg::Vec3( 0.0, -tmp2.z(), tmp2.y() ) ;
            j2a[1] = q * osg::Vec3( tmp2.z(), 0.0, -tmp2.x() ) ;
            j2a[2] = q * osg::Vec3( -tmp2.y(), tmp2.x(), 0.0 ) ;
        }
    }


    //
    // check for limits
    //

    bool    linear_limits[3] = { false, false, false } ;

    for( int i=0; i<3; i++ ) {

        ooReal  lo_stop = getParam( dParamLoStop + dParamGroup * i) ;
        ooReal  hi_stop = getParam( dParamHiStop + dParamGroup * i) ;

        if( joint_error[i] <= lo_stop ) {

            joint_error[i] -= lo_stop ;

            linear_limits[i] = true ;

        }


        else if( joint_error[i] >= hi_stop ) {

            joint_error[i] -= hi_stop ;

            linear_limits[i] = true ;
        }



        joint_error[i] *= getParam( dParamStopERP + dParamGroup * i ) / step_size ;


    }





    //
    // add the rows
    //
    for( int i=0; i<3; i++ ) {

        if( linear_limits[i] ) {

            setRow( row++, j1a[i], j1l[i], j2a[i], j2l[i], joint_error[i], getParam(dParamStopCFM + dParamGroup * i) ) ;
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

        osg::Vec3   tmp1, tmp2 ;
        osg::Quat   tmp3 ;

        merr.decompose(tmp1, qerr, tmp2, tmp3) ;


    } else {

        osg::Matrix     merr = m_initial_transformation * world_to_b1  ;

        osg::Vec3   tmp1, tmp2 ;
        osg::Quat   tmp3 ;

        merr.decompose(tmp1, qerr, tmp2, tmp3) ;
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




    //
    // check for limits
    //

    bool    angular_limits[3] = { false, false, false } ;

    for( int i=0; i<3; i++ ) {

        ooReal  lo_stop = getParam( dParamLoStop4 + dParamGroup * i) ;
        ooReal  hi_stop = getParam( dParamHiStop4 + dParamGroup * i) ;

        if( c[i] <= lo_stop ) {

            c[i] -= lo_stop ;

            angular_limits[i] = true ;

        }


        else if( c[i] >= hi_stop ) {

            c[i] -= hi_stop ;

            angular_limits[i] = true ;
        }




        c[i] *= getParam( dParamStopERP + dParamGroup4 * i ) / step_size ;

    }





    //
    // add the rows
    //
    for( int i=0; i<3; i++ ) {

        if( angular_limits[i] ) {

            setRow( row++, -axes[i], osg::Vec3(), b2 ? axes[i] : osg::Vec3(), osg::Vec3(), c[i], getParam(dParamStopCFM4 + dParamGroup * i) ) ;
        }
    }




    this->setInfo( 6, row, row ) ;


    this->BypassJoint::update( step_size ) ;
}
/* ....................................................................... */
/* ======================================================================= */
