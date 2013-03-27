/*!
 * @file Math.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 by Rocco Martino                                   *
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
#include <osg/Math>
#include <osgODEUtil/Math>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm

#define SQRT2_2     0.70710678118654757
#define EPSILON     0.01
#define EPSILON2    0.1
/* ....................................................................... */
void
osgODEUtil::getAxisAndAngle( const osg::Matrix& rotation, osg::Vec3& axis, double& angle_out )
{
    const osg::Matrix&  m = rotation ;

    double angle,x,y,z; // variables for result

    if ((fabs(m(0,1)-m(1,0))< EPSILON)
      && (fabs(m(0,2)-m(2,0))< EPSILON)
      && (fabs(m(1,2)-m(2,1))< EPSILON)) {
        // singularity found
        // first check for identity matrix which must have +1 for all terms
        //  in leading diagonaland zero in other terms
        if ((fabs(m(0,1)+m(1,0)) < EPSILON2)
          && (fabs(m(0,2)+m(2,0)) < EPSILON2)
          && (fabs(m(1,2)+m(2,1)) < EPSILON2)
          && (fabs(m(0,0)+m(1,1)+m(2,2)-3) < EPSILON2)) {
            // this singularity is identity matrix so angle = 0
            axis.set(0,0,1) ;
            angle_out = 0.0 ;
            return ;
//             return new axisAngle(0,1,0,0); // zero angle, arbitrary axis
        }
        // otherwise this singularity is angle = 180
        angle = osg::PI;
        double xx = (m(0,0)+1)/2;
        double yy = (m(1,1)+1)/2;
        double zz = (m(2,2)+1)/2;
        double xy = (m(0,1)+m(1,0))/4;
        double xz = (m(0,2)+m(2,0))/4;
        double yz = (m(1,2)+m(2,1))/4;
        if ((xx > yy) && (xx > zz)) { // m(0,0) is the largest diagonal term
            if (xx< EPSILON) {
                x = 0;
                y = SQRT2_2;
                z = SQRT2_2;
            } else {
                x = sqrt(xx);
                y = xy/x;
                z = xz/x;
            }
        } else if (yy > zz) { // m(1,1) is the largest diagonal term
            if (yy< EPSILON) {
                x = SQRT2_2;
                y = 0;
                z = SQRT2_2;
            } else {
                y = sqrt(yy);
                x = xy/y;
                z = yz/y;
            }
        } else { // m(2,2) is the largest diagonal term so base result on this
            if (zz< EPSILON) {
                x = SQRT2_2;
                y = SQRT2_2;
                z = 0;
            } else {
                z = sqrt(zz);
                x = xz/z;
                y = yz/z;
            }
        }
        axis.set(x, y, z) ;
        angle_out = angle ;
        return ;
//         return new axisAngle(angle,x,y,z); // return 180 deg rotation
    }
    // as we have reached here there are no singularities so we can handle normally
    double s = sqrt((m(2,1) - m(1,2))*(m(2,1) - m(1,2))
        +(m(0,2) - m(2,0))*(m(0,2) - m(2,0))
        +(m(1,0) - m(0,1))*(m(1,0) - m(0,1))); // used to normalise
    if (fabs(s) < 0.001) s=1;
        // prevent divide by zero, should not happen if matrix is orthogonal and should be
        // caught by singularity test above, but I've left it in just in case
    angle = acos(( m(0,0) + m(1,1) + m(2,2) - 1)/2);
    x = (m(2,1) - m(1,2))/s;
    y = (m(0,2) - m(2,0))/s;
    z = (m(1,0) - m(0,1))/s;
    axis.set(x, y, z) ;
    angle_out = angle ;
    return ;
//    return new axisAngle(angle,x,y,z);
}
/* ....................................................................... */
#undef SQRT2_2
#undef EPSILON
#undef EPSILON2
/* ======================================================================= */
