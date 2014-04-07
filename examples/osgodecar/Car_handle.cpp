/*!
 * @file Car_handle.cpp
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
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "Car"

#include <osgODE/Engine>
#include <osgODE/SuspensionJoint>
#include <osgODE/DifferentialJoint>
#include <osgODE/Notify>

#include <osgGA/GUIEventAdapter>




using namespace osgODE ;




void
Car::handleKeyDown(const int& key, const int& mod_key_mask, bool& handled)
{
    const double    R_BRAKE_FORCE = 250.0 ;
    const double    F_BRAKE_FORCE = 500.0 ;


    switch( key )
    {
        case 'w':
            m_engine->setGas(1.0) ;

            handled = true ;
        break ;




        case 's':
            m_hinge_RL->setParam(dParamVel2, 0.0) ;
            m_hinge_RL->setParam(dParamFMax2, R_BRAKE_FORCE) ;

            m_hinge_RR->setParam(dParamVel2, 0.0) ;
            m_hinge_RR->setParam(dParamFMax2, R_BRAKE_FORCE) ;

            m_hinge_FR->setParam(dParamVel2, 0.0) ;
            m_hinge_FR->setParam(dParamFMax2, F_BRAKE_FORCE) ;

            m_hinge_FL->setParam(dParamVel2, 0.0) ;
            m_hinge_FL->setParam(dParamFMax2, F_BRAKE_FORCE) ;

            handled = true ;
        break ;




        case 'a':
            m_steering_left = true ;

            handled = true ;
        break ;




        case 'd':
            m_steering_right = true ;

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Up:
            m_engine->setCurrentGear( m_engine->getCurrentGear() + 1 ) ;

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Down:

            if( m_engine->getCurrentGear() > 0 ) {
                m_engine->setCurrentGear( m_engine->getCurrentGear() - 1 ) ;
            }

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Left:

            getBody()->setCameraManipulatorCenter( osg::Vec3(7.5, 0.0, 1.0) ) ;

            getBody()->setCameraManipulatorDirection( -osg::X_AXIS ) ;

            getBody()->setCameraManipulatorElasticity( 5 ) ;

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Right:

            getBody()->setCameraManipulatorCenter( osg::Vec3(-7.5, 0.0, 1.0) ) ;

            getBody()->setCameraManipulatorDirection( osg::X_AXIS ) ;

            getBody()->setCameraManipulatorElasticity( 5 ) ;

            handled = true ;
        break ;



        default:
            handled = false ;
        break ;
    }
}




void
Car::handleKeyUp(const int& key, const int& mod_key_mask, bool& handled)
{

    switch( key )
    {
        case 'w':
            m_engine->setGas(0.0) ;

            handled = true ;
        break ;




        case 's':
            m_hinge_RL->setParam(dParamVel2, 0.0) ;
            m_hinge_RL->setParam(dParamFMax2, 0.0) ;

            m_hinge_RR->setParam(dParamVel2, 0.0) ;
            m_hinge_RR->setParam(dParamFMax2, 0.0) ;

            m_hinge_FR->setParam(dParamVel2, 0.0) ;
            m_hinge_FR->setParam(dParamFMax2, 0.0) ;

            m_hinge_FL->setParam(dParamVel2, 0.0) ;
            m_hinge_FL->setParam(dParamFMax2, 0.0) ;

            handled = true ;
        break ;




        case 'a':
            m_steering_left = false ;

            handled = true ;
        break ;




        case 'd':
            m_steering_right = false ;

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Left:

            getBody()->setCameraManipulatorCenter( osg::Vec3(0.0, -7.5, 1.0) ) ;

            getBody()->setCameraManipulatorDirection( osg::Y_AXIS ) ;

            getBody()->setCameraManipulatorElasticity( 10 ) ;

            handled = true ;
        break ;




        case osgGA::GUIEventAdapter::KEY_Right:

            getBody()->setCameraManipulatorCenter( osg::Vec3(0.0, -7.5, 1.0) ) ;

            getBody()->setCameraManipulatorDirection( osg::Y_AXIS ) ;

            getBody()->setCameraManipulatorElasticity( 10 ) ;

            handled = true ;
        break ;




        default:
            handled = false ;
        break ;
    }
}
