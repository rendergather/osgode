/*!
 * @file Events_serializer.cpp
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
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/* ======================================================================= */
/* ....................................................................... */
#include <osgODE/Events>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkEventList(const osgODE::Events& events)
{
    return events.getEventList().size() != 0 ;
}

static bool writeEventList(osgDB::OutputStream& os, const osgODE::Events& events)
{
    const osgODE::Events::EventList&    event_list = events.getEventList() ;

    os << os.BEGIN_BRACKET << std::endl ;

    os.writeSize( event_list.size() ) ;

    for(unsigned int i=0; i<event_list.size(); i++) {
        os.writeObject( event_list[i].first.get() ) ;
        os.writeObject( event_list[i].second.get() ) ;
    }

    os << os.END_BRACKET << std::endl ;

    return true ;
}

static bool readEventList(osgDB::InputStream& is, osgODE::Events& events)
{
    is >> is.BEGIN_BRACKET ;

    unsigned int    size = is.readSize() ;


    for(unsigned int i=0; i<size; i++) {
        osg::ref_ptr<osgGA::GUIEventAdapter>    ea = dynamic_cast<osgGA::GUIEventAdapter*>( is.readObject() ) ;
        osg::ref_ptr<osg::View>                 aa = dynamic_cast<osg::View*>( is.readObject() ) ;

        if( ea.valid() ) {
            events.enqueue( ea, aa ) ;
        }
    }

    is >> is.END_BRACKET ;


    return true ;
}
} // anon namespace
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( Events,
                         new osgODE::Events,
                         osgODE::Events,
                         "osg::Object osgODE::Events" )
{
    ADD_USER_SERIALIZER( EventList ) ;
}
/* ....................................................................... */
/* ======================================================================= */
