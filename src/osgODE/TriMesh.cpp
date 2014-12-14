/*!
 * @file TriMesh.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2012 by Rocco Martino                            *
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
#include <osgODE/TriMesh>
#include <osgODE/Space>
#include <osgODE/Notify>

#include <osg/BoundingBox>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
TriMesh::TriMesh(void):
    m_size( osg::Vec3(1,1,1) )
{
    m_ODE_geom = dCreateTriMesh(NULL, dGeomTriMeshDataCreate(), NULL, NULL, NULL) ;


    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TriMesh::TriMesh(const TriMesh& other, const osg::CopyOp& copyop):
    Collidable(other, copyop),
    m_size( other.m_size ),
    m_last_transform( other.m_last_transform )
{
    m_ODE_geom = dCreateTriMesh(NULL, dGeomTriMeshDataCreate(), NULL, NULL, NULL) ;


    setVertexArray( osg::clone(other.getVertexArray(), copyop) ) ;
    setIndexArray( osg::clone(other.getIndexArray(), copyop) ) ;


    build(false) ;


    setSize( other.getSize() ) ;



    _cloneODETriMesh(other.m_ODE_geom, m_ODE_geom) ;

    dGeomSetBody(m_ODE_geom, m_ODE_body) ;

    dGeomSetData(m_ODE_geom, this) ;


    setCollidableOffset(other.getCollidableOffset()) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
TriMesh::~TriMesh(void)
{
    destroy() ;

    dGeomDestroy(m_ODE_geom) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::setSize(const osg::Vec3& size)
{
    m_size = size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Vec3
TriMesh::getSize(void) const
{
    return m_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::setMass(ooReal mass)
{
    PS_DBG2("oo::TriMesh::setMass(%p, mass=%f)", this, mass) ;



    // See RigidBody::setMass(mass, size, type_class)
//     bool    kinematic_flag = this->RigidBody::getKinematic() ;


    //! @bug    TriMesh::setMass(ooReal) doesn't work
//     dMass   m ;
// 
//     dMassSetTrimeshTotal(&m, mass, m_ODE_geom) ;
//     dBodySetMass(m_ODE_body, &m) ;



//     this->RigidBody::setKinematic(kinematic_flag) ;



    this->osgODE::RigidBody::setMass(mass, getSize(), dBoxClass) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::setDensity(ooReal density)
{
    PS_DBG2("oo::TriMesh::setDensity(%p, density=%f)", this, density) ;



    // check for negative mass
    if( density <= 0.0 ) {

        density = fabs( density ) ;

        m_mass_negative = true ;

        this->setGravityMode(false) ;


    } else {

        m_mass_negative = false ;
    }




    // See RigidBody::setMass(mass, size, type_class)
    bool    kinematic_flag = this->RigidBody::getKinematic() ;


    dMass   m ;

    dMassSetTrimesh(&m, density, m_ODE_geom) ;
    dBodySetMass(m_ODE_body, &m) ;



    this->RigidBody::setKinematic(kinematic_flag) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
TriMesh::build(bool compute_size)
{
    PS_DBG2("oo::TriMesh::build(%p)", this) ;


    if( ! m_vertex_array.valid() ) {
        PS_FATAL("oo::TriMesh::build(%p): m_vertex_array == NULL", this) ;
        return false ;
    }



    if( ! m_index_array.valid() ) {
        PS_WARN("oo::TriMesh::build(%p): m_index_array == NULL, building now assuming array of triangles", this) ;

        m_index_array = new osg::IntArray() ;

        for(unsigned int i=0; i<m_vertex_array->getNumElements(); i++) {
            m_index_array->push_back(i) ;
        }
    }



    if( compute_size ) {
        osg::BoundingBox    bb ;

        for( unsigned int i=0; i<m_vertex_array->size(); i++ ) {
            bb.expandBy( (*m_vertex_array)[i] ) ;
        }

        if( bb.valid() ) {
            osg::Vec3   vmin(  bb.xMin(), bb.yMin(), bb.zMin() ) ;
            osg::Vec3   vmax(  bb.xMax(), bb.yMax(), bb.zMax() ) ;

            setSize( vmax - vmin ) ;
        }
    }





    /*!
     * @todo    single/ooReal in TriMesh::build
     */


    dGeomTriMeshDataBuildSingle1(
                                    dGeomTriMeshGetData(m_ODE_geom),
                                    m_vertex_array->getDataPointer(),
                                    3 * sizeof(float),
                                    m_vertex_array->getNumElements(),
                                    m_index_array->getDataPointer(),
                                    m_index_array->getNumElements(),
                                    3 * sizeof(unsigned int),
                                    NULL
                                ) ;



    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::destroy(void)
{
    dGeomTriMeshDataDestroy( dGeomTriMeshGetData(m_ODE_geom) ) ;
    dGeomTriMeshSetData( m_ODE_geom, dGeomTriMeshDataCreate() ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::update(ooReal step_size)
{
    this->Collidable::update(step_size) ;

    m_last_transform = this->getMatrix() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::postUpdate(ooReal step_size)
{
    this->Collidable::postUpdate(step_size) ;

    dMatrix4    last_transform ;

    last_transform[0 ] = m_last_transform(0,0) ;
    last_transform[1 ] = m_last_transform(1,0) ;
    last_transform[2 ] = m_last_transform(2,0) ;
    last_transform[3 ] = m_last_transform(3,0) ;

    last_transform[4 ] = m_last_transform(0,1) ;
    last_transform[5 ] = m_last_transform(1,1) ;
    last_transform[6 ] = m_last_transform(2,1) ;
    last_transform[7 ] = m_last_transform(3,1) ;

    last_transform[8 ] = m_last_transform(0,2) ;
    last_transform[9 ] = m_last_transform(1,2) ;
    last_transform[10] = m_last_transform(2,2) ;
    last_transform[11] = m_last_transform(3,2) ;

    last_transform[12] = m_last_transform(3,0) ;
    last_transform[13] = m_last_transform(3,1) ;
    last_transform[14] = m_last_transform(3,2) ;
    last_transform[15] = m_last_transform(3,3) ;

    dGeomTriMeshSetLastTransform(m_ODE_geom, last_transform) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
TriMesh::_cloneODETriMesh(dGeomID src, dGeomID dst)
{
    PS_DBG2("TriMesh::_cloneODETriMesh(src=%p, dest=%p)", src, dst) ;







    dGeomTriMeshSetCallback(            dst, dGeomTriMeshGetCallback(           src)) ;
    dGeomTriMeshSetArrayCallback(       dst, dGeomTriMeshGetArrayCallback(      src)) ;
    dGeomTriMeshSetRayCallback(         dst, dGeomTriMeshGetRayCallback(        src)) ;
    dGeomTriMeshSetTriMergeCallback(    dst, dGeomTriMeshGetTriMergeCallback(   src)) ;



    Collidable::_cloneODEGeom(src, dst) ;
}
/* ....................................................................... */
/* ======================================================================= */
