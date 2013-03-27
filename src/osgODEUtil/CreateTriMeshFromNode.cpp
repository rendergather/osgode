/*!
 * @file CreateTriMeshFromNode.cpp
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
#include <osgODEUtil/CreateTriMeshFromNode>
#include <osgODEUtil/TrianglesEmitterIndexVisitor>

#include <osgODE/TriMesh>

#include <iostream>
#include <osg/io_utils>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace
{
    class OSG_EXPORT TriMeshData: public osgODEUtil::TrianglesEmitterPrimitiveIndexFunctor::TriangleCallback
    {
    public:
            TriMeshData(void):
                m_indices(new osg::IntArray()),
                m_vertices(new osg::Vec3Array()) {}

            TriMeshData(const TriMeshData& other):
                m_indices(other.m_indices),
                m_vertices(other.m_vertices) {}

        virtual void    operator()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3)
        {
            m_indices->push_back( m_vertices->size() ) ; m_vertices->push_back(v1) ;
            m_indices->push_back( m_vertices->size() ) ; m_vertices->push_back(v2) ;
            m_indices->push_back( m_vertices->size() ) ; m_vertices->push_back(v3) ;
        }


        osg::IntArray*  getIndexArray(void) { return m_indices.get() ; }
        osg::Vec3Array* getVertexArray(void) { return m_vertices.get() ; }

    private:
        osg::ref_ptr<osg::IntArray>     m_indices ;
        osg::ref_ptr<osg::Vec3Array>    m_vertices ;
    } ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::TriMesh*
osgODEUtil::createTriMeshFromNode(osg::Node* node, osg::NodeVisitor::TraversalMode traversal_mode, bool apply_transform)
{
    TriMeshData     tmd ;

    osg::ref_ptr<TrianglesEmitterIndexVisitor>  visitor = new TrianglesEmitterIndexVisitor(tmd) ;

    visitor->setTraversalMode(traversal_mode) ;
    visitor->setApplyTransforms(apply_transform) ;

    node->accept(*visitor) ;


    osgODE::TriMesh*    trimesh = new osgODE::TriMesh() ;
    trimesh->setVertexArray(tmd.getVertexArray()) ;
    trimesh->setIndexArray(tmd.getIndexArray()) ;
    trimesh->build() ;

    return trimesh ;
}
/* ....................................................................... */
/* ======================================================================= */
