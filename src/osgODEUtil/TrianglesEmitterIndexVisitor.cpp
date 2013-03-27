/*!
 * @file TrianglesEmitterIndexVisitor.cpp
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
#include <osgODEUtil/TrianglesEmitterIndexVisitor>

#include <osgODE/Notify>

#include <osg/Geode>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterIndexVisitor::TrianglesEmitterIndexVisitor(TrianglesEmitterPrimitiveIndexFunctor::TriangleCallback& tri_cbk):
    m_triangle_callback(tri_cbk),
    m_accumulated_matrix(osg::Matrix::identity()),
    m_apply_transforms(true)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterIndexVisitor::TrianglesEmitterIndexVisitor(const TrianglesEmitterIndexVisitor& other):
    osg::NodeVisitor(other),
    m_triangle_callback(other.m_triangle_callback),
    m_accumulated_matrix(other.m_accumulated_matrix),
    m_apply_transforms(other.m_apply_transforms)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TrianglesEmitterIndexVisitor::~TrianglesEmitterIndexVisitor(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TrianglesEmitterIndexVisitor::apply(osg::Node& node)
{
    osg::Transform*     transform = node.asTransform() ;

    osg::Matrix     bck_matrix(m_accumulated_matrix) ;

    if( m_apply_transforms && transform ) {
        transform->computeLocalToWorldMatrix(m_accumulated_matrix, this) ;
    }


    osg::Geode* geode = node.asGeode() ;

    if(geode) {

        osgODEUtil::TrianglesEmitterPrimitiveIndexFunctor  functor(m_triangle_callback) ;
        functor.setMatrix(m_accumulated_matrix) ;

        for(unsigned int i=0; i<geode->getNumDrawables(); i++) {

            osg::Drawable*  drawable = geode->getDrawable(i) ;


            if( drawable->supports(functor) ) {

                drawable->accept(functor) ;

            } else {
                PS_FATAL("TrianglesEmitterIndexVisitor(%p): Drawable(%p) does not support PrimitiveIndexFunctor", this, drawable) ;
            }
        }
    }

    traverse(node) ;


    m_accumulated_matrix.set(bck_matrix) ;
}
/* ....................................................................... */
/* ======================================================================= */
