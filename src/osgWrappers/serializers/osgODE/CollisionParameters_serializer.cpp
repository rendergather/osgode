/*!
 * @file CollisionParameters_serializer.cpp
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
#include <osgODE/ODE>
#include <osgODE/CollisionParameters>

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
REGISTER_OBJECT_WRAPPER( CollisionParameters,
                         new osgODE::CollisionParameters,
                         osgODE::CollisionParameters,
                         "osg::Object osgODE::CollisionParameters" )
{
    ADD_INT_SERIALIZER( Mode, dContactApprox1 ) ;
    ADD_REAL_SERIALIZER( Mu, 1.0 ) ;
    ADD_REAL_SERIALIZER( Mu2, 1.0 ) ;
    ADD_REAL_SERIALIZER( Bounce, 0.0 ) ;
    ADD_REAL_SERIALIZER( BounceVel, 0.0 ) ;
    ADD_REAL_SERIALIZER( SoftERP, 1.0 ) ;
    ADD_REAL_SERIALIZER( SoftCFM, 0.0 ) ;
    ADD_REAL_SERIALIZER( Motion1, 0.0 ) ;
    ADD_REAL_SERIALIZER( Motion2, 0.0 ) ;
    ADD_REAL_SERIALIZER( MotionN, 0.0 ) ;
    ADD_REAL_SERIALIZER( Slip1, 0.0 ) ;
    ADD_REAL_SERIALIZER( Slip2, 0.0 ) ;
    ADD_UINT_SERIALIZER( MaxContactNum, 8 ) ;
    ADD_BOOL_SERIALIZER( CollideConnected, false ) ;
    ADD_BOOL_SERIALIZER( CollisionsEnabled, true ) ;
}
/* ....................................................................... */
/* ======================================================================= */
