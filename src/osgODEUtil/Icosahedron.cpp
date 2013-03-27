/*!
 * @file Icosahedron.cpp
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
#include <osgODEUtil/Icosahedron>

#include <osgODE/Notify>

#ifndef WIN32
#include <unistd.h>
#endif
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
Icosahedron::Icosahedron(unsigned int subdivisions, bool smooth):
    m_smooth(smooth)
{
    setSupportsDisplayList(true) ;




#define ADD_VERTEX(X, Y, Z) \
{ \
    osg::Vec3   v(X, Y, Z) ; \
    v.normalize() ; \
    m_vertex_array->push_back(v) ; \
}


#define ADD_TRIANGLE(I1, I2, I3) \
{ \
    m_index_array->push_back(I1) ; \
    m_index_array->push_back(I2) ; \
    m_index_array->push_back(I3) ; \
}




    m_vertex_array = new osg::Vec3Array() ;
    m_index_array = new osg::IntArray() ;


    const float t = (1.0 + sqrt(5.0)) * 0.5 ;

    ADD_VERTEX( t,  1,  0 ) ;
    ADD_VERTEX(-t,  1,  0 ) ;
    ADD_VERTEX( t, -1,  0 ) ;
    ADD_VERTEX(-t, -1,  0 ) ;
    ADD_VERTEX( 0,  t,  1 ) ;
    ADD_VERTEX( 0, -t,  1 ) ;
    ADD_VERTEX( 0,  t, -1 ) ;
    ADD_VERTEX( 0, -t, -1 ) ;
    ADD_VERTEX( 1,  0,  t ) ;
    ADD_VERTEX( 1,  0, -t ) ;
    ADD_VERTEX(-1,  0,  t ) ;
    ADD_VERTEX(-1,  0, -t ) ;


    ADD_TRIANGLE( 0,  4,  8) ;
    ADD_TRIANGLE( 1,  6, 11) ;
    ADD_TRIANGLE( 2,  5,  7) ;
    ADD_TRIANGLE(11,  3,  1) ;
    ADD_TRIANGLE( 0,  9,  6) ;
    ADD_TRIANGLE( 3,  5, 10) ;
    ADD_TRIANGLE( 3,  7,  5) ;
    ADD_TRIANGLE( 4,  10, 8) ;
    ADD_TRIANGLE( 2,  8,  5) ;
    ADD_TRIANGLE( 3,  11, 7) ;
    ADD_TRIANGLE( 8,  2,  0) ;
    ADD_TRIANGLE( 5,  8, 10) ;
    ADD_TRIANGLE( 2,  7,  9) ;
    ADD_TRIANGLE( 0,  6,  4) ;
    ADD_TRIANGLE( 9,  0,  2) ;
    ADD_TRIANGLE( 6,  9, 11) ;
    ADD_TRIANGLE( 1,  10, 4) ;
    ADD_TRIANGLE( 1,  4,  6) ;
    ADD_TRIANGLE(10,  1,  3) ;
    ADD_TRIANGLE( 7,  11, 9) ;

#undef ADD_TRIANGLE
#undef ADD_VERTEX


    for(unsigned int i=0; i<subdivisions; i++) {
        subdivide() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Icosahedron::Icosahedron(const Icosahedron& other, const osg::CopyOp& copyop):
    osg::Drawable(other, copyop),
    m_vertex_array( new osg::Vec3Array( *other.m_vertex_array, copyop ) ),
    m_index_array( new osg::IntArray( *other.m_index_array, copyop ) ),
    m_smooth( other.m_smooth )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Icosahedron::~Icosahedron(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Icosahedron::drawImplementation(osg::RenderInfo& render_info) const
{
	(void) render_info ;


#ifndef ANDROID
    glBegin(GL_TRIANGLES) ;

        const unsigned int  index_array_size = m_index_array->getNumElements() ;

        if( m_smooth ) {

            for(unsigned int i=0; i<index_array_size; i+=3) {
                const int   a = (*m_index_array)[i] ;
                const int   b = (*m_index_array)[i+1] ;
                const int   c = (*m_index_array)[i+2] ;

                const osg::Vec3&    va = (*m_vertex_array)[a] ;
                const osg::Vec3&    vb = (*m_vertex_array)[b] ;
                const osg::Vec3&    vc = (*m_vertex_array)[c] ;


                glNormal3f(va.x(), va.y(), va.z()) ;
                glVertex3f(va.x(), va.y(), va.z()) ;

                glNormal3f(vb.x(), vb.y(), vb.z()) ;
                glVertex3f(vb.x(), vb.y(), vb.z()) ;

                glNormal3f(vc.x(), vc.y(), vc.z()) ;
                glVertex3f(vc.x(), vc.y(), vc.z()) ;
            }

        } else {

            for(unsigned int i=0; i<index_array_size; i+=3) {
                const int   a = (*m_index_array)[i] ;
                const int   b = (*m_index_array)[i+1] ;
                const int   c = (*m_index_array)[i+2] ;

                const osg::Vec3&    va = (*m_vertex_array)[a] ;
                const osg::Vec3&    vb = (*m_vertex_array)[b] ;
                const osg::Vec3&    vc = (*m_vertex_array)[c] ;

                osg::Vec3   n = va + vb + vc ;
                n.normalize() ;

                glNormal3f(n.x(), n.y(), n.z()) ;


                glVertex3f(va.x(), va.y(), va.z()) ;
                glVertex3f(vb.x(), vb.y(), vb.z()) ;
                glVertex3f(vc.x(), vc.y(), vc.z()) ;
            }
        }

    glEnd() ;
#endif
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::BoundingBox
Icosahedron::computeBound(void) const
{
    return osg::BoundingBox( osg::Vec3(-1, -1, -1), osg::Vec3(1, 1, 1) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Icosahedron::subdivide(void)
{
    std::map< int, std::map<int, int> >     vmap ;
    std::map< int, std::map<int, bool> >    dmap ;



    int     current_index = m_vertex_array->getNumElements() ;

    const unsigned int  num_indices = m_index_array->getNumElements() ;

    for(unsigned int i=0; i<num_indices; i+=3) {
        const int   ia = (*m_index_array)[i] ;
        const int   ib = (*m_index_array)[i+1] ;
        const int   ic = (*m_index_array)[i+2] ;

        const bool  dab = dmap[ia][ib] | dmap[ib][ia] ;
        const bool  dbc = dmap[ib][ic] | dmap[ic][ib] ;
        const bool  dca = dmap[ic][ia] | dmap[ia][ic] ;

        const osg::Vec3     a = (*m_vertex_array)[ia] ;
        const osg::Vec3     b = (*m_vertex_array)[ib] ;
        const osg::Vec3     c = (*m_vertex_array)[ic] ;



        if( ! dab ) {
            const int   iab = current_index++ ;

            const int   m = osg::minimum(ia, ib) ;
            const int   M = osg::maximum(ia, ib) ;

            vmap[m][M] = iab ;
            dmap[m][M] = true ;

            osg::Vec3   ab = a + b ;
            ab.normalize() ;
            m_vertex_array->push_back( ab ) ;
        }



        if( ! dbc ) {
            const int   ibc = current_index++ ;

            const int   m = osg::minimum(ib, ic) ;
            const int   M = osg::maximum(ib, ic) ;

            vmap[m][M] = ibc ;
            dmap[m][M] = true ;

            osg::Vec3   bc = b + c ;
            bc.normalize() ;
            m_vertex_array->push_back( bc ) ;
        }



        if( ! dca ) {
            const int   ica = current_index++ ;

            const int   m = osg::minimum(ic, ia) ;
            const int   M = osg::maximum(ic, ia) ;

            vmap[m][M] = ica ;
            dmap[m][M] = true ;

            osg::Vec3   ca = c + a ;
            ca.normalize() ;
            m_vertex_array->push_back( ca ) ;
        }
    }




    osg::IntArray*  new_index_array = new osg::IntArray() ;

    for(unsigned int i=0; i<num_indices; i+=3) {
        const int   a = (*m_index_array)[i] ;
        const int   b = (*m_index_array)[i+1] ;
        const int   c = (*m_index_array)[i+2] ;
        const int   ab = a < b ? vmap[a][b] : vmap[b][a] ;
        const int   bc = b < c ? vmap[b][c] : vmap[c][b] ;
        const int   ca = c < a ? vmap[c][a] : vmap[a][c] ;


        new_index_array->push_back(a) ;
        new_index_array->push_back(ab) ;
        new_index_array->push_back(ca) ;

        new_index_array->push_back(b) ;
        new_index_array->push_back(bc) ;
        new_index_array->push_back(ab) ;

        new_index_array->push_back(c) ;
        new_index_array->push_back(ca) ;
        new_index_array->push_back(bc) ;

        new_index_array->push_back(ab) ;
        new_index_array->push_back(bc) ;
        new_index_array->push_back(ca) ;
    }

    m_index_array = new_index_array ;




    dirtyDisplayList() ;
}
/* ....................................................................... */
/* ======================================================================= */
