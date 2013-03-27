/*!
 * @file AllRayCastResult.inl
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

#ifndef _OSGODEUTIL_ALLRAYCASTRESULT_INL
#define _OSGODEUTIL_ALLRAYCASTRESULT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODEUtil::AllRayCastResult::hasHit(void) const
{
    return ! m_collision_list.empty() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODEUtil::AllRayCastResult::CollisionList&
osgODEUtil::AllRayCastResult::getCollisionList(void) const
{
    return m_collision_list ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODEUtil::AllRayCastResult::CollisionList&
osgODEUtil::AllRayCastResult::getCollisionList(void)
{
    return m_collision_list ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODEUTIL_ALLRAYCASTRESULT_INL */
