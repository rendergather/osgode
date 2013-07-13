/*!
 * @file AnimationHelper_serializer.cpp
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

/* ======================================================================= */
/* ....................................................................... */
#include <osgODEUtil/AnimationHelper>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkAnimationManagerList(const osgODEUtil::AnimationHelper& animation_helper)
{
    (void) animation_helper ;

    return true ;
}

static bool writeAnimationManagerList(osgDB::OutputStream& os, const osgODEUtil::AnimationHelper& animation_helper)
{
    const osgODEUtil::AnimationHelper::AnimationManagerList&    objs = animation_helper.getAnimationManagerList() ;
    os << (unsigned long int)objs.size() << std::endl ;
    for(unsigned int i=0; i<objs.size(); i++) {
        os << objs[i].get() ;
    }
    return true ;
}

static bool readAnimationManagerList(osgDB::InputStream& is, osgODEUtil::AnimationHelper& animation_helper)
{
    unsigned int    size = 0 ;
    is >> size ;

    osgODEUtil::AnimationHelper::AnimationManagerList   objs ;

    for(unsigned int i=0; i<size; i++) {
        osg::ref_ptr<osg::Object>   tmp = is.readObject() ;
        osgODEUtil::AnimationManager*   obj = dynamic_cast<osgODEUtil::AnimationManager*>(tmp.get()) ;
        if( obj ) {
            objs.push_back(obj) ;
        }
    }

    animation_helper.setAnimationManagerList(objs) ;

    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( AnimationHelper,
                         new osgODEUtil::AnimationHelper,
                         osgODEUtil::AnimationHelper,
                         "osg::Object osgODE::ODECallback osgODEUtil::AnimationHelper" )
{
    ADD_USER_SERIALIZER( AnimationManagerList ) ;
}
/* ....................................................................... */
/* ======================================================================= */
