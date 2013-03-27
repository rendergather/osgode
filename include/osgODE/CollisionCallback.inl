/*!
 * @file CollisionCallback.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 by Rocco Martino                                   *
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

#ifndef _OSGODE_COLLISIONCALLBACK_INL
#define _OSGODE_COLLISIONCALLBACK_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CollisionCallback::setNestedCallback(osgODE::CollisionCallback* cbk)
{
    m_nested_callback = cbk ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CollisionCallback::addNestedCallback(osgODE::CollisionCallback* cbk)
{
    if(cbk) {
        if( m_nested_callback.valid() ) {
            m_nested_callback->addNestedCallback(cbk) ;
        } else {
            setNestedCallback(cbk) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CollisionCallback::removeNestedCallback(osgODE::CollisionCallback* cbk)
{
    if(cbk) {
        if( m_nested_callback.get() != cbk ) {
            if( m_nested_callback.valid() ) {
                m_nested_callback->removeNestedCallback(cbk) ;
            }
        } else {
            m_nested_callback = m_nested_callback->getNestedCallback() ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::CollisionCallback*
osgODE::CollisionCallback::getNestedCallback(void)
{
    return m_nested_callback.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::CollisionCallback*
osgODE::CollisionCallback::getNestedCallback(void) const
{
    return m_nested_callback.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CollisionCallback::traverse(Collidable* owner, Collidable* other, NearCallback* near_callback)
{
    if(m_nested_callback.valid()) {
        (*m_nested_callback)(owner, other, near_callback) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_COLLISIONCALLBACK_INL */
