/*!
 * @file Viewer.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include "Viewer.hpp"

#include <osgViewer/Viewer>

#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
setupViewer(osgViewer::Viewer* viewer, osg::Node* scene_data, float w, float h)
{
    PS_DBG("%s", "setupViewer()") ;


#if defined(ANDROID)
    viewer->setUpViewerAsEmbeddedInWindow(0, 0, w, h) ;

	viewer->getCamera()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF) ;
#endif



    viewer->setSceneData(scene_data) ;
    viewer->setCameraManipulator(NULL) ;


    viewer->realize() ;


    viewer->getCamera()->setViewMatrixAsLookAt( osg::Z_AXIS * 40, -osg::Z_AXIS, osg::Y_AXIS ) ;
    viewer->getCamera()->setClearColor( osg::Vec4(0.5, 0.5, 0.5, 1.0) ) ;
}
/* ....................................................................... */
/* ======================================================================= */
