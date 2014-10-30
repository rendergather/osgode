/*!
 * @file JointBreakCallback_serializer.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/JointBreakCallback>

#include <osgDB/Registry>

#include "real_serializer"
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( JointBreakCallback,
                         new osgODE::JointBreakCallback,
                         osgODE::JointBreakCallback,
                         "osg::Object osgODE::ODECallback osgODE::JointBreakCallback" )
{
    ADD_REAL_SERIALIZER(ForceLimit, FLT_MAX) ;
    ADD_REAL_SERIALIZER(TorqueLimit, FLT_MAX) ;
    ADD_BOOL_SERIALIZER(RemoveOnBreak, true) ;
    ADD_BOOL_SERIALIZER(Enabled, true) ;
}
/* ....................................................................... */
/* ======================================================================= */
