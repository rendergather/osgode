/*!
 * @file Container.inl
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

#ifndef _OSGODE_CONTAINER_INL
#define _OSGODE_CONTAINER_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
// inline bool
// osgODE::Container::removeObject(osgODE::ODEObject* obj, bool preserve_order)
// {
//     if( ! obj ) {
//         return false ;
//     }
// 
//     unsigned int    idx = getObjectIDX(obj) ;
// 
//     if( idx != ODEOBJECT_NOT_FOUND ) {
//         return removeObject(idx, preserve_order) ;
//     }
// 
//     return false ;
// }
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::Container::hasObject(osgODE::ODEObject* obj) const
{
    return getObjectIDX(obj) != ODEOBJECT_NOT_FOUND ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::Container::setObjectList(const ObjectList& object_list)
{
    clear() ;

    ObjectList::const_iterator  itr = object_list.begin() ;
    ObjectList::const_iterator  itr_end = object_list.end() ;


    while( itr != itr_end ) {

        addObject( *itr++ ) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Container::ObjectList&
osgODE::Container::getObjectList(void)
{
    return m_object_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Container::ObjectList&
osgODE::Container::getObjectList(void) const
{
    return m_object_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
template<class T>
inline void
osgODE::Container::traverseObjects(T& t, void (T::*member)(ODEObject*))
{
    ObjectList::iterator    itr = m_object_list.begin() ;
    ObjectList::iterator    itr_end = m_object_list.end() ;


    while( itr != itr_end ) {

        (t.*member)( *itr++ ) ;

    }
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_CONTAINER_INL */
