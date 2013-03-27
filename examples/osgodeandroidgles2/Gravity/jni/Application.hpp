/*!
 * @file Application.hpp
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

#ifndef _OSGODE_APPLICATION_HPP
#define _OSGODE_APPLICATION_HPP




/* ======================================================================= */
#include <osg/Export>
#include <osg/observer_ptr>
/* ======================================================================= */




/* ======================================================================= */
namespace osgODE {
    class Manager ;
}
/* ======================================================================= */




/* ======================================================================= */
namespace osgViewer {
    class Viewer ;
}
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
//!
/*!
 */
class OSG_EXPORT Application
{
/* ======================================================================= */
private:
             Application(void) ;
             Application(const Application& other) ;
            ~Application(void) ;
/* ======================================================================= */









/* ======================================================================= */
public:
    void    init(float w=-1.0, float h=-1.0) ;
    void    frame(void) ;
    void    quit(void) ;

    bool    done(void) ;

    void    setGravity(float x, float y, float z) ;
/* ======================================================================= */




/* ======================================================================= */
public:
    static Application&     instance(void) ;
/* ======================================================================= */




/* ======================================================================= */
private:
    osg::observer_ptr<osgODE::Manager>  m_manager ;

    osg::ref_ptr<osgViewer::Viewer>     m_viewer ;
/* ======================================================================= */
} ;
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODE_APPLICATION_HPP */
