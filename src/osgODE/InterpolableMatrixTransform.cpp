/*!
 * @file InterpolableMatrixTransform.cpp
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
#include <osgODE/InterpolableMatrixTransform>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
InterpolableMatrixTransform::InterpolableMatrixTransform(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
InterpolableMatrixTransform::InterpolableMatrixTransform(const InterpolableMatrixTransform& other, const osg::CopyOp& copyop):
    osg::MatrixTransform(other, copyop)
{
    m_points[0] = other.m_points[0] ;
    m_points[1] = other.m_points[1] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
InterpolableMatrixTransform::~InterpolableMatrixTransform(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
InterpolableMatrixTransform::push(void)
{
    const osg::Matrix&  m = this->getMatrix() ;

    m_points[0] = m_points[1] ;

    osg::Quat   so ;

    m.decompose( m_points[1].Position, m_points[1].Quaternion, m_points[1].Scale, so ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
InterpolableMatrixTransform::interpolate(double t)
{
    osg::Vec3   p = m_points[0].Position + (m_points[1].Position - m_points[0].Position) * t ;
    osg::Vec3   s = m_points[0].Scale + (m_points[1].Scale - m_points[0].Scale) * t ;

    osg::Quat   q ;
    q.slerp(t, m_points[0].Quaternion, m_points[1].Quaternion) ;

    osg::Matrix m = osg::Matrix::scale(s) * osg::Matrix::rotate(q) * osg::Matrix::translate(p) ;


    setMatrix( m ) ;
}
/* ....................................................................... */
/* ======================================================================= */
