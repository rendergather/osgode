/*!
 * @file CharacterController.inl
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _OSGODE_CHARACTERCONTROLLER_INL
#define _OSGODE_CHARACTERCONTROLLER_INL




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterController::setKeys(const Keys& keys)
{
    m_keys = keys ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::CharacterController::Keys&
osgODE::CharacterController::getKeys(void)
{
    return m_keys ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::CharacterController::Keys&
osgODE::CharacterController::getKeys(void) const
{
    return m_keys ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODE::CharacterController::setSensitivity(float f)
{
    m_sensitivity = f ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline float
osgODE::CharacterController::getSensitivity(void) const
{
    return m_sensitivity ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osgODE::Character*
osgODE::CharacterController::getCharacter(void)
{
    return m_character.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline const osgODE::Character*
osgODE::CharacterController::getCharacter(void) const
{
    return m_character.get() ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_CHARACTERCONTROLLER_INL */
