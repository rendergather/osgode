/*!
 * @file ThreadedManagerUpdateCallback.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2014 by Rocco Martino                                   *
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/ThreadedManagerUpdateCallback>
#include <osgODE/ThreadedManager>
#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
ThreadedManagerUpdateCallback::ThreadedManagerUpdateCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManagerUpdateCallback::ThreadedManagerUpdateCallback(const ThreadedManagerUpdateCallback& other, const osg::CopyOp& copyop):
    osg::NodeCallback       ( other, copyop )
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
ThreadedManagerUpdateCallback::~ThreadedManagerUpdateCallback(void)
{
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
ThreadedManagerUpdateCallback::operator()(osg::Node* n, osg::NodeVisitor* nv)
{
    PS_ASSERT1( dynamic_cast<ThreadedManager*> (n) ) ;

    ThreadedManager*    manager = static_cast<ThreadedManager*>(n) ;

    World*  world = manager->getWorld() ;

    PS_ASSERT1( world ) ;



    if(     !  ( manager->getDone()  ||  manager->isRunning() )  ) {

        manager->startThread() ;
    }


    bool    rdy = manager->rdy() ;


    if( rdy ) {

        manager->pause() ;

        world->updateTransformsInternal() ;
        world->runOperationsInternal() ;



        const Events::ViewMatrixRequests&   view_matrix_requests = world->getFrontEventsBuffer()->getViewMatrixRequests() ;

        for( unsigned int i=0; i<view_matrix_requests.size(); i++ ) {
            view_matrix_requests[i].first->getCamera()->setViewMatrix( view_matrix_requests[i].second ) ;
        }

        world->swapEventsBuffer() ;


        manager->rdy(false) ;
        manager->unpause() ;

    }



    traverse( n, nv ) ;
}
/* ....................................................................... */
/* ======================================================================= */
