/*!
 * @file TrianglesEmitterPrimitiveIndexFunctor.cpp
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
#include <osgODEUtil/TrianglesEmitterPrimitiveIndexFunctor>

#include <osgODE/Notify>

#include <osg/io_utils>
#include <iostream>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterPrimitiveIndexFunctor::TrianglesEmitterPrimitiveIndexFunctor(TriangleCallback& cbk):
    m_triangle_callback(cbk),
    m_vec3_array(NULL),
    m_array_count(0),
    m_current_mode((GLenum)-1),
    m_vertex_member(NULL),
    m_aux_array(new osg::Vec3Array()),
    m_matrix(osg::Matrix::identity())
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterPrimitiveIndexFunctor::TrianglesEmitterPrimitiveIndexFunctor(const TrianglesEmitterPrimitiveIndexFunctor& other):
    m_triangle_callback(other.m_triangle_callback),
    m_vec3_array(NULL),
    m_array_count(other.m_array_count),
    m_current_mode(other.m_current_mode),
    m_vertex_member(other.m_vertex_member),
    m_aux_array( new osg::Vec3Array(*other.m_aux_array) ),
    m_matrix( other.m_matrix )
{
    m_vec3_array = new osg::Vec3[m_array_count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(other.m_vec3_array[i]) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterPrimitiveIndexFunctor::~TrianglesEmitterPrimitiveIndexFunctor(void)
{
    delete [] m_vec3_array ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec2* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i].x(), vertices[i].y(), 0.0) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec3* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i]) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec4* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i].x(), vertices[i].y(), vertices[i].z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec2d* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i].x(), vertices[i].y(), 0.0) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec3d* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i]) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::setVertexArray(unsigned int count,const osg::Vec4d* vertices)
{
    delete [] m_vec3_array ;

    m_array_count = count ;

    m_vec3_array = new osg::Vec3[count] ;

    for(unsigned int i=0; i<m_array_count; i++) {
        m_vec3_array[i].set(vertices[i].x(), vertices[i].y(), vertices[i].z()) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::drawArrays(GLenum mode,GLint first,GLsizei count)
{
    PS_ASSERT1(m_vec3_array != NULL) ;

    this->begin(mode) ;

    const GLint last = first + count ;
    for(GLint i=first; i<last; i++) {
        this->vertex(i) ;
    }

    this->end() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::drawElements(GLenum mode,GLsizei count,const GLushort* indices)
{
    PS_ASSERT1(m_vec3_array != NULL) ;

    this->begin(mode) ;


    for(GLsizei i=0; i<count; i++) {
        this->vertex(indices[i]) ;
    }

    this->end() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::drawElements(GLenum mode,GLsizei count,const GLuint* indices)
{
    PS_ASSERT1(m_vec3_array != NULL) ;

    this->begin(mode) ;


    for(GLsizei i=0; i<count; i++) {
        this->vertex(indices[i]) ;
    }

    this->end() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::drawElements(GLenum mode,GLsizei count,const GLubyte* indices)
{
    PS_ASSERT1(m_vec3_array != NULL) ;

    this->begin(mode) ;


    for(GLsizei i=0; i<count; i++) {
        this->vertex(indices[i]) ;
    }

    this->end() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::begin(GLenum mode)
{
    PS_ASSERT1(m_current_mode == (unsigned)-1) ;

    m_current_mode = mode ;

    m_aux_array->clear() ;


    switch(mode)
    {
        case GL_POINTS:
            PS_FATAL("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_POINTS): GL_POINTS not suported", this) ;
            m_vertex_member = NULL ;
        break ;

        case GL_LINES:
            PS_FATAL("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_LINES): GL_LINES not suported", this) ;
            m_vertex_member = NULL ;
        break ;

        case GL_LINE_STRIP:
            PS_FATAL("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_LINE_STRIP): GL_LINE_STRIP not suported", this) ;
            m_vertex_member = NULL ;
        break ;

        case GL_LINE_LOOP:
            PS_FATAL("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_LINE_LOOP): GL_LINE_LOOP not suported", this) ;
            m_vertex_member = NULL ;
        break ;

        case GL_TRIANGLES:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_TRIANGLES)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLES ;
        break ;

        case GL_TRIANGLE_STRIP:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_TRIANGLE_STRIP)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLE_STRIP ;
        break ;

        case GL_TRIANGLE_FAN:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_TRIANGLE_FAN)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLE_FAN ;
        break ;

        case GL_QUADS:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_QUADS)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_QUADS ;
        break ;

        case GL_QUAD_STRIP:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_QUAD_STRIP)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_QUAD_STRIP ;
        break ;

        case GL_POLYGON:
            PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, GL_POLYGON)", this) ;
            m_vertex_member = &TrianglesEmitterPrimitiveIndexFunctor::_vertex_POLYGON ;
        break ;

        default:
            PS_FATAL("TrianglesEmitterPrimitiveIndexFunctor::begin(%p, %u): unknown mode", this, mode) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::vertex(unsigned int pos)
{
    PS_ASSERT1( m_vec3_array != NULL ) ;
    PS_ASSERT1( m_vertex_member != NULL ) ;
    PS_ASSERT1( m_current_mode != (unsigned)-1 ) ;

    (this->*m_vertex_member)(pos) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::end(void)
{
    PS_ASSERT1(m_current_mode != (unsigned)-1) ;

    PS_DBG3("TrianglesEmitterPrimitiveIndexFunctor::end(%p)", this) ;

    m_current_mode = (GLenum)-1 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLES(unsigned int pos)
{
    m_aux_array->push_back( osg::Vec3(m_vec3_array[pos]) ) ;

    if( m_aux_array->size() == 3 ) {

        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[1], (*m_aux_array)[2] ) ;

        m_aux_array->clear() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLE_STRIP(unsigned int pos)
{
    m_aux_array->push_back( m_vec3_array[pos] ) ;


    switch( m_aux_array->size() )
    {
        case 3:
            _emitTriangle((*m_aux_array)[0], (*m_aux_array)[1], (*m_aux_array)[2]) ;
        break ;

        case 4:
            _emitTriangle((*m_aux_array)[2], (*m_aux_array)[1], (*m_aux_array)[3]) ;

            (*m_aux_array)[0] = (*m_aux_array)[2] ;
            (*m_aux_array)[1] = (*m_aux_array)[3] ;

            m_aux_array->resize(2) ;
        break ;

        default:
        break ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_TRIANGLE_FAN(unsigned int pos)
{
    osg::Vec3   vertex(m_vec3_array[pos]) ;


    if( m_aux_array->size() == 2 ) {

        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[1], vertex ) ;

        (*m_aux_array)[1] = vertex ;

    } else {
        m_aux_array->push_back( vertex ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_QUADS(unsigned int pos)
{
    m_aux_array->push_back( osg::Vec3(m_vec3_array[pos]) ) ;

    if( m_aux_array->size() == 4 ) {
        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[1], (*m_aux_array)[2] ) ;
        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[2], (*m_aux_array)[3] ) ;

        m_aux_array->clear() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_QUAD_STRIP(unsigned int pos)
{
    m_aux_array->push_back( osg::Vec3(m_vec3_array[pos]) ) ;

    if( m_aux_array->size() == 4 ) {

        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[1], (*m_aux_array)[2] ) ;
        _emitTriangle( (*m_aux_array)[2], (*m_aux_array)[1], (*m_aux_array)[3] ) ;

        (*m_aux_array)[0] = (*m_aux_array)[2] ;
        (*m_aux_array)[1] = (*m_aux_array)[3] ;

        m_aux_array->resize(2) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_vertex_POLYGON(unsigned int pos)
{
    osg::Vec3   vertex(m_vec3_array[pos]) ;


    if( m_aux_array->size() == 2 ) {

        _emitTriangle( (*m_aux_array)[0], (*m_aux_array)[1], vertex ) ;

        (*m_aux_array)[1] = vertex ;

    } else {
        m_aux_array->push_back( vertex ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterPrimitiveIndexFunctor::_emitTriangle(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3)
{
    m_triangle_callback(    v1 * m_matrix,
                            v2 * m_matrix,
                            v3 * m_matrix ) ;
}
/* ....................................................................... */
/* ======================================================================= */
