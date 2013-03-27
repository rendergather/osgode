/*!
 * @file HashTable.inl
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

#ifndef _OSGODEUTIL_HASHTABLE_INL
#define _OSGODEUTIL_HASHTABLE_INL

/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline osg::Object*
osgODEUtil::HashTable::getOrReset(const std::string& key)
{
    osg::Object*    obj = get(key) ;

    if( ! obj ) {
        reset(key) ;
    }

    return obj ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODEUtil::HashTable::clear(void)
{
    delete[] m_pairs ;
    m_pairs = new osg::ref_ptr<Pair>[m_table_size] ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODEUtil::HashTable::setTableSize(unsigned int size)
{
    delete[] m_pairs ;

    m_table_size = size ;

    m_pairs = new osg::ref_ptr<Pair>[m_table_size] ;

#if !( defined(ANDROID) || defined(WIN32) )
    m_right_shift = 32 - log2(size) ;
#else
    m_right_shift = 32 - log(size) / 0.69314718055994529 ;
#endif
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODEUtil::HashTable::getTableSize(void) const
{
    return m_table_size ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODEUtil::HashTable::setConstant(float constant)
{
    m_constant = constant ;
    m_hash_fdf = (unsigned int) (constant * 4294967296.0f) ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline float
osgODEUtil::HashTable::getConstant(void) const
{
    return m_constant ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
template<class T>
inline void
osgODEUtil::HashTable::traverseObjects(T& t, void (T::*member)(osg::Object*))
{
    for(unsigned int i=0; i<m_table_size; i++) {

        for(Pair* pair = m_pairs[i]; pair != NULL; pair = pair->getNext()) {
            (t.*member)( pair->getValue() ) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline void
osgODEUtil::HashTable::traverseObjects( void (*fn)(osg::Object*) )
{
    for(unsigned int i=0; i<m_table_size; i++) {

        for(Pair* pair = m_pairs[i]; pair != NULL; pair = pair->getNext()) {
            fn( pair->getValue() ) ;
        }
    }
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODEUtil::HashTable::_hash(const char* key) const
{
    unsigned int    h = 0 ;

    for(unsigned char c = *key; c ; c = *key++) {
        unsigned int    hi = h & 0xF8000000 ;
        h = h << 5 ;
        hi = hi >> 27 ;
        h ^= c ;
        h ^= hi ;
    }

    return h ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODEUtil::HashTable::_lookup(unsigned int hash) const
{
    const unsigned int  x = hash * m_hash_fdf ;

    return x >> m_right_shift ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
inline unsigned int
osgODEUtil::HashTable::_lookup(const std::string& key) const
{
    return _lookup( _hash( key.c_str() ) ) ;
}
/* ....................................................................... */
/* ======================================================================= */




#endif /* _OSGODEUTIL_HASHTABLE_INL */
