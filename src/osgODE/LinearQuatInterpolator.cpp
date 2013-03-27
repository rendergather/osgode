/*!
 * @file LinearQuatInterpolator.cpp
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/LinearQuatInterpolator>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
LinearQuatInterpolator::LinearQuatInterpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LinearQuatInterpolator::LinearQuatInterpolator(const LinearQuatInterpolator& other, const osg::CopyOp& copyop):
    QuatInterpolator(other, copyop)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
LinearQuatInterpolator::~LinearQuatInterpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
LinearQuatInterpolator::interpolateImplementation(double x, osg::Quat& result)
{
    unsigned int            size = m_points.size() ;


    switch( size ) {
        /*
         * No points...
         */
        case 0 :
            result = osg::Quat() ;
            return ;
        break ;


        /* one point */
        case 1:
            result = m_points[0].second ;
            return ;
        break ;


        default:
        break ;
    }





    unsigned int    x0 = findX0(x) ;

    /* calculate f(x) */

    const Point&    p1 = m_points[x0] ;
    const Point&    p2 = m_points[x0+1] ;

    const double    t1 = p1.first ;
    const double    t2 = p2.first ;
    const double    t = (x - t1) / (t2 - t1) ;


    result.slerp( t, p1.second, p2.second ) ;
}
/* ....................................................................... */
/* ======================================================================= */
