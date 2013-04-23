/*!
 * @file CharacterController.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2013 by Rocco Martino                                   *
 *   martinorocco@gmail.com                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
// #include <osgODE/CharacterController>
#include "CharacterController"
#include <osgODE/Character>
#include <osgODE/RigidBody>

#include <osgODE/World>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
CharacterController::CharacterController(Character* character):
    m_sensitivity(0.5),
    m_character(character),

    m_warp_pointer(false),
    m_motion(NO_MOTION),

    m_picked( NULL )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterController::CharacterController(const CharacterController& other, const osg::CopyOp& copyop):
    osgGA::GUIEventHandler(other, copyop),
    m_keys( other.m_keys ),
    m_sensitivity(other.m_sensitivity),
    m_character(other.m_character),

    m_warp_pointer(other.m_warp_pointer),
    m_motion( other.m_motion ),

    m_picked( other.m_picked )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterController::~CharacterController(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterController::setCharacter(Character* character)
{
    m_character = character ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::handle(    const osgGA::GUIEventAdapter& ea,
                                    osgGA::GUIActionAdapter& aa,
                                    osg::Object*,
                                    osg::NodeVisitor*)
{


    if( ! m_character.valid() ) {
        return false ;
    }




    switch( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::MOVE:
        case osgGA::GUIEventAdapter::DRAG:
            return _handleMOVE(ea, aa) ;




        case osgGA::GUIEventAdapter::KEYDOWN:
            return _handleKEYDOWN(ea, aa) ;




        case osgGA::GUIEventAdapter::KEYUP:
            return _handleKEYUP(ea, aa) ;




        case osgGA::GUIEventAdapter::PUSH:
            return _handlePUSH(ea, aa) ;


        case osgGA::GUIEventAdapter::RELEASE:
            return _handleRELEASE(ea, aa) ;




        default:
            return false ;
    }





    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::_handleMOVE(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    if( ! m_warp_pointer ) {
        float   x = ea.getXnormalized() * m_sensitivity ;
        float   y = ea.getYnormalized() * m_sensitivity ;

        if( x==0.0 && y == 0.0 ) {
            return true ;
        }

        double  yaw = m_character->getYaw() - x * osg::PI ;
        double  pitch = m_character->getPitch() + y * osg::PI ;



        pitch = osg::clampTo(pitch, 0.0, osg::PI) ;


        m_character->setYaw( yaw ) ;
        m_character->setPitch( pitch ) ;


        m_warp_pointer = true ;
        aa.requestWarpPointer( (ea.getXmax() - ea.getXmin()) * 0.5, (ea.getYmax() - ea.getYmin()) * 0.5) ;
    } else {
        m_warp_pointer = false ;
    }


    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::_handleKEYDOWN(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    int key = ea.getKey() ;


    if( key == m_keys.MoveFront ) {
        m_motion |= MOVE_FRONT ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveBack ) {
        m_motion |= MOVE_BACK ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveLeft ) {
        m_motion |= MOVE_LEFT ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveRight ) {
        m_motion |= MOVE_RIGHT ;
        _moveCharacter() ;
        return true ;
    }






    else if( key == m_keys.Jump ) {

        if( m_character->isOnGround()  &&  m_character->getWorld() != NULL ) {

            m_character->getWorld()->writeLock() ;

            m_character->setJump( osg::Z_AXIS * 1.0e3 / m_character->getWorld()->getCurrentStepSize(), 0.5 ) ;

            m_character->getWorld()->writeUnlock() ;

        }
        return true ;
    }


    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::_handleKEYUP(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    int key = ea.getKey() ;



    if( key == m_keys.MoveFront ) {
        m_motion &= ~MOVE_FRONT ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveBack ) {
        m_motion &= ~MOVE_BACK ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveLeft ) {
        m_motion &= ~MOVE_LEFT ;
        _moveCharacter() ;
        return true ;
    }


    else if( key == m_keys.MoveRight ) {
        m_motion &= ~MOVE_RIGHT ;
        _moveCharacter() ;
        return true ;
    }


    return false ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::_handlePUSH(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    const int   bm = ea.getButtonMask() ;

    const bool  primary = (bm & m_keys.PrimaryAction) != 0 ;
    const bool  secondary = (bm & m_keys.SecondaryAction) != 0 ;



    m_character->getWorld()->writeLock() ;



    if( m_picked.valid() ) {
        m_character->detach( m_picked ) ;
    }


    if( primary ) {

        if( m_picked.valid() ) {
            const osg::Vec3 direction = m_character->getBody()->getQuaternion() * m_character->getFrontVersor() ;

            m_picked->addForce( direction * 5.0e4 ) ;

            m_picked = NULL ;
        }

    }

    else if( secondary ) {

        m_picked = m_character->touch( 5 ) ;

        if( m_picked.valid() ) {
            m_character->attach( m_picked ) ;
        }

    }



    m_character->getWorld()->writeUnlock() ;


    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
CharacterController::_handleRELEASE(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    const int   bm = ea.getButtonMask() ;

    const bool  primary = (bm & m_keys.PrimaryAction) != 0 ;
    const bool  secondary = (bm & m_keys.SecondaryAction) != 0 ;



    m_character->getWorld()->writeLock() ;



    if( ! secondary ) {

        if( m_picked.valid() ) {
            m_character->detach( m_picked ) ;
            m_picked = NULL ;
        }
    }



    m_character->getWorld()->writeUnlock() ;




    return true ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
CharacterController::_moveCharacter(void)
{
    osg::Vec3   axis ;

    osg::Vec3   front_versor =  -osg::Z_AXIS ;
    osg::Vec3   rhs_versor =  osg::X_AXIS ;
    front_versor.normalize() ;

    if( m_motion & MOVE_FRONT ) {
        axis += front_versor ;
    }

    if( m_motion & MOVE_BACK ) {
        axis -= front_versor ;
    }

    if( m_motion & MOVE_RIGHT ) {
        axis += rhs_versor ;
    }

    if( m_motion & MOVE_LEFT ) {
        axis -= rhs_versor ;
    }

    axis.normalize() ;


    m_character->getWorld()->writeLock() ;

    m_character->setMotion( axis * (m_motion != 0) * 40.0, 200 ) ;

    m_character->getWorld()->writeUnlock() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterController::Keys::Keys(void)
{
    MoveFront       = 'w' ;
    MoveBack        = 's' ;
    MoveLeft        = 'a' ;
    MoveRight       = 'd' ;
    Jump            = osgGA::GUIEventAdapter::KEY_Space ;
    PrimaryAction   = osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ;
    SecondaryAction = osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
CharacterController::Keys::Keys(const CharacterController::Keys& other)
{
    MoveFront       = other.MoveFront ;
    MoveBack        = other.MoveBack ;
    MoveLeft        = other.MoveLeft ;
    MoveRight       = other.MoveRight ;
    Jump            = other.Jump ;
    PrimaryAction   = other.PrimaryAction ;
    SecondaryAction = other.SecondaryAction ;
}
/* ....................................................................... */
/* ======================================================================= */
