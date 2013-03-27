/*!
 * @file NearestRayCastResult.inl
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

#ifndef _OSGODEUTIL_NEARESTRAYCASTRESULT_INL
#define _OSGODEUTIL_NEARESTRAYCASTRESULT_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline bool
osgODEUtil::NearestRayCastResult::hasHit(void) const
{
    return m_collidable.valid() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Collidable*
osgODEUtil::NearestRayCastResult::getCollidable(void)
{
    return m_collidable.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Collidable*
osgODEUtil::NearestRayCastResult::getCollidable(void) const
{
    return m_collidable.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODEUtil::NearestRayCastResult::getPosition(void) const
{
    return m_position ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osg::Vec3&
osgODEUtil::NearestRayCastResult::getNormal(void) const
{
    return m_normal ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const double&
osgODEUtil::NearestRayCastResult::getDistance(void) const
{
    return m_distance ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODEUTIL_NEARESTRAYCASTRESULT_INL */
