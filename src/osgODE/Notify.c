/*!
 * @file Notify.c
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

/* ======================================================================= */
#include <osgODE/Notify>
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
psLogLevelEnum  psLogLevel = PS_DEFAULT_LOG_LEVEL ;
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
psSetLogLevel(psLogLevelEnum ll)
{
    psLogLevel = ll ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
psLogLevelEnum
psGetLogLevel(void)
{
    return psLogLevel ;
}
/* ....................................................................... */
/* ======================================================================= */
