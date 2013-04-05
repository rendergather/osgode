/*!
 * @file Container.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 - 2013 by Rocco Martino                            *
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
#include <osgODE/Container>
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
Container::Container(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Container::Container(const Container& other, const osg::CopyOp& copyop):
    ODEObject(other, copyop)
{
    ObjectList::const_iterator  itr = other.m_object_list.begin() ;
    ObjectList::const_iterator  itr_end = other.m_object_list.end() ;

    while( itr != itr_end ) {
        addObject( (*itr++).get() ) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Container::~Container(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
Container*
Container::asContainer(void)
{
    return this ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::update(double step_size)
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;

    while( itr != itr_end ) {
        (*itr++)->update(step_size) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::postUpdate(double step_size)
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;

    while( itr != itr_end ) {
        (*itr++)->postUpdate(step_size) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Container::addToWorldInternal(World* world)
{
    PS_DBG2("osgODE::Container::addToWorldInternal(%p, world=%p)", this, world) ;



    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;


    while( itr != itr_end ) {
        ODEObject*  current = *itr++ ;

        if( current->addToWorldInternal(world) ) {
            PS_DBG2("osgODE::Container::addToWorldInternal(%p, world=%p): %p added to world",
                            this, world, current) ;

            current->setWorldInternal(world) ;

        } else {
            PS_FATAL("osgODE::Container::addToWorldInternal(%p, world=%p): cannot add %s::%s (%p)",
                            this, world, current->libraryName(), current->className(), current) ;

            current->setWorldInternal(NULL) ;
        }
    }



    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
Container::removeFromWorldInternal(World* world)
{
    PS_DBG2("osgODE::Container::removeFromWorldInternal(%p, world=%p)", this, world) ;


    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;


    while( itr != itr_end ) {
        ODEObject*  current = *itr++ ;

        if( current->removeFromWorldInternal(world) ) {
            PS_DBG2("osgODE::Container::removeFromWorldInternal(%p, world=%p): %p removed from world",
                            this, world, current) ;

            current->setWorldInternal(NULL) ;

        } else {
            PS_FATAL("osgODE::Container::removeFromWorldInternal(%p, world=%p): cannot remove %s::%s (%p)",
                            this, world, current->libraryName(), current->className(), current) ;
        }
    }


    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::accept(osg::NodeVisitor& nv)
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;

    while( itr != itr_end ) {
        (*itr++)->accept(nv) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const osg::BoundingSphere&
Container::getBound(void) const
{
    ObjectList::const_iterator  itr = m_object_list.begin() ;
    ObjectList::const_iterator  itr_end = m_object_list.end() ;

    const_cast<osg::BoundingSphere&>(m_bounding_sphere).init() ;

    while( itr != itr_end ) {
        const_cast<osg::BoundingSphere&>(m_bounding_sphere).expandBy( (*itr++)->getBound() ) ;
    }


    return m_bounding_sphere ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::addObject(ODEObject* obj)
{
    PS_DBG2("osgODE::Container::addObject(%p, obj=%p)", this, obj) ;

    if( ! obj ) {
        return ;
    }


    m_object_list.push_back(obj) ;


    World*  world = getWorld() ;


    if( world ) {
        if( obj->addToWorldInternal(world) ) {
            PS_DBG2("osgODE::Container::addToWorldInternal(%p, world=%p): %p added to world",
                            this, world, obj) ;

            obj->setWorldInternal(world) ;
        } else {
            PS_FATAL("osgODE::Container::addToWorldInternal(%p, world=%p): cannot add %s::%s (%p)",
                            this, world, obj->libraryName(), obj->className(), obj) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::removeObject(unsigned int idx, bool preserve_order)
{
    if( idx >= m_object_list.size() ) {
        PS_FATAL("osgODE::Container::removeObject(%p, idx=%u): index out of range", this, idx) ;
        return ;
    }


    osg::ref_ptr<ODEObject> obj = m_object_list[idx].get() ;


    PS_DBG2("osgODE::Container::removeObject(%p, obj=%p)", this, obj.get()) ;


    if( preserve_order ) {
        m_object_list.erase( m_object_list.begin() + idx ) ;
    } else {
        unsigned int    last = m_object_list.size() - 1 ;
        m_object_list[idx] = m_object_list[ last ] ;
        m_object_list.resize(last) ;
    }


    World*  world = getWorld() ;


    if( world ) {
        if( obj->removeFromWorldInternal(world) ) {
            PS_DBG2("osgODE::Container::removeFromWorldInternal(%p, world=%p): %p removed from world",
                            this, world, obj.get()) ;

        } else {
            PS_FATAL("osgODE::Container::removeFromWorldInternal(%p, world=%p): cannot remove %s::%s (%p)",
                            this, world, obj->libraryName(), obj->className(), obj.get()) ;
        }
    }

    obj->setWorldInternal(NULL) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osgODE::ODEObject*
Container::getObject(unsigned int idx)
{
    if( idx >= m_object_list.size() ) {
        PS_FATAL("osgODE::Container::removeObject(%p, idx=%u): index out of range", this, idx) ;
        return NULL ;
    }


    return m_object_list[idx].get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
const osgODE::ODEObject*
Container::getObject(unsigned int idx) const
{
    if( idx >= m_object_list.size() ) {
        PS_FATAL("osgODE::Container::removeObject(%p, idx=%u): index out of range", this, idx) ;
        return NULL ;
    }


    return m_object_list[idx].get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
Container::getObjectIDX(ODEObject* obj) const
{
    ObjectList::const_iterator  itr = m_object_list.begin() ;
    ObjectList::const_iterator  itr_end = m_object_list.end() ;

    unsigned int    found = 0 ;

    while( itr != itr_end ) {
        if( (*itr++).get() == obj ) {
            return found ;
        }

        ++found ;

    }

    return ODEOBJECT_NOT_FOUND ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::callUpdateCallbackInternal(void)
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;


    ODECallback*    cbk = getUpdateCallback() ;

    if( cbk ) {
        (*cbk)(this) ;
    }



    while( itr != itr_end ) {
        (*itr++)->callUpdateCallbackInternal() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
Container::callPostUpdateCallbackInternal(void)
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;


    ODECallback*    cbk = getPostUpdateCallback() ;

    if( cbk ) {
        (*cbk)(this) ;
    }



    while( itr != itr_end ) {
        (*itr++)->callPostUpdateCallbackInternal() ;
    }
}
/* ....................................................................... */
/* ======================================================================= */
