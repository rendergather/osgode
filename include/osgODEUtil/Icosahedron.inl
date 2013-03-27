/*!
 * @file Icosahedron.inl
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

#ifndef _OSGODEUTIL_ICOSAHEDRON_INL
#define _OSGODEUTIL_ICOSAHEDRON_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODEUtil::Icosahedron::setSmooth(bool smooth)
{
    if( smooth != m_smooth ) {
        m_smooth = smooth ;
        dirtyDisplayList() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODEUtil::Icosahedron::getSmooth(void) const
{
    return m_smooth ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Vec3Array*
osgODEUtil::Icosahedron::getVertexArray(void)
{
    return m_vertex_array.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::IntArray*
osgODEUtil::Icosahedron::getIndexArray(void)
{
    return m_index_array.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3Array*
osgODEUtil::Icosahedron::getVertexArray(void) const
{
    return m_vertex_array.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::IntArray*
osgODEUtil::Icosahedron::getIndexArray(void) const
{
    return m_index_array.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODEUTIL_ICOSAHEDRON_INL */
