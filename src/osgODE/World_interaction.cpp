/*!
 * @file World_interaction.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2013 by Rocco Martino                            *
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
#include <osgODE/Notify>
#include <osgODE/World>
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
/* ....................................................................... */
/* ======================================================================= */




using namespace osgODE ;




/* ======================================================================= */
/* ....................................................................... */
bool
World::findInteractions(    InteractionCallback* cbk,
                            const osg::Vec3& from,
                            const osg::Vec3& to)
{
    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;

    bool    found = false ;


    while( itr != itr_end ) {

        ODEObject*  current = *itr++ ;

        if( _intersectRaySphere( current->getInteractingSphere(), from, to ) ) {
            (*cbk)(current, NULL) ;
            found = true ;
        }

    }



    {
        // free()
        osg::ref_ptr<InteractionCallback>   tmp(cbk) ;
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::findInteractions(    InteractionCallback* cbk,
                            const osg::BoundingSphere& sphere)
{
    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;

    bool    found = false ;


    while( itr != itr_end ) {

        ODEObject*  current = *itr++ ;

        if( _intersectSphereSphere( current->getInteractingSphere(), sphere ) ) {
            (*cbk)(current, NULL) ;
            found = true ;
        }

    }



    {
        // free()
        osg::ref_ptr<InteractionCallback>   tmp(cbk) ;
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
namespace {
class finder {
public:
    void    check(ODEObject* object)
    {
        if( object->getInteractingSphere().valid() )
            mObjectList.push_back(object) ;
    }

    World::Objects     mObjectList ;
} ;
}


/* ....................................................................... */
bool
World::findInteractions(    InteractionCallback* cbk )
{

    bool    found = false ;

    finder  fndr ;
    traverseObjects(fndr, &finder::check) ;


    Objects::iterator   i1 = fndr.mObjectList.begin() ;
    Objects::iterator   i2 = fndr.mObjectList.begin() ;
    Objects::iterator   end = fndr.mObjectList.end() ;



    while( i1 != end ) {

        ODEObject*  o1 = *i1 ++ ;



        const osg::BoundingSphere&  b1 = o1->getInteractingSphere() ;


        PS_ASSERT1( b1.valid() ) ;





        Objects::iterator   i2 = i1 ;

        while( i2 != end ) {

            ODEObject*  o2 = *i2 ++ ;



            const osg::BoundingSphere&  b2 = o2->getInteractingSphere() ;


            PS_ASSERT1( b2.valid() ) ;



            if( _intersectSphereSphere(b1, b2) ) {

                found = true ;

                if( cbk ) {
                    (*cbk)(o1, o2) ;


                } else {
                    ODECallback*    cbk1 = o1->getInteractionCallback() ;
                    ODECallback*    cbk2 = o2->getInteractionCallback() ;

                    if( cbk1 ) {
                        (*cbk1)( o2 ) ;
                    }

                    if( cbk2 ) {
                        (*cbk2)( o1 ) ;
                    }
                }
            }
        }

    }



    {
        // free()
        osg::ref_ptr<InteractionCallback>   tmp(cbk) ;
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
namespace {
class finder2 {
public:
    void    check(ODEObject* object)
    {
        if( object->getInteractingSphere().valid() && object->getInteractionCallback() != NULL )
            mObjectList.push_back(object) ;
    }

    World::Objects     mObjectList ;
} ;
}


/* ....................................................................... */
bool
World::findInteractions(    const osg::Vec3& from,
                            const osg::Vec3& to)
{

    bool    found = false ;

    finder2 fndr2 ;
    traverseObjects(fndr2, &finder2::check) ;


    Objects::iterator   i1 = fndr2.mObjectList.begin() ;
    Objects::iterator   i2 = fndr2.mObjectList.begin() ;
    Objects::iterator   end = fndr2.mObjectList.end() ;



    while( i1 != end ) {

        ODEObject*  obj = *i1++ ;



        const osg::BoundingSphere&  sphere = obj->getInteractingSphere() ;


        PS_ASSERT1( sphere.valid() ) ;



        if( _intersectRaySphere(sphere, from, to) ) {

            ODECallback*    cbk = obj->getInteractionCallback() ;

            PS_ASSERT1( cbk != NULL ) ;

            found = true ;

            (*cbk)(obj) ;
        }
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::_intersectRaySphere( const osg::BoundingSphere& sphere,
                            const osg::Vec3& from,
                            const osg::Vec3& to)
{
    if( ! sphere.valid() ) {
        return false ;
    }



    const double&       radius = sphere.radius() ;
    const osg::Vec3&    center = sphere.center() ;



    // V0: versore da to al centro della sfera
    osg::Vec3   V0 = center - to ;

    // controllo se to è interno alla sfera
    if( V0.normalize() <= radius ) {
        return true ;
    }



    // V1 è il versore da from al centro della sfera
    osg::Vec3   V1 = center - from ;

    // ip è la distanza da from al centro della sfera
    // salvo la lunghezza del vettore perché poi mi servirà per calcolare
    // l'altezza del triangolo che si va a formare fra from, il centro e la
    // proiezione di centro-from sulla retta del segmento
    const double    ip = V1.normalize() ;

    // controllo se from è interno alla sfera
    if( ip <= radius ) {
        return true ;
    }



    // Se siamo qui vuol dire che nessun estremo è interno alla sfera:
    // la prima condizione perché si verifichi la collisione è che la distanza
    // fra il centro della sfera e la retta su cui giace il segmento sia non
    // maggiore del raggio

    // V2 è il versore del segmento
    osg::Vec3   V2 = to - from ;

    // ne approfitto per controllare che la sfera non sia troppo distante per
    // consentire interazioni
    if( V2.normalize() < ip ) {
        return false ;
    }



    // controllo che la sfera non si trovi alle spalle del segmento
    if( V1*V2 < 0.0 ) {
        return false ;
    }



    // L'ipoten. è ip; il seno è il prod. vett. fra V1 e V2; la distanza è
    // sin * ip
    const double    CtoR = (V1^V2).length() * ip ;

    // Se la distanza è maggiore del raggio, di sicuro non c'è collisione
    if( CtoR > radius ) {
        return false ;
    }





    // A questo punto, se il raggio interseca, vuol dire che trapassa la sfera
    // da parte a parte; se è così, l'angolo compreso fra il vettore centro-to
    // e il vettore from-to deve essere acuto


    // da notare che V2 va da from a to, quindi il risultato va cambiato di
    // segno
    return V0*V2 < 0.0 ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::_intersectSphereSphere(  const osg::BoundingSphere& sphere1,
                                const osg::BoundingSphere& sphere2)
{
    if( ! (sphere1.valid() && sphere2.valid()) ) {
        return false ;
    }

    return (sphere1.center() - sphere2.center()).length() <= sphere1.radius() + sphere2.radius() ;
}
/* ....................................................................... */
/* ======================================================================= */
