/*!
 * @file RagDoll.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/RagDoll>
#include <osgODE/Collidable>
#include <osgODE/Notify>

#include <osgAnimation/Bone>
#include <osgAnimation/BoneMapVisitor>

#include <iostream>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
RagDoll::RagDoll(void):
    m_dirty     ( true )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RagDoll::RagDoll(const RagDoll& other, const osg::CopyOp& copyop):
    Container   ( other, copyop ),
    m_dirty     ( true ),
    m_skeleton  ( osg::clone(other.m_skeleton.get(), copyop) )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
RagDoll::~RagDoll(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RagDoll::init(void)
{
    _initRagDoll() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RagDoll::postUpdate( double step_size )
{
    if( m_dirty ) {

        init() ;

        m_dirty = false ;
    }

    if( m_skeleton.valid() ) {

        PS_ASSERT1( m_root_bone.valid() ) ;

        m_skeleton->setMatrix( m_root_body->getMatrix() ) ;
        m_skeleton->dirtyBound() ;
    }

    this->Container::postUpdate( step_size ) ;




    const ObjectList&   object_list = getObjectList() ;

    m_bounding_sphere.init() ;

    for( unsigned int i=0; i<object_list.size(); i++ ) {

        ODEObject*  obj = object_list[i].get() ;

        RigidBody*  body        = obj->asRigidBody() ;
        Collidable* collidable  = obj->asCollidable() ;


        if( collidable ) {

            const osg::Vec3 half_size = collidable->getSize() * 0.5 ;
            const osg::Vec3 position = collidable->getPosition() ;

            m_bounding_sphere.expandBy( osg::BoundingBox( position - half_size, position + half_size ) ) ;

        } else {

            if( body ) {
                m_bounding_sphere.expandBy( body->getPosition() ) ;
            }
        }

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const osg::BoundingSphere&
RagDoll::getBound(void) const
{
//     return m_bounding_sphere ;


    if( m_skeleton.valid() ) {
        return m_skeleton->getBound() ;
    }

    return this->Container::getBound() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RagDoll::accept( osg::NodeVisitor& nv )
{
    if( m_skeleton.valid() ) {
        m_skeleton->accept(nv) ;
    }

    this->Container::accept(nv) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
    class MyBoneUpdate: public osg::NodeCallback
    {
    public:
        MyBoneUpdate(osgODE::RigidBody* body, const osg::Matrix& offset, osgAnimation::Skeleton* skeleton):
            m_body      ( body ),
            m_offset    ( offset),
            m_skeleton  ( skeleton) {}



        virtual void    operator()(osg::Node* node, osg::NodeVisitor* nv)
        {
            PS_ASSERT1( dynamic_cast<osgAnimation::Bone*>( node ) ) ;
            PS_ASSERT1( m_skeleton.valid() ) ;

            osgAnimation::Bone* bone = static_cast<osgAnimation::Bone*>( node ) ;

            bone->dirtyBound() ;


            osg::Matrix m = m_offset * m_body->getMatrix() ;

            bone->setMatrixInSkeletonSpace( m * osg::Matrix::inverse( m_skeleton->getMatrix() ) ) ;

            osgAnimation::Bone* parent = bone->getBoneParent() ;
            if( parent ) {
                bone->setMatrix( m * osg::Matrix::inverse( parent->getMatrixInSkeletonSpace() ) ) ;
            } else {
                bone->setMatrix( bone->getMatrixInSkeletonSpace() ) ;
            }


            traverse(node, nv) ;
        }

    private:
        osg::ref_ptr<osgODE::RigidBody>         m_body ;
        osg::Matrix                             m_offset ;
        osg::ref_ptr<osgAnimation::Skeleton>    m_skeleton ;
    } ;
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
RagDoll::_initRagDoll(void)
{
    PS_DBG2("osgODE::RagDoll::_initRagDoll(%p)", this) ;


    /*
     * [1] collect the bodies
     */

    typedef std::vector<RigidBody* > BodyList ;

    BodyList body_list ;

    {

        const ObjectList&   object_list = getObjectList() ;


        for( unsigned int i=0; i< object_list.size(); i++ ) {

            RigidBody*  body = object_list[i]->asRigidBody() ;

            if( body ) {
                body_list.push_back( body ) ;


                const unsigned int  num_children = body->getMatrixTransform()->getNumChildren() ;

                if( num_children != 0 ) {
                    PS_WARN("osgODE::RagDoll::_initRagDoll(%p): body \"%s\" has %u children. Removing", this, body->getName().c_str(), num_children) ;

                    body->getMatrixTransform()->removeChildren( 0, num_children ) ;
                }
            }
        }
    }









    /*
     * [2] collect the bones
     */

    osgAnimation::BoneMap   bone_map ;

    {
        osg::ref_ptr<osgAnimation::BoneMapVisitor>  bmv = new osgAnimation::BoneMapVisitor() ;

        PS_ASSERT1( m_skeleton.valid() ) ;

        m_skeleton->accept( *bmv ) ;

        bone_map = bmv->getBoneMap() ;
    }








    /*
     * [3] find the bone named "Root"
     */
    m_root_bone = bone_map["Root"].get() ;
    PS_ASSERT1( m_root_bone.valid() ) ;








    /*
     * [4] create the callbacks
     */
    m_root_body = NULL ;

    for( unsigned int i = 0;  i < body_list.size();  i++ ) {

        RigidBody*  body = body_list[i] ;

        if( body->getName() == "Root" ) {
            m_root_body = body ;
        }



        osgAnimation::Bone* bone = bone_map[ body->getName() ] ;

        if( ! bone ) {
            PS_WARN("osgODE::RagDoll::_initRagDoll(%p): cannot find bone \"%s\"", this, body->getName().c_str()) ;

            continue ;
        }

        bone->setUpdateCallback(
            new MyBoneUpdate(
                body,
                bone->getMatrixInSkeletonSpace() * osg::Matrix::inverse( body->getMatrix() ),
                m_skeleton.get()
            )
        ) ;
    }


    PS_ASSERT1( m_root_body.valid() ) ;
}
/* ....................................................................... */
/* ======================================================================= */
