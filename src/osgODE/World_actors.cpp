/*!
 * @file World_actors.cpp
 * @author Rocco Martino
 */
/***************************************************************************
 *   Copyright (C) 2011 - 2014 by Rocco Martino                            *
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
unsigned int
World::findActors(  ActorCallback* cbk,
                    const osg::Vec3& from,
                    const osg::Vec3& to)
{

    if( m_dirty_actors ) {
        collectActors() ;
    }


    Objects::iterator   itr = m_actors.begin() ;
    Objects::iterator   itr_end = m_actors.end() ;

    unsigned int    found = 0 ;


    while( itr != itr_end ) {

        ODEObject*  current = *itr++ ;


        if( World::intersectRaySphere( current->getActorBound(), from, to ) ) {

            ++found ;

            if( (*cbk)(current) ) {
                return found ;
            }
        }

    }



    {
        // free()
        osg::ref_ptr<ActorCallback>   tmp(cbk) ;
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
World::findActors(  ActorCallback* cbk,
                    const osg::BoundingSphere& sphere,
                    unsigned int max_actors
                 )
{
    // free()
    osg::ref_ptr<ActorCallback>   tmp = cbk ;

    if( m_dirty_actors ) {
        collectActors() ;
    }


    Objects::iterator   itr = m_actors.begin() ;
    Objects::iterator   itr_end = m_actors.end() ;

    unsigned int    found = 0 ;


    if( cbk ) {
        while( itr != itr_end ) {

            ODEObject*  current = *itr++ ;

            if( World::intersectSphereSphere( current->getActorBound(), sphere ) ) {

                ++found ;

                if( (*cbk)(current) ) {
                    return found ;
                }

                if( found == max_actors ) {
                    return found ;
                }
            }
        }

    } else { // cbk
        while( itr != itr_end ) {

            ODEObject*  current = *itr++ ;

            if( World::intersectSphereSphere( current->getActorBound(), sphere ) ) {
                ++found ;

                if( found == max_actors ) {
                    return found ;
                }
            }
        }
    }


    return found ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
bool
World::intersectRaySphere(  const osg::BoundingSphere& sphere,
                            const osg::Vec3& from,
                            const osg::Vec3& to)
{
    if( ! sphere.valid() ) {
        return false ;
    }



    const ooReal&       radius = sphere.radius() ;
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
    const ooReal    ip = V1.normalize() ;

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
    const ooReal    CtoR = (V1^V2).length() * ip ;

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
World::intersectSphereSphere(   const osg::BoundingSphere& sphere1,
                                const osg::BoundingSphere& sphere2)
{
    if( ! (sphere1.valid() && sphere2.valid()) ) {
        return false ;
    }

    return (sphere1.center() - sphere2.center()).length() <= sphere1.radius() + sphere2.radius() ;
}
/* ....................................................................... */
/* ======================================================================= */




/* ======================================================================= */
/* ....................................................................... */
unsigned int
World::collectActors(void)
{
    m_actors.clear() ;

    Objects::iterator   itr = m_objects.begin() ;
    Objects::iterator   itr_end = m_objects.end() ;


    while( itr != itr_end ) {

        ODEObject*  obj = *itr++ ;

        PS_ASSERT1( obj ) ;

        if( obj->getActor() ) {
            m_actors.push_back( obj ) ;
        }
    }

    m_dirty_actors = false ;


    return m_actors.size() ;
}
/* ....................................................................... */
/* ======================================================================= */
