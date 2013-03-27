/*!
 * @file Viewer.hpp
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

#ifndef _VIEWER_HPP
#define _VIEWER_HPP




/* ======================================================================= */
#include <osg/Export>
/* ======================================================================= */




/* ======================================================================= */
namespace osgViewer {
    class Viewer ;
    class Node ;
}
/* ======================================================================= */




/* ======================================================================= */
namespace osg {
    class Node ;
}
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
// w e h tenute in considerazione solo se #defined(ANDROID) == true, nel
// qual caso viene chiamata la
// osgViewer::Viewer::setUpViewerAsEmbeddedInWindow(0, 0, w, h)
extern OSG_EXPORT void
setupViewer(osgViewer::Viewer* viewer, osg::Node* scene_data, float w, float h) ;
/* ....................................................................... */
/* ======================================================================= */




#endif /* _VIEWER_HPP */
