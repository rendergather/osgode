/*!
 * @file StaticWorld.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2010 - 2014 by Rocco Martino                            *
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
#include <osgODE/StaticWorld>
#include <osgODE/Notify>

#include <OpenThreads/Thread>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
StaticWorld::StaticWorld(void)
{
    OpenThreads::Thread::Init() ;

    dInitODE2(0) ;
    dAllocateODEDataForThread( (unsigned int) dAllocateMaskAll ) ;


    m_ODE_world = dWorldCreate() ;

    dWorldSetGravity                        ( m_ODE_world, 0.0f, 0.0f, -9.80665f ) ;
    dWorldSetMaxAngularSpeed                ( m_ODE_world, FLT_MAX ) ;
    dWorldSetLinearDamping                  ( m_ODE_world, 0.01f ) ;
    dWorldSetAngularDamping                 ( m_ODE_world, 0.01f ) ;
    dWorldSetLinearDampingThreshold         ( m_ODE_world, 0.01f ) ;
    dWorldSetAngularDampingThreshold        ( m_ODE_world, 0.01f ) ;
    dWorldSetAutoDisableLinearThreshold     ( m_ODE_world, 0.1f ) ;
    dWorldSetAutoDisableAngularThreshold    ( m_ODE_world, 0.1f ) ;
    dWorldSetAutoDisableFlag                ( m_ODE_world, 0 ) ;
    dWorldSetAutoDisableSteps               ( m_ODE_world, 180 ) ;
    dWorldSetAutoDisableTime                ( m_ODE_world, 3.0f ) ;
    dWorldSetAutoDisableAverageSamplesCount ( m_ODE_world, 6 ) ;



    dWorldSetERP(m_ODE_world, 0.2f) ;
    dWorldSetCFM(m_ODE_world, 0.00001f) ;

    dWorldSetContactMaxCorrectingVel( m_ODE_world, FLT_MAX ) ;
    dWorldSetContactSurfaceLayer( m_ODE_world, 0.0f ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
StaticWorld::StaticWorld(const StaticWorld& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop)
{
    m_ODE_world = dWorldCreate() ;

    dWorldSetGravity                        ( m_ODE_world, 0.0, 0.0, -9.80665f ) ;
    dWorldSetMaxAngularSpeed                ( m_ODE_world, FLT_MAX ) ;
    dWorldSetLinearDamping                  ( m_ODE_world, 0.01f ) ;
    dWorldSetAngularDamping                 ( m_ODE_world, 0.01f ) ;
    dWorldSetLinearDampingThreshold         ( m_ODE_world, 0.01f ) ;
    dWorldSetAngularDampingThreshold        ( m_ODE_world, 0.01f ) ;
    dWorldSetAutoDisableLinearThreshold     ( m_ODE_world, 0.1f ) ;
    dWorldSetAutoDisableAngularThreshold    ( m_ODE_world, 0.1f ) ;
    dWorldSetAutoDisableFlag                ( m_ODE_world, 0 ) ;
    dWorldSetAutoDisableSteps               ( m_ODE_world, 180 ) ;
    dWorldSetAutoDisableTime                ( m_ODE_world, 3.0f ) ;
    dWorldSetAutoDisableAverageSamplesCount ( m_ODE_world, 6 ) ;



    dWorldSetERP(m_ODE_world, 0.2f) ;
    dWorldSetCFM(m_ODE_world, 0.00001f) ;

    dWorldSetContactMaxCorrectingVel( m_ODE_world, FLT_MAX ) ;
    dWorldSetContactSurfaceLayer( m_ODE_world, 0.0f ) ;
}
/* ....................................................................... */
/* ======================================================================= */





/* ======================================================================= */
/* ....................................................................... */
StaticWorld::~StaticWorld(void)
{
    dWorldDestroy(m_ODE_world) ;

    dCloseODE() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
StaticWorld*
StaticWorld::instance(void)
{
    static osg::ref_ptr<StaticWorld>    ref_world = new StaticWorld() ;
    static StaticWorld*                 world = ref_world.get() ;

    return world ;
}
/* ....................................................................... */
/* ======================================================================= */
