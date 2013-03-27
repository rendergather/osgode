/*!
 * @file HashTable.cpp
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

#include <osgODE/Notify>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODEUtil ;




/* ======================================================================= */
/* ....................................................................... */
HashTable::HashTable(unsigned int table_size):
    m_pairs(NULL)
{
    setTableSize(table_size) ;

    setConstant( 0.5f * sqrtf(5.0f) - 1.0f ) ;


    m_pairs = new osg::ref_ptr<Pair>[m_table_size] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HashTable::HashTable(const HashTable& other, const osg::CopyOp& copyop):
    osg::Object(other, copyop),
    m_table_size( other.m_table_size ),
    m_pairs(NULL),
    m_constant(other.m_constant),
    m_right_shift(other.m_right_shift),
    m_hash_fdf(other.m_hash_fdf)
{
    m_pairs = new osg::ref_ptr<Pair>[m_table_size] ;


    for(unsigned int i=0; i<m_table_size; i++) {
        m_pairs[i] = other.m_pairs[i] ;
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HashTable::~HashTable(void)
{
    delete[] m_pairs ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
HashTable::set(const std::string& key, osg::Object* value)
{
    const unsigned int  array_idx = _lookup(key) ;

    Pair*   pair = m_pairs[array_idx] ;

    if( ! pair ) {
        m_pairs[array_idx] = new Pair(key, value) ;
        return ;
    }





    do {
        if( pair->getKey() == key ) {
            pair->setValue(value) ;
            break ;
        }


        if( pair->getNext() == NULL ) {
            pair->setNext( new Pair(key, value) ) ;
            break ;
        }


        pair = pair->getNext() ;

    } while( true ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
HashTable::set(Pair* pair)
{
    const unsigned int  array_idx = _lookup( pair->getKey() ) ;

    Pair*   current_pair = m_pairs[array_idx] ;

    if( ! current_pair ) {
        m_pairs[array_idx] = pair ;
        return ;
    }





    do {
        if( current_pair->getKey() == pair->getKey() ) {
            m_pairs[array_idx] = pair ;
            break ;
        }


        if( current_pair->getNext() == NULL ) {
            current_pair->setNext( pair ) ;
            break ;
        }


        current_pair = current_pair->getNext() ;

    } while( true ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
void
HashTable::reset(const std::string& key)
{
    const unsigned int  array_idx = _lookup(key) ;

    Pair*   pair = m_pairs[array_idx] ;

    if( ! pair ) {
        return ;
    }



    if( pair->getKey() == key ) {

        m_pairs[array_idx] = pair->getNext() ;

        return ;
    }


    do {

        Pair*   next_pair = pair->getNext() ;


        if( next_pair == NULL ) {
            break ;
        }

        if( next_pair->getKey() == key ) {
            pair->setNext( next_pair->getNext() ) ;
            break ;
        }


        pair = next_pair ;

    } while( true ) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
osg::Object*
HashTable::get(const std::string& key) const
{
    const unsigned int  array_idx = _lookup(key) ;

    Pair*   pair = m_pairs[array_idx] ;


    while(pair) {

        if( pair->getKey() == key ) {
            return pair->getValue() ;
        }

        pair = pair->getNext() ;

    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
HashTable::Pair*
HashTable::get(unsigned int idx) const
{
    PS_ASSERT1(idx < m_table_size) ;

    if( idx < m_table_size ) {
        return m_pairs[idx] ;
    }

    return NULL ;
}
/* ....................................................................... */
/* ======================================================================= */
