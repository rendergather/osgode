/*!
 * @file ODEObjectContainer_serializer.cpp
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
#include <osgODE/ODEObjectContainer>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkODEObjects(const osgODE::ODEObjectContainer& container)
{
	(void) container ;

    return true ;
}

static bool writeODEObjects(osgDB::OutputStream& os, const osgODE::ODEObjectContainer& container)
{
    const osgODE::ODEObjectContainer::ObjectList&   objs = container.getObjectList() ;
    os << (unsigned long int)objs.size() << std::endl ;
    for(unsigned int i=0; i<objs.size(); i++) {
        os << objs[i].get() ;
    }
    return true ;
}

static bool readODEObjects(osgDB::InputStream& is, osgODE::ODEObjectContainer& container)
{
    unsigned int    size = 0 ;
    is >> size ;
    for(unsigned int i=0; i<size; i++) {
        osg::ref_ptr<osg::Object>   tmp = is.readObject() ;
        osgODE::ODEObject*  obj = dynamic_cast<osgODE::ODEObject*>(tmp.get()) ;
        if( obj ) {
            container.addObject(obj) ;
        }
    }
    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( ODEObjectContainer,
                         new osgODE::ODEObjectContainer,
                         osgODE::ODEObjectContainer,
                         "osg::Object osgODE::ODEObject osgODE::ODEObjectContainer" )
{

    ADD_USER_SERIALIZER(ODEObjects) ;
}
/* ....................................................................... */
/* ======================================================================= */
