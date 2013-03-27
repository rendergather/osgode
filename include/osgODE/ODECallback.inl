/*!
 * @file ODECallback.inl
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

#ifndef _OSGODE_ODECALLBACK_INL
#define _OSGODE_ODECALLBACK_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::ODECallback::setNestedCallback(osgODE::ODECallback* cbk)
{
    m_nested_callback = cbk ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::ODECallback::addNestedCallback(osgODE::ODECallback* cbk)
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
osgODE::ODECallback::removeNestedCallback(osgODE::ODECallback* cbk)
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
inline osgODE::ODECallback*
osgODE::ODECallback::getNestedCallback(void)
{
    return m_nested_callback.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::ODECallback*
osgODE::ODECallback::getNestedCallback(void) const
{
    return m_nested_callback.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::ODECallback::traverse(osgODE::ODEObject* object)
{
    if(m_nested_callback.valid()) {
        (*m_nested_callback)(object) ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_ODECALLBACK_INL */
