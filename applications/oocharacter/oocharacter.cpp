/*!
 * @file oocharacter.cpp
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

#include <osgODE/Notify>

#include "Action"


using namespace osgODE ;




typedef struct {
    osg::ref_ptr<Character> mCharacter ;
} Data ;




Action* read_actions(int argc, char** argv)
{
    osg::ArgumentParser ap(&argc, argv) ;



    Action*     action = new NullAction() ;


    std::string     str_par ;

    while( ap.read("--create") ) {
        action->add( new CreateAction() ) ;
        action->add( new InitAction() ) ;
    }

    while( ap.read("--open", str_par) ) {
        action->add( new OpenAction(str_par) ) ;
    }

//     while( ap.read("--init") ) {
//         action->add( new InitAction() ) ;
//     }

    while( ap.read("--set-first-person", str_par) ) {
        action->add( new SetFirstPersonAction(str_par) ) ;
    }

    while( ap.read("--write", str_par) ) {
        action->add( new WriteAction(str_par) ) ;
    }


    return action ;
}








int
main(int argc, char** argv)
{
    osg::ref_ptr<Character> character ;

    osg::ref_ptr<Action>    action = read_actions(argc, argv) ;

    PS_ASSERT1( action ) ;


    while( action.valid() ) {

        character = (*action)( character ) ;

        action = action->getNext() ;

    }


    return 0 ;
}

