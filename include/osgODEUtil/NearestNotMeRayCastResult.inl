/*!
 * @file NearestNotMeRayCastResult.inl
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2012 by Rocco Martino                            *
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

#ifndef _OSGODEUTIL_NEARESTNOTMERAYCASTRESULT_INL
#define _OSGODEUTIL_NEARESTNOTMERAYCASTRESULT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Collidable*
osgODEUtil::NearestNotMeRayCastResult::getMe(void)
{
    return m_me.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Collidable*
osgODEUtil::NearestNotMeRayCastResult::getMe(void) const
{
    return m_me.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODEUTIL_NEARESTNOTMERAYCASTRESULT_INL */
