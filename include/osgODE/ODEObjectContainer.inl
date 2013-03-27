/*!
 * @file ODEObjectContainer.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2012 by Rocco Martino                                   *
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

#ifndef _OSGODE_ODEOBJECTCONTAINER_INL
#define _OSGODE_ODEOBJECTCONTAINER_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODE::ODEObjectContainer::removeObject(osgODE::ODEObject* obj, bool preserve_order)
{
    unsigned int    idx = getObjectIDX(obj) ;

    if( idx != ODEOBJECT_NOT_FOUND ) {
        removeObject(idx, preserve_order) ;
        return true ;
    }

    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::ODEObjectContainer::ObjectList&
osgODE::ODEObjectContainer::getObjectList(void) const
{
    return m_object_list ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_ODEOBJECTCONTAINER_INL */
