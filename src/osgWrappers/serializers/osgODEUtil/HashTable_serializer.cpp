/*!
 * @file Serializer_HashTable.cpp
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
#include <osgODEUtil/HashTable>

#include <osgDB/Registry>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
namespace {
static bool checkPairs(const osgODEUtil::HashTable& table)
{
    (void) table ;
    return true ;
}

static bool writePairs(osgDB::OutputStream& os, const osgODEUtil::HashTable& table)
{
    std::vector< osg::ref_ptr<osgODEUtil::HashTable::Pair> >    pairs ;

    for(unsigned int i=0; i<table.getTableSize(); i++) {
        osgODEUtil::HashTable::Pair*    pair = table.get(i) ;

        if( pair ) {
            pairs.push_back(pair) ;
        }
    }


    os << (unsigned long int)pairs.size() << std::endl ;

    for(unsigned int i=0; i<pairs.size(); i++) {
        os << pairs[i].get() ;
    }
    return true ;
}

static bool readPairs(osgDB::InputStream& is, osgODEUtil::HashTable& table)
{
    unsigned int    size = 0 ;
    is >> size ;
    for(unsigned int i=0; i<size; i++) {
        osg::ref_ptr<osg::Object>   tmp = is.readObject() ;
        osgODEUtil::HashTable::Pair*    obj = dynamic_cast<osgODEUtil::HashTable::Pair*>(tmp.get()) ;
        if( obj ) {
            table.set(obj) ;
        }
    }
    return true ;
}
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
REGISTER_OBJECT_WRAPPER( HashTable,
                         new osgODEUtil::HashTable,
                         osgODEUtil::HashTable,
                         "osg::Object osgODEUtil::HashTable" )
{

    ADD_UINT_SERIALIZER(TableSize, 256) ;
    ADD_FLOAT_SERIALIZER(Constant,  0.5f * sqrtf(5.0f) - 1.0f ) ;

    ADD_USER_SERIALIZER(Pairs) ;
}
/* ....................................................................... */
/* ======================================================================= */
