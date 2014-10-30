/*!
 * @file CubicVec3Interpolator.cpp
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
#include <osgODE/CubicVec3Interpolator>
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
CubicVec3Interpolator::CubicVec3Interpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CubicVec3Interpolator::CubicVec3Interpolator(const CubicVec3Interpolator& other, const osg::CopyOp& copyop):
    Vec3Interpolator(other, copyop),
    m_coefficients(other.m_coefficients)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CubicVec3Interpolator::~CubicVec3Interpolator(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CubicVec3Interpolator::init(void)
{
    sortPoints() ;


    if( m_points.size() > 2 ) {
        _computeCoefficients() ;
    }


    setDirty(false) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CubicVec3Interpolator::interpolateImplementation(ooReal x, osg::Vec3& result)
{
    unsigned int            size = m_points.size() ;


    switch( size ) {
        /*
         * No points...
         */
        case 0 :
            result = osg::Vec3() ;
            return ;
        break ;


        /* one point */
        case 1:
            result = m_points[0].second ;
            return ;
        break ;


        /* two points: linear */
        case 2:
        {
            const Point&    p1 = m_points[0] ;
            const Point&    p2 = m_points[1] ;
            result = p1.second + ( (p2.second - p1.second) * (x - p1.first)  / (p2.first - p1.first) ) ;
            return ;
        }
        break ;


        default:
        break ;
    }





    const unsigned int  x0 = findX0(x) ;


    const ooReal    xxx = pow(x, 3) ;
    const ooReal    xx = pow(x, 2) ;

    osg::Vec3   a, b, c, d ;
    _getCoefficients(x0, a, b, c, d) ;

    result.x() = a.x() * xxx  +  b.x() * xx  +  c.x() * x  +  d.x() ;
    result.y() = a.y() * xxx  +  b.y() * xx  +  c.y() * x  +  d.y() ;
    result.z() = a.z() * xxx  +  b.z() * xx  +  c.z() * x  +  d.z() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CubicVec3Interpolator::_computeCoefficients(void)
{
    PS_ASSERT1( m_points.size() > 2 ) ;


    m_coefficients.clear() ;


    const unsigned int  size = m_points.size() ;




    {
        const Point&    a = m_points[0] ;
        const Point&    b = m_points[1] ;
        const Point&    c = m_points[2] ;

        m_coefficients.push_back( _computeCoefficient( a, b, c ) ) ;
    }




    const unsigned int  last = m_points.size() - 2 ;

    for(unsigned int i=1; i < last; i++) {
        const Point&    p0 = m_points[i - 1] ;
        const Point&    p1 = m_points[i] ;
        const Point&    p2 = m_points[i + 1] ;
        const Point&    p3 = m_points[i + 2] ;

        m_coefficients.push_back( _computeCoefficient(p0, p1, p2, p3) ) ;
    }




    {
        const Point&    a = m_points[size-3] ;
        const Point&    b = m_points[size-2] ;
        const Point&    c = m_points[size-1] ;

        m_coefficients.push_back( _computeCoefficient( c, b, a ) ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CubicVec3Interpolator::_getCoefficients(unsigned int idx, osg::Vec3& a, osg::Vec3& b, osg::Vec3& c, osg::Vec3& d)
{
    PS_ASSERT1( idx < m_points.size() ) ;


    const Coefficient&  coefficient = m_coefficients[idx] ;

    a = coefficient.A ;
    b = coefficient.B ;
    c = coefficient.C ;
    d = coefficient.D ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {




typedef ooReal  real_t ;




#define ADD(MAT, BVEC, LHS, RHS, ROWSZ) \
{ \
    real_t* lhs_ = MAT + (LHS) * (ROWSZ) ; \
    real_t* rhs_ = MAT + (RHS) * (ROWSZ) ; \
    BVEC[LHS] += BVEC[RHS] ; \
    for(int i = 0; i < ROWSZ; i++ ) { \
        lhs_[i] += rhs_[i] ; \
    } \
}




#define MUL(MAT, BVEC, LHS, VALUE, ROWSZ) \
{ \
    real_t* lhs_ = MAT + (LHS) * (ROWSZ) ; \
    real_t  val_ = VALUE ; \
    BVEC[LHS] *= val_ ; \
    for(int i = 0; i < n; i++) { \
        lhs_[i] *= val_ ; \
    } \
}




#define LOOKUP(MAT, R, C, ROWSZ) (MAT)[ (R) * (ROWSZ) + (C) ]




static void
gauss_jordan(real_t* A, real_t* b, int n)
{
    for(int r=0; r<n; r++) {

        if( LOOKUP(A,r,r,n) == 0.0 ) {
            int     idx = -1 ;

            for(int i=0; i<n; i++) {
                if( LOOKUP(A,i,r,n) != 0.0 ) {
                    idx = i ;
                    break ;
                }
            }


            if( idx >= 0 ) {
                ADD(A, b, r, idx, n) ;
            }
        }
    }



    for(int c = 0; c < n; c++) {

        if( LOOKUP(A,c,c,n) == 0.0 ) {
            continue ;
        }




        for( int r=0; r<n; r++ ) {

            real_t  value = LOOKUP(A,r,c,n) ;


            if( (r != c)  &&  (value != 0.0) ) {

                MUL(A, b, r, -1.0 * LOOKUP(A,c,c,n) / value, n) ;
                ADD(A, b, r, c, n) ;
            }
        }


    }





    for(int i=0; i<n; i++) {

        real_t  pivot = LOOKUP(A,i,i,n) ;

        if( pivot != 0.0 ) {
            MUL(A, b, i, 1.0 / pivot, n) ;
        }
    }
}

#undef ADD
#undef MUL
#undef LOOKUP
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CubicVec3Interpolator::Coefficient
CubicVec3Interpolator::_computeCoefficient( const Point& p0,
                                            const Point& p1,
                                            const Point& p2,
                                            const Point& p3
                                          )
{
    const ooReal    t1 = p1.first ;
    const ooReal    t2 = p2.first ;
    const ooReal    t0 = p0.first ;
    const ooReal    t3 = p3.first ;

    const ooReal    ttt1 = pow(t1, 3) ;
    const ooReal    tt1 = pow(t1, 2) ;
    const ooReal    ttt2 = pow(t2, 3) ;
    const ooReal    tt2 = pow(t2, 2) ;

    const ooReal    t1_2 = 2.0 * t1 ;
    const ooReal    t2_2 = 2.0 * t2 ;

    const ooReal    tt1_3 = 3.0 * tt1 ;
    const ooReal    tt2_3 = 3.0 * tt2 ;





    Coefficient c ;



    for(int i=0; i<3; i++) {


        ooReal  A[16] = {   ttt1,       tt1,        t1,     1,
                            ttt2,       tt2,        t2,     1,
                            tt1_3,      t1_2,       1,      0,
                            tt2_3,      t2_2,       1,      0
                        } ;




        const ooReal    y0 = p0.second[i] ;
        const ooReal    y1 = p1.second[i] ;
        const ooReal    y2 = p2.second[i] ;
        const ooReal    y3 = p3.second[i] ;





        ooReal  b[4] = {    y1,
                            y2,
                            (y2 - y0)  /  (t2 - t0),
                            (y3 - y1)  /  (t3 - t1)
                        } ;



        gauss_jordan(A, b, 4) ;

        c.A[i] = b[0] ;
        c.B[i] = b[1] ;
        c.C[i] = b[2] ;
        c.D[i] = b[3] ;
    }


    return c ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CubicVec3Interpolator::Coefficient
CubicVec3Interpolator::_computeCoefficient( const Point& p0,
                                            const Point& p1,
                                            const Point& p2
                                          )
{
    const ooReal    t0 = p0.first ;
    const ooReal    t1 = p1.first ;
    const ooReal    t2 = p2.first ;

    const ooReal    tt0 = t0 * t0 ;
    const ooReal    tt1 = t1 * t1 ;

    const ooReal    t1_2 = 2.0 * t1 ;





    Coefficient c ;



    for(int i=0; i<3; i++) {


        ooReal  A[9] =  {   t1_2,   1,      0,
                            tt1,    t1,     1,
                            tt0,    t0,     1
                        } ;




        const ooReal    y0 = p0.second[i] ;
        const ooReal    y1 = p1.second[i] ;
        const ooReal    y2 = p2.second[i] ;





        ooReal  b[3] =  {   (y2 - y0)  /  (t2 - t0),
                            y1,
                            y0
                        } ;



        gauss_jordan(A, b, 3) ;

        c.A[i] = 0.0 ;
        c.B[i] = b[0] ;
        c.C[i] = b[1] ;
        c.D[i] = b[2] ;
    }


    return c ;
}
/* ....................................................................... */
/* ======================================================================= */
